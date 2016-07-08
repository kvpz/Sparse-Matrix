/*
    v2sv.cpp
    03/19/14

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
  fsu::Vector<double> a(0,0);
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

  if (a.Size() < 101)
  {
    std::cout << "A as loaded:\n";
    Display(a,2);
  }
  else
  {
    std::cout << "A has size " << a.Size() << '\n';
  }

  fsu::SparseVector<double> A;
  Convert(A,a);

  std::cout << "A as converted to SparseVector:\n";
  Display(A,2);

  if (argc < 3)
  {
    do
    {
      std::cout << " Enter file to save SparseVector: ";
      std::cin >> filename;
    } while (!Save(A,filename.Cstr()));
    std::cout << "SparseVector saved to file " << filename << '\n';
  }
  else
  {
    Save (A,argv[2]);
    std::cout << "SparseVector saved to file " << argv[2] << '\n';
  }
}
