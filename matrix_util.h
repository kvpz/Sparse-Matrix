/*
    matrix_util.h
    03/19/14

*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector.h>
#include <matrix.h>

#ifndef _MATRIX_UTIL_H
#define _MATRIX_UTIL_H

template <typename T>
void Display (const fsu::Vector<T>& v, int cw, std::ostream& os = std::cout)
{
  for (size_t j = 0; j < v.Size(); ++j)
  {
    os << std::setw(cw) << v[j];
  }
  std::cout << '\n';
}

template <typename T>
void Display (const fsu::Matrix<T>& m, int cw, std::ostream& os = std::cout)
{
  for (size_t i = 0; i < m.NumberOfRows(); ++i)
  {
    for (size_t j = 0; j < m.NumberOfCols(); ++j)
    {
      os << std::setw(cw) << m[i][j];
    }
    std::cout << '\n';
  }
}

template <typename N>
void DisplayProduct(const fsu::Matrix<N>& a, const fsu::Vector<N>& b, int cw)
{
  fsu::Vector <N> c = a*b;
  size_t I = a.NumberOfRows(), J = b.Size(), K = c.Size();
  size_t M = I; if (M < J) M = J; if (M < K) M = K;  // M = max no of rows
  size_t mid = (I-1) >> 1; // (I-1)/2;

  std::cout << '\n';
  for (size_t i = 0, j = 0; i < M; ++i)
  {
    // row i of A
    if (i < I)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';
    for (j = 0; j < a.NumberOfCols(); ++j)
    {
      if (i < I)
        std::cout << std::setw(cw) << a[i][j];
      else
        std::cout << std::setw(cw) << ' ';
    }
    if (i < I)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';

    // operator symbol
    if (i == mid) std::cout << std::setw(cw) << '*';
    else          std::cout << std::setw(cw) << ' ';

    // entry i of V
    if (i < J)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';
    if (i < J)
      std::cout << std::setw(cw) << b[i];
    else
      std::cout << std::setw(cw) << ' ';
    if (i < J)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';

    // equals symbol
    if (i == mid) std::cout << std::setw(cw) << '=';
    else          std::cout << std::setw(cw) << ' ';

    // entry i of C
    if (i < K)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';
    if (i < K)
      std::cout << std::setw(cw) << c[i];
    else
      std::cout << std::setw(cw) << ' ';
    if (i < K)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';
    std::cout << '\n';
  }
  std::cout << '\n';
}

template <typename N>
void DisplayProduct(const fsu::Matrix<N>& a, const fsu::Matrix<N>& b, int cw)
{
  fsu::Matrix <N> c = a*b;
  size_t I = a.NumberOfRows(), J = b.NumberOfRows(), K = c.NumberOfRows();
  size_t M = I; if (M < J) M = J; if (M < K) M = K;  // M = max no of rows
  size_t mid = (I-1) >> 1; // (I-1)/2;

  std::cout << '\n';
  for (size_t i = 0, j = 0; i < M; ++i)
  {
    // row i of A
    if (i < I)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';
    for (j = 0; j < a.NumberOfCols(); ++j)
    {
      if (i < I)
        std::cout << std::setw(cw) << a[i][j];
      else
        std::cout << std::setw(cw) << ' ';
    }
    if (i < I)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';

    // operator symbol
    if (i == mid) std::cout << std::setw(cw) << '*';
    else          std::cout << std::setw(cw) << ' ';

    // row i of B
    if (i < J)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';
    for (j = 0; j < b.NumberOfCols(); ++j)
    {
      if (i < J)
        std::cout << std::setw(cw) << b[i][j];
      else
        std::cout << std::setw(cw) << ' ';
    }
    if (i < J)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';

    // equals symbol
    if (i == mid) std::cout << std::setw(cw) << '=';
    else          std::cout << std::setw(cw) << ' ';

    // row i of C
    if (i < K)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';
    for (j = 0; j < c.NumberOfCols(); ++j)
    {
      if (i < K)
        std::cout << std::setw(cw) << c[i][j];
      else
        std::cout << std::setw(cw) << ' ';
    }
    if (i < K)
      std::cout << std::setw(cw) << '|';
    else
      std::cout << std::setw(cw) << ' ';
    std::cout << '\n';
  }
  std::cout << '\n';
}

template < typename N >
bool Load (fsu::Matrix<N>& m, const char* filename)
{
  std::ifstream ifs(filename);
  if (ifs.fail())
  {
    std::cerr << " ** Load Failure: Unable to open file " << filename << '\n';
    return 0;
  }
  size_t rows, cols;
  ifs >> rows >> cols;
  m.SetSize(rows,cols);
  for (size_t i = 0; i < rows && !ifs.fail(); ++i)
  {
    for (size_t j = 0; j < cols && !ifs.fail(); ++j)
      ifs >> m[i][j];
  }
  if (ifs.fail())
  {
    std::cerr << " ** Load Failure: data read failure in file " << filename << '\n';
    m.SetSize(0,0);
    ifs.clear();
    ifs.close();
    return 0; 
  }
  ifs.close();
  return 1;
}

template < typename N >
bool Save (const fsu::Matrix<N>& m, const char* filename)
{
  std::ofstream ofs(filename);
  if (ofs.fail())
  {
    std::cerr << " ** Save Failure: Unable to open file " << filename << '\n';
    return 0;
  }
  size_t rows = m.NumberOfRows(), cols = m.NumberOfCols();
  ofs << rows << ' ' << cols << '\n';
  for (size_t i = 0; i < rows && !ofs.fail(); ++i)
  {
    for (size_t j = 0; j < cols - 1 && !ofs.fail(); ++j)
      ofs << m[i][j] << ' ';
    if (!ofs.fail()) ofs << m[i][cols - 1] << '\n';
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
}

template < typename N >
bool Load(fsu::Vector<N>& v, const char* filename)
{
  std::ifstream ifs(filename);
  if (ifs.fail())
  {
    std::cerr << " ** Load Failure: Unable to open file " << filename << '\n';
    return 0;
  }
  size_t size;
  ifs >> size;
  v.SetSize(size);
  for (size_t j = 0; j < size && !ifs.fail(); ++j)
    ifs >> v[j];
  if (ifs.fail())
  {
    std::cerr << " ** Load Failure: data read failure in file " << filename << '\n';
    v.SetSize(0,0);
    ifs.clear();
    ifs.close();
    return 0; 
  }
  ifs.close();
  return 1;
}

template < typename N >
bool Save (const fsu::Vector<N>& v, const char* filename)
{
  std::ofstream ofs(filename);
  if (ofs.fail())
  {
    std::cerr << " ** Save Failure: Unable to open file " << filename << '\n';
    return 0;
  }
  size_t size = v.Size();
  ofs << size << '\n';
  for (size_t j = 0; j < size - 1 && !ofs.fail(); ++j)
    ofs << v[j] << ' ';
  if (!ofs.fail()) ofs << v[size - 1] << '\n';
  if (ofs.fail())
  {
    std::cerr << " ** Save Failure: data write failure in file " << filename << '\n';
    ofs.clear();
    ofs.close();
    return 0; 
  }
  ofs.close();
  return 1;
}

#endif
