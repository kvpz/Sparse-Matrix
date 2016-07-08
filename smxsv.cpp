/*
    smxsv.cpp
    03/19/14
	  
		Project 9:
		SparseMatrix * SparseVector
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
  fsu::SparseMatrix<double> a(100);
  fsu::SparseVector<double> b(100), c(100);
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
      std::cout << " Enter file for sparse vector V: ";
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
  fsu::Matrix<double> A;
  fsu::Vector<double> B;
  Convert(A,a);


  std::cout << "A as converted to Matrix:\n";
  Display(A,2);

  Convert(B,b);
  std::cout << "V as converted to Vector:\n";
  Display(B,2);
  */

  c = a * b;

  // std::cout << "Product equation:\n";
  // DisplayProduct(a,b,2);

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
