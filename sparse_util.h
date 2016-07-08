/* 
   sparse_util.h

*/

#include <sparse.h>

#ifndef _SPARSE_UTIL_H
#define _SPARSE_UTIL_H

template <typename T>
void Display (const fsu::SparseMatrix<T>& m, int cw, std::ostream& os = std::cout)
{
  typename fsu::SparseMatrix<T>::Iterator i;
  typename fsu::SparseMatrix<T>::RowType::Iterator j;
  for (i = m.Begin(); i != m.End(); ++i)
  {
    for (j = (*i).data_.Begin(); j != (*i).data_.End(); ++j)
    {
      os << " (" << (*i).key_ << ',' << (*j).key_ << ')' 
         << std::setw(cw) << " = " << (*j).data_
         << '\n'; 
    }
    // os << '\n';
  }
} // Display SparseMatrix

template <typename N>
void Display (const fsu::SparseVector<N>& v, int cw, std::ostream& os = std::cout)
{
  typename fsu::SparseVector<N>::Iterator j;
  for (j = v.Begin(); j != v.End(); ++j)
  {
    os << " [" << (*j).key_ << ']' 
       << std::setw(cw) << " = " << (*j).data_
       << '\n'; 
  }
  // os << '\n';
} // Display SparseVector

template < typename N >
bool Load (fsu::SparseVector<N>& v, const char* filename)
{
  std::ifstream ifs(filename);
  if (ifs.fail())
  {
    std::cerr << " ** Load Failure: Unable to open file " << filename << '\n';
    return 0;
  }

  size_t j;
  N n;
  while (ifs >> j >> n)
  {
    v[j] = n;
  }
  ifs.close();
  return 1;
} // Load SparseVector

template < typename N >
bool Load (fsu::SparseMatrix<N>& m, const char* filename)
{
  std::ifstream ifs(filename);
  if (ifs.fail())
  {
    std::cerr << " ** Load Failure: Unable to open file " << filename << '\n';
    return 0;
  }
  size_t i, j; N n;
  while (ifs >> i >> j >> n)
    m(i,j) = n;
  ifs.close();
  return 1;
} // Load SparseMatrix

template < typename N >
void Convert (fsu::SparseVector<N>& x, const fsu::Vector<N> y)
{
  x.Clear();
  for (size_t j = 0; j < y.Size(); ++j)
  {
    if (y[j] != 0)
    {
      x[j] = y[j];
    }
  }
} // Convert (SparseVector, Vector)

template < typename N >
void Convert (fsu::Vector<N>& x, const fsu::SparseVector<N> y)
{
  x.Clear();
  x.SetSize(y.MaxIndex()+1,N());
  typename fsu::SparseVector<N>::Iterator j;
  for (j = y.Begin(); j != y.End(); ++j)
  {
    // size_t index = (*j).key_;
    // if (x.Size() <= index)
    // x.SetSize(index + 1,N());
    x[(*j).key_] = (*j).data_; 
  }
} // Convert (Vector, SparseVector)

template < typename N >
void Convert (fsu::SparseMatrix<N>& x, const fsu::Matrix<N> y)
{
  x.Clear();
  for (size_t i = 0; i < y.NumberOfRows(); ++i)
  {
    for (size_t j = 0; j < y.NumberOfCols(); ++j)
    {
      if (y(i,j) != 0)
      {
        x(i,j) = y(i,j);
      }
    }
  }
} // Convert (SparseMatrix, Matrix)

template < typename N >
void Convert (fsu::Matrix<N>& x, const fsu::SparseMatrix<N> y)
{
  x.Clear();
  fsu::Pair<size_t,size_t> p = y.MaxIndices();
	//setting row and column size and data type 
  x.SetSize(1+p.first_, 1+p.second_, N());
  typename fsu::SparseMatrix<N>::Iterator i;
  typename fsu::SparseMatrix<N>::RowType::Iterator j;
  for (i = y.Begin(); i != y.End(); ++i)
  {
    for (j = (*i).data_.Begin(); j != (*i).data_.End(); ++j)
    {
      x((*i).key_,(*j).key_) = (*j).data_; 
    }
  }
} // Convert (Matrix, SparseMatrix)

template < typename N >
bool Save (const fsu::SparseMatrix<N>& m, const char* filename)
{
  std::ofstream ofs(filename);
  if (ofs.fail())
  {
    std::cerr << " ** Save Failure: Unable to open file " << filename << '\n';
    return 0;
  }
  typename fsu::SparseMatrix<N>::Iterator i;
  typename fsu::SparseMatrix<N>::RowType::Iterator j;
  for (i = m.Begin(); i != m.End(); ++i)
  {
    for (j = (*i).data_.Begin(); j != (*i).data_.End(); ++j)
    {
      ofs << (*i).key_ << '\t' << (*j).key_ << '\t' << (*j).data_ << '\n'; 
    }
  }
  if (ofs.fail())
  {
    std::cerr << " ** Save Failure: data write failure to file " << filename << '\n';
    ofs.clear();
    ofs.close();
    return 0; 
  }
  ofs.close();
  return 1;
} // Save SparseMatrix

template < typename N >
bool Save (const fsu::SparseVector<N>& v, const char* filename)
{
  std::ofstream ofs(filename);
  if (ofs.fail())
  {
    std::cerr << " ** Save Failure: Unable to open file " << filename << '\n';
    return 0;
  }

  typename fsu::SparseVector<N>::Iterator j;
  for (j = v.Begin(); j != v.End(); ++j)
  {
    ofs << (*j).key_ << '\t' << (*j).data_ << '\n';
  }

  if (ofs.fail())
  {
    std::cerr << " ** Save Failure: data write failure in file " << filename << '\n';
    ofs.clear();
    ofs.close();
    return 0; 
  }
  ofs.close();
  return 1;
} // Save SparseVector

template < typename N >
void ConstTest ( const fsu::SparseVector<N> & v ,  size_t i, const fsu::SparseMatrix<N> m, size_t j, size_t k )
{
  std::cout << "const v[" << i << "] = " << v[i] << '\n';
  std::cout << "const m[" << j << "][" << k << "] = " << m[j][k] << '\n';
  std::cout << "const m(" << j << ',' << k << ") = " << m(j,k) << '\n';
  std::cout << "v.NumEntries() = " << v.NumEntries() << '\n';
  std::cout << "m.NumEntries() = " << m.NumEntries() << '\n';
}

#endif
