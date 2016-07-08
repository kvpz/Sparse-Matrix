/*
    sm2m.cpp
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
  fsu::SparseVector<double> a;
  fsu::String filename;
  if (argc < 2)
  {
    do
    {
      std::cout << " Enter file for sparse vector A: ";
      std::cin >> filename;
    } while (!Load(a,filename.Cstr()));
  }
  else
  {
    Load (a,argv[1]);
  }

  std::cout << "A as loaded:\n";
  Display(a,2);

  fsu::Vector<double> A;
  Convert(A,a);

  if (A.Size() < 81)
  {
    std::cout << "A as converted:\n";
    Display(A,2);
  }
  else
  {
    std::cout << "A converted has size " << A.Size() << '\n';
  }

  if (argc < 3)
  {
    do
    {
      std::cout << " Enter file to save product: ";
      std::cin >> filename;
    } while (!Save(A,filename.Cstr()));
    std::cout << "Vector saved to file " << filename << '\n';
  }
  else
  {
    Save (A,argv[2]);
    std::cout << "Vector saved to file " << argv[2] << '\n';
  }
}
