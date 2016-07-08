/*
    mxm.cpp
    03/19/14
	  
		Project 9:
		Matrix * matrix
*/

#include <iostream>
#include <iomanip>
#include <fstream>

#include <vector.h>
#include <matrix.h>
#include <matrix_util.h>

// in lieu of makefile
#include <xstring.cpp>
// */

int main ( int argc , char* argv[] )
{
  fsu::Matrix<double> a(0,0), b(0,0), c(0,0);
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
      std::cout << " Enter file for matrix B: ";
      std::cin >> filename;
    } while (!Load(b,filename.Cstr()));
  }
  else
  {
    Load (a,argv[1]);
    Load (b,argv[2]);
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

  if (b.NumberOfRows() < 101 && b.NumberOfCols() < 81)
  {
    std::cout << "B as loaded:\n";
    Display(b,2);
  }
  else
  {
    std::cout << "B is " << b.NumberOfRows() << " x " << b.NumberOfCols() << '\n';
  }
  c = a * b;
  if (c.NumberOfRows() < 101 && c.NumberOfCols() < 51)
  {
    
    std::cout << "Product equation:\n";
    DisplayProduct(a,b,2);

    std::cout << "Product A*B:\n";
    Display(c,2);
  }
  else
  {
    std::cout << "A*B is " << c.NumberOfRows() << " x " << c.NumberOfCols() << '\n';
  }

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
