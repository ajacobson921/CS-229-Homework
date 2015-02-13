//@author Aaron Jacobson ajacob1

#include <iostream> // provides objects like cin and cout for sending data
                    // to and from the standard streams input and output.
        // These objects are part of the std namespace.
#include <cstdlib>  // has exit etc.
#include <fstream>  // file streams and operations
#include <sstream>  // string streams and operations
using namespace std; // a container for a set of identifiers.
                     // Because of this line, there is no need to use std::cout
#include <typeinfo> // This header file is included for using typeid.
#include <stdexcept>
#include <climits>
#include <stdio.h>
#include <string.h>
#include <ctype.h>  // A library with functions for testing characters.
#include "wrapper.hh"
#include "prototype.hh"
#include "Direct.hh"
#include "Matrix.hh"

Matrix:: Matrix(Direct &seqone, Direct &seqtwo, struct scoretp &param)
// Input parameter seqone is a reference to a Direct object holding one DNA sequence.
// Input parameter seqtwo is a reference to a Direct object holding another DNA sequence.
// Input parameter param is a reference to a struct scoretp variable holding the scoring parameters.

// The constructor sets the member reference origin to seqone, and
// the member reference derived to seqtwo,
// through the initialization list before the constructor body.
//
// The constructor sets the member rowind to the return value from getLength() of seqone.
// The constructor sets the member colind to the return value from getLength() of seqtwo.
// If both rowind and colind are zero, then prints an error message.
// Then it allocates space for each
// matrix in the mattp structure by calling get2dspace(). Next it computes,
// in reverse order, the three matrices by the dynamic programming algorithm
// for aligning the two sequences pointed to by getLength() of seqone
// that of seqtwo. The sequence in seqone is treated as the sequence A.
: origin(seqone), derived(seqtwo)  // an initialization list.
{
  size_t s = strlen(origin.getSeq());
  if(s > INT_MAX)
  {
    fatal("The origin length is too big.");
  }
  rowind = (int) s;

  size_t t = strlen(derived.getSeq());
  if(t > INT_MAX)
  {
    fatal("The derived length is too big.");
  }
  colind = (int) t;

  if(rowind < 1 || colind < 1)
  {
    fatal("Can't make a matrix with 0 dimension");
  }
  Dmat = get2dspace(rowind+1, colind+1);
  Smat = get2dspace(rowind+1, colind+1);
  Imat = get2dspace(rowind+1, colind+1);
  int q = param.gopen;
  int r = param.gext;
  int m = rowind;
  int n = colind;


  Smat[m][n] = 0;
  Dmat[m][n] = (Smat[m][n]) - q;
  Imat[m][n] = (Smat[m][n]) - q;
  for(int j = n; j > 0; j--)
  {
      Imat[m][j] = Imat[m][j-1] - r;
      Smat[m][j] = Imat[m][j];
      Dmat[m][j] = Smat[m][j] - q;
  }
  for(int i = m; i > 0; i--)
  {
      Dmat[i][n] = Dmat[i+1][n] - r;
      Smat[i][n] = Dmat[i][n];
      Imat[i][n] = Smat[i][n] - q;
      for(int j = n; j > 0; j--)
      {
          Dmat[i][j] = max(Dmat[i+1][j] - r, Smat[i+1][j] - q - r);
          Imat[i][j] = max(Imat[i][j+1] - r, Smat[i][j+1] - q - r);
          Smat[i][j] = max(Smat[i+1][j+1] + sigma(origin.getSeq()[i], derived.getSeq()[i], 10, param.mismat), max(Dmat[i][j], Imat[i][j]));
      }
  }
}

// Frees heap memory.
Matrix:: ~Matrix()
{
  delete &origin;
  delete &derived;
  free2dspace(rowind, Dmat);
  free2dspace(rowind, Imat);
  free2dspace(rowind, Smat);
  delete Dmat;
  delete Imat;
  delete Smat;
}

int** Matrix:: get2dspace(int rowind, int colind)
// Allocates a 2-dimensional int array of (rowind + 1) by (colind + 1)
// and returns its memory address.
// Prints out an error message if rowind or colind is less than 0.
{
  if(rowind < 0 || colind <0)
  {
    fatal("Index cannot be zero.");
  }
  int** mat = new int*[rowind+1];
  for(int d = 0; d < rowind + 1; d++)
  {
    mat[d] = new int[colind+1];
  }
  return mat;
}

void Matrix:: free2dspace(int rowind, int **arr)
// Deallocates a 2-dimensional int array of first dimension 
// (rowind + 1) by releasing the memory for arr[j] for each j
// and then releasing the memory for arr.
// Prints out an error message if rowind is less than 0.
{
    if(rowind < 0)
    {
      fatal("Rowind less than 0");
    }
    for(int i = 0; i < rowind+1; i++)
    {
      delete[] arr[i];
    }
    delete arr;
}

Direct& Matrix:: getOrigin() const
// Returns origin.
{
  return origin;
}

Direct& Matrix:: getDerived() const
// Returns derived.
{
  return derived;
}

int Matrix:: getRowInd() const
// Returns rowind.
{
  return rowind;
}

int Matrix:: getColInd() const
// Returns colind.
{
  return colind;
}

int** Matrix:: getMat(char kind) const
// If kind is 'S', then returns Smat.
// If kind is 'D', then returns Dmat.
// If kind is 'I', then returns Imat.
// Otherwise, prints out an error message.
{
  if(kind == 'S')
  {
    return Smat;
  }
  else if(kind == 'D')
  {
    return Dmat;
  }
  else if(kind == 'I')
  {
    return Imat;
  }
  else
  {
    fatal("Not a kind of matrix");
  }
}

string Matrix:: toString(char kind) const
// Input parameter kind denotes matrix type: 'D', 'I', or 'S'.
// The function returns the matrix type and each value in the matrix
// in the form of a string.
// This function is for your own use to check on each matrix,
// so any format is OK.
{
  if(kind == 'D')
  {

  }
  else if(kind == 'S')
  {

  }
  else if(kind == 'I')
  {

  }
  else
  {

  }
}

int Matrix:: max(int a, int b)
{
    if( a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int Matrix:: sigma(int a, int b, int match, int penalty)
{
    if(a==b)
    {
        return match;
    }
    else
    {
        return penalty;
    }
}