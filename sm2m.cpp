/*
    sm2m.cpp
    03/19/14
	  
		Project 9:
		Converts sparse matrix file to matrix file
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
#include <xstring.cpp>
#include <primes.cpp>
#include <bitvect.cpp>
#include <hashfunctions.cpp>
// */

int main ( int argc , char* argv[] )
{
  fsu::SparseMatrix<double> a;
  fsu::String filename;
  if (argc < 2)
  {
    do
    {
      std::cout << " Enter file for sparse matrix A: ";
      std::cin >> filename;
    } while (!Load(a,filename.Cstr()));
  }
  else
  {
    Load (a,argv[1]);
  }

  std::cout << "A as loaded:\n";
  Display(a,2);

  fsu::Matrix<double> A;
  Convert(A,a);

  if (A.NumberOfRows() < 101 && A.NumberOfCols() < 81)
  {
    std::cout << "A as converted:\n";
    Display(A,2);
  }
  else
  {
    std::cout << "conversion is " << A.NumberOfRows() << " x " << A.NumberOfCols() << '\n';
  }

  if (argc < 3)
  {
    do
    {
      std::cout << " Enter file to save conversion: ";
      std::cin >> filename;
    } while (!Save(A,filename.Cstr()));
    std::cout << "Matrix saved to file " << filename << '\n';
  }
  else
  {
    Save (A,argv[2]);
    std::cout << "Matrix saved to file " << argv[2] << '\n';
  }
}
