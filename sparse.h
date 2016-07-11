/*
    sparse.h
    Kevin Perez
    last modified: 7/8/16

    implementations missing:
    (1) SparseVector<N> operator* (const SparseMatrix<N>& m , const SparseVector<N>& v)
    (2) SparseMatrix<N> operator* (const SparseMatrix<N>& a , const SparseMatrix<N>& b)
    
    Retrieve(i,j,value) / Retrieve(i, value) is used to prevent calls to the element operators 
    that might expand the size of the object unnecessarily. If the key(s) being searched is in 
    the table, returns true and sets value to the value associated with the key. Else it returns 
    false and the value is unspecified. See hashtbl.h for more.

    Pair<> MaxIndicies() const returns the largest row and largest col index in the SparseMatrix 
    as a pair of unsigned ints. These are useful in utilities such as 
    Convert( Matrix& , const SparseMatrix& ) where the size of a matrix needs to be adjusted 
    to fit a sparse matrix.
    

*/

#include <iostream>
#include <iomanip>
#include <fstream>

#include <hashtbl.h>
#include <hashclasses.h>
#include <hashfunctions.h>

#ifndef _SPARSE_H
#define _SPARSE_H

namespace fsu
{

  //---------------------
  //   class SparseVector
  //---------------------

  template < typename N >
  class SparseVector
  {

  public:
    typedef N  ValueType;
    ValueType&       operator [] (size_t i)       { return val_[i]; }
    const ValueType& operator [] (size_t i) const { return val_[i]; }

    typedef size_t                                             KeyType;
    typedef N                                                  DataType;
    typedef hashclass::KISS < size_t >                         HashType;
    typedef fsu::Entry      < KeyType , DataType >             EntryType;
    typedef fsu::List       < EntryType >                      BucketType;
    typedef fsu::HashTable  < KeyType , DataType , HashType >  TableType;
    typedef typename TableType::Iterator                       Iterator;

    Iterator Begin () const { return val_.Begin(); }
    Iterator End   () const { return val_.End(); }

    bool     Retrieve   (size_t i, N& n) const { return val_.Retrieve(i,n); }
    size_t   NumEntries () const { return val_.Size(); }
    size_t   MaxIndex   () const;

    explicit SparseVector      (size_t size = 100) : val_(size) {}
    SparseVector               (const SparseVector& v) : val_(v.val_) {}
    virtual ~SparseVector      () {}
    SparseVector<N>& operator= (const SparseVector<N>& v) { val_ = v.val_; return *this; }
    void Clear                 () { val_.Clear(); }

    void Rehash ( size_t size = 0 ) { val_.Rehash(size); }

  private: 
    TableType val_;
  };

  template < typename N >
  size_t SparseVector<N>::MaxIndex () const
  {
    size_t max = 0; 
    for (Iterator i = Begin(); i != End(); ++i)
      if ((*i).key_ > max) max = (*i).key_;
    return max;
  }

  //---------------------
  //   class SparseMatrix
  //---------------------

  template < typename N >
  class SparseMatrix
  {

  public:
    typedef N                   ValueType;
    typedef SparseVector<N>     RowType;

    // element access
    RowType&       operator   [] (size_t i)                 { return row_[i]; }
    const RowType& operator   [] (size_t i) const           { return row_[i]; }
    ValueType& operator       () (size_t i, size_t j)       { return (*this)[i][j]; }
    const ValueType& operator () (size_t i, size_t j) const { return (*this)[i][j]; }

    // proper type
    explicit SparseMatrix (size_t numrows = 100) : row_(numrows) {}
    SparseMatrix (const SparseMatrix& m) : row_(m.row_) {}
    virtual ~SparseMatrix() { Clear(); }
    void Clear() { row_.Clear(); }
    SparseMatrix<N>& operator=  (const SparseMatrix<N>& m) { row_ = m.row_; return *this; }

    // other terminology support
    typedef size_t                                             KeyType;
    typedef RowType                                            DataType;
    typedef hashclass::KISS < size_t >                         HashType;
    typedef fsu::Entry      < KeyType , DataType >             EntryType;
    typedef fsu::HashTable  < KeyType , DataType , HashType >  TableType;
    typedef typename TableType::Iterator Iterator;

    // iterator support
    Iterator Begin () const { return row_.Begin(); }
    Iterator End   () const { return row_.End(); }

    // informational
    size_t NumEntries() const;
    fsu::Pair<size_t, size_t> MaxIndices() const;
    bool Retrieve (size_t i, size_t j, N& n) const;
		// bool Retrieve (size_t i, RowType& r) const;
    // improve structural efficiency
    void Rehash ( size_t size = 0 );

  private: 
    fsu::HashTable < size_t , SparseVector<N> , hashclass::KISS < size_t > > row_;
  };

  template < typename N >
  bool SparseMatrix<N>::Retrieve (size_t i, size_t j, N& n) const
  {
    RowType r;
    if (!row_.Retrieve(i,r))
      return 0;
    if (!r.Retrieve(j,n))
      return 0;
    return 1;
  }

  template < typename N >
  size_t SparseMatrix<N>::NumEntries() const
  {
    size_t count(0);
    for (Iterator i = row_.Begin(); i != row_.End(); ++i)
    {
      count += (*i).data_.NumEntries();
    }
    return count;
  }

  template < typename N >
  fsu::Pair<size_t, size_t> SparseMatrix<N>::MaxIndices() const
  {
    fsu::Pair<size_t,size_t> p(0,0);
    for (Iterator i = row_.Begin(); i != row_.End(); ++i)
    {
      if (p.first_ < (*i).key_) p.first_ = (*i).key_;
      for (typename RowType::Iterator j = (*i).data_.Begin(); j != (*i).data_.End(); ++j)
        if (p.second_ < (*j).key_) p.second_ = (*j).key_;
    }
    return p;
  }
    
  template < typename N >
  void SparseMatrix<N>::Rehash ( size_t size )
  {
    for (Iterator i = row_.Begin(); i != row_.End(); ++i)
    {
      (*i).data_.Rehash();
    }
    row_.Rehash(size);
  }

} // namespace fsu


//---------------------
//   global operators
//---------------------

/*
  In sparse_util.h there are these useful methods: 
  Convert ( fsu::SparseVector<N>& , const fsu::Vector y)
  Convert ( fsu::Vector& ,          const fsu::SparseVector<N> )
  Convert ( fsu::SparseMatrix<N>& , const fsu::Matrix<N> )
  Convert ( fsu::Matrix<N>& ,       const fsu::SparseMatrix<N> )

  The implementation of the product operators is required to be space conservative, 
  meaning that no unnecessary new elements of SparseVectors or SparseMatrices are 
  created by the product operators.

  The implementation of SparseMatrix * SparseVector should have runtime <= O(n), 
  where n is the number of (non-zero) elements of the SparseMatrix.
	
 */
template < typename N >
fsu::SparseVector<N> operator* (const fsu::SparseMatrix<N>& a, const fsu::SparseVector<N>& v)
{
  fsu::SparseVector <N> result;
	typename fsu::SparseMatrix<N>::Iterator mitr;
	// Iterator through matrix rows
	for(mitr = a.Begin(); mitr != a.End(); ++mitr)
	{
		N currentRow = (*mitr).key_;
		typename fsu::SparseVector<N>::Iterator mvitr1;
		mvitr1 = ((*mitr).data_).Begin(); // HashTableIterator
		// iterating through current Matrix row
		for(;mvitr1 != ((*mitr).data_).End(); ++mvitr1)
		{
			N column = (*mvitr1).key_;
			N vectorValue = 0;
			if(v.Retrieve(column,vectorValue)){
				result[currentRow] += vectorValue*(*mvitr1).data_;
			}
		}
		
	}
  return result;
} // SM*SV 

/*
template < typename N >
fsu::SparseMatrix<N> operator* (const fsu::SparseMatrix<N>& a, const fsu::SparseMatrix<N>& b)
{
  fsu::SparseMatrix <N> x;
  return x;
} // SM*SM 
*/
#endif
