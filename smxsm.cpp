/*
    smxsm.cpp
    03/19/14
	  
		Project 9:
		SpareMatrix * SparseMatrix
		Driver program for the executable smxsm.x
*/

#include <iostream>
#include <iomanip>
#include <fstream>

#include <vector.h>
#include <matrix.h>
#include <matrix_util.h>
#include <sparse.h>
#include <sparse_util.h>

// in lieu of makefile
#include <hashfunctions.cpp>
#include <bitvect.cpp>
#include <xstring.cpp>
#include <primes.cpp>
// */

int main ( int argc , char* argv[] )
{
  fsu::SparseMatrix<double> a, b, c;
  fsu::String filename;
  if (argc < 3)
  {
    do
    {
      std::cout << " Enter file for sparse matrix A: ";
      std::cin >> filename;
    } while (!Load(a,filename.Cstr()));
    do
    {
      std::cout << " Enter file for sparse matrix B: ";
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
  std::cout << "B as loaded:\n";
  Display(b,2);

  c = a * b;
  std::cout << "Product A*B:\n";
  Display(c,2);

  /* // converts to regular matrix product and back
  fsu::Matrix<double> A, B, C;
  Convert(A,a);
  std::cout << "A as converted to Matrix:\n";
  Display(A,2);

  Convert(B,b);
  std::cout << "B as converted to Matrix:\n";
  Display(B,2);

  C = B * C;
  Convert(d,C);

  // std::cout << "Product equation:\n";
  // DisplayProduct(a,b,2);

  std::cout << "Product A*B:\n";
  Display(c,2);
  std::cout << "Check   A*B:\n";
  Display(d,2);
  // */

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
