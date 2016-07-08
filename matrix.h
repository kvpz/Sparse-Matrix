/*
    matrix.h
    03/22/2014
    Chris Lacher

    class template Matrix<T> built on top of Vector<T>
    includes global multiplication operators Matrix*Matrix and Matrix*Vector

    Copyright 2014, R.C. Lacher
*/

#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector.h>
// #include <debug.h>

namespace fsu
{

  //------------------
  //   class Matrix
  //------------------

  template <typename T>
  class Matrix
  {
  public:
    typedef T ValueType;
    typedef typename fsu::Vector<T>::Iterator RowIterator;
    typedef typename fsu::Vector<T>::ConstIterator ConstRowIterator;

    // these are used by graph algorithms when using adjacency matrix rep
    RowIterator Begin (size_t row) { return row_[row].Begin(); }
    RowIterator End   (size_t row) { return row_[row].End(); }
    ConstRowIterator Begin (size_t row) const { return row_[row].Begin(); }
    ConstRowIterator End   (size_t row) const { return row_[row].End(); }

    Matrix ();
    Matrix (size_t numrows, size_t numcols);
    Matrix (size_t numrows, size_t numcols, T t);
    Matrix (const Matrix& m);
    virtual ~Matrix();
    Matrix<T>& operator=  (const Matrix<T>& m);

    Vector <T>& operator[]        (size_t rowindex);
    const Vector <T>& operator[]  (size_t rowindex) const;
    ValueType& operator()         (size_t i, size_t j);
    const ValueType& operator()   (size_t i, size_t j) const;

    void SetSize (size_t numrows, size_t numcols);
    void SetSize (size_t numrows, size_t numcols, T t);
    size_t NumberOfRows () const;
    size_t NumberOfCols () const;

    void Clear();

  protected:
    fsu::Vector < fsu::Vector < T > > row_;
  } ;  // class Matrix <T>

  template <typename T>
  T& Matrix<T>::operator () (size_t i, size_t j)
  {
    // std::cerr << " call             : (" << i << ',' << j << ")\n";
    // std::cerr << " dimensions before: (" << NumberOfRows() << ',' << NumberOfCols() << ")\n";
    bool resize = 0;
    size_t rows= NumberOfRows(), cols = NumberOfCols();
    if (i >= rows)
    {
      rows = i + 1;
      resize = 1;
    }
    if (j >= cols)
    {
      cols = j + 1;
      resize = 1;
    }
    if (resize)
      SetSize(rows,cols,T());
    // std::cerr << " dimensions after : (" << NumberOfRows() << ',' << NumberOfCols() << ")\n";
    return (*this)[i][j];
  }

  template <typename T>
  const T& Matrix<T>::operator () (size_t i, size_t j) const
  {
    return (*this)[i][j];
  }

  template <typename T>
  Matrix<T>::Matrix () :  row_() // uses default vector size
  {
    // fsu::Debug ("Matrix::Matrix()");
  }

  template <typename T>
  Matrix<T>::Matrix (size_t numrows, size_t numcols) :  row_(numrows)
  {
    // fsu::Debug ("Matrix::Matrix(r,c)");
    for (size_t i = 0; i < numrows; ++i)
      row_[i].SetSize(numcols,T());
  }

  template <typename T>
  Matrix<T>::Matrix (size_t numrows, size_t numcols, T t) :  row_(numrows)
  {
    // fsu::Debug ("Matrix::Matrix(r,c,t)");
    for (size_t i = 0; i < numrows; ++i)
      row_[i].SetSize(numcols,t);
  }

  template <typename T>
  Matrix<T>::Matrix (const Matrix<T>& m) :  row_(m.row_)
  {}

  template <typename T>
  Matrix<T>::~Matrix ()
  {
    Clear();
  }

  template <typename T>
  Matrix<T>& Matrix<T>::operator= (const Matrix<T>& m)
  {
    if (this != &m)
    {
      row_ = m.row_;
    }
    return *this;
  }

  template <typename T>
  void Matrix<T>::Clear()
  {
    for (size_t i = 0; i < row_.Size(); ++i)
      row_[i].Clear();
    row_.Clear();
  }

  template <typename T>
  void Matrix<T>::SetSize (size_t numrows, size_t numcols, T t)
  {
    row_.SetSize(numrows);
    for (size_t i = 0; i < numrows; ++i)
      row_[i].SetSize(numcols, t);
  }

  template <typename T>
  void Matrix<T>::SetSize (size_t numrows, size_t numcols)
  {
    row_.SetSize(numrows);
    if (numrows > 0)
      for (size_t i = 0; i < numrows; ++i)
        row_[i].SetSize(numcols);
  }

  template <typename T>
  Vector <T>& Matrix<T>::operator [] (size_t rowindex) 
  {
    return row_ [rowindex];
  }

  template <typename T>
  const Vector <T>& Matrix<T>::operator [] (size_t rowindex) const
  {
    return row_ [rowindex];
  }

  template <typename T>
  size_t Matrix<T>::NumberOfRows () const
  {
    return row_.Size();
  }

  template <typename T>
  size_t Matrix<T>::NumberOfCols () const
  {
    if (0 == row_.Size()) return 0;
    return row_[0].Size();
  }

} // namespace fsu

//-------------------------------------
//   global operators on Matrix, Vector
//-------------------------------------

template < typename N >
fsu::Vector<N> operator* (const fsu::Matrix<N>& A, const fsu::Vector<N>& V)
{
  fsu::Vector <N> X(0);
  size_t J = A.NumberOfCols();
  if (J != V.Size())
  {
    std::cerr << " ** M*V Multiplication Error: rows of M and V not same size\n";
    return X;
  }
  size_t I = A.NumberOfRows();
  X.SetSize(I,N());
  for (size_t i = 0; i < I; ++i)
  {
    for (size_t j = 0; j < J; ++j)
    {
      X[i] += A(i,j)*V[j];
    }
  }
  return X;
}

template < typename N >
fsu::Matrix<N> operator* (const fsu::Matrix<N>& A, const fsu::Matrix<N>& B)
{
  fsu::Matrix <N> X(0,0);
  size_t K = A.NumberOfCols();
  if (K != B.NumberOfRows())
  {
    std::cerr << " ** A*B Multiplication Error: rows of A and cols of B not same size\n";
    return X;
  }
  size_t I = A.NumberOfRows(), J = B.NumberOfCols();
  X.SetSize(I,J,N());
  for (size_t i = 0; i < I; ++i) // row
  {
    for (size_t j = 0; j < J; ++j) // col
    {
      for (size_t k = 0; k < K; ++k) // x(i,j) = (row i of A) dot (col j of B)
      {
        X(i,j) += A(i,k)*B(k,j);
      }
    }
    std::cout << '.' << std::flush;
  }
  std::cout << '\n';
  return X;
}

#endif
