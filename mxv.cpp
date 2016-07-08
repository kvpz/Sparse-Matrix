/*
    mxv.cpp
    03/19/14
		
		Project 9:
		Matrix * Vector
*/

#include <iostream>
#include <iomanip>
#include <fstream>

#include <vector.h>
#include <matrix.h>
#include <matrix_util.h>

// #include <sparse.h>
// #include <sparse_util.h>

// in lieu of makefile
#include <xstring.cpp>
#include <primes.cpp>
#include <bitvect.cpp>
#include <hashfunctions.cpp>
// */

int main ( int argc , char* argv[] )
{
  fsu::Matrix<double> a(0,0);
  fsu::Vector<double> b(0), c(0);
  fsu::String filename;
  if (argc < 3)
  {
    do
    {
      std::cout << " Enter file for matrix A: ";
      std::cin >> filename;
    } while (!Load(a,filename.Cstr()));
    do
    {
      std::cout << " Enter file for vector V: ";
      std::cin >> filename;
    } while (!Load(b,filename.Cstr()));
  }
  else
  {
    Load (a,argv[1]);
    Load (b,argv[2]);
  }

  std::cout << "A as loaded:\n";
  Display(a,2);
  std::cout << "V as loaded:\n";
  Display(b,2);

  /*
  fsu::SparseMatrix<double> A;
  fsu::SparseVector<double> B;
  Convert(A,a);
  Convert(B,b);

  std::cout << "A as converted to SparseMatrix:\n";
  Display(A,2);
  std::cout << "V as converted to SparseVector:\n";
  Display(B,2);
  */

  c = a * b;

  std::cout << "Product equation:\n";
  DisplayProduct(a,b,2);

  std::cout << "Product A*V:\n";
  Display(c,2);

  if (argc < 4)
  {
    do
    {
      std::cout << " Enter file to save product: ";
      std::cin >> filename;
    } while (!Save(c,filename.Cstr()));
    std::cout << "Product saved in file " << filename << '\n';
  }
  else
  {
    Save (c,argv[3]);
    std::cout << "Product saved in file " << argv[3] << '\n';
  }
}
