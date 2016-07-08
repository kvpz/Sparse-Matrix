/*
    m2sm.cpp
    03/19/14
		
		Project 9:
	  converts matrix file to sparse matrix file
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
  fsu::Matrix<double> a(0,0);
  fsu::String filename;
  if (argc < 2)
  {
    do
    {
      std::cout << " Enter file for matrix A: ";
      std::cin >> filename;
    } while (!Load(a,filename.Cstr()));
  }
  else
  {
    Load (a,argv[1]);
  }

  if (a.NumberOfRows() < 101 && a.NumberOfCols() < 81)
  {
    std::cout << "A as loaded:\n";
    Display(a,2);
  }
  else
  {
    std::cout << "A is " << a.NumberOfRows() << " x " << a.NumberOfCols() << '\n';
  }

  fsu::SparseMatrix<double> A;
  Convert(A,a);

  std::cout << "A as converted to SparseMatrix:\n";
  Display(A,2);

  if (argc < 3)
  {
    do
    {
      std::cout << " Enter file to save SparseMatrix: ";
      std::cin >> filename;
    } while (!Save(A,filename.Cstr()));
    std::cout << "SparseMatrix saved to file " << filename << '\n';
  }
  else
  {
    Save (A,argv[2]);
    std::cout << "SparseMatrix saved to file " << argv[2] << '\n';
  }
}
