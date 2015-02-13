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

#include <stdio.h>
#include <string.h>
#include <ctype.h>  // A library with functions for testing characters.
#include "wrapper.hh"
#include "Direct.hh"
#include "prototype.hh"
#include "Matrix.hh"
#include "Alignment.hh"

Alignment:: Alignment(Matrix &matobj, struct scoretp &param): origin(matobj.getOrigin()), derived(matobj.getDerived())
// Input parameter matobj is a reference to a Matrix object.
// Input parameter param is a reference to a struct scoretp variable with scoring parameters.

// The constructor initializes some member variables by calling public
// functions of Matrix from the object matobj.
// Then it allocates memory for each of the three char arrays,
// produces an optimal alignment by tracing through the matrices in the object matobj,
// and saves the alignment along with its score and length and editnum
// and subinsertlen in corresponding member variables.
// The alignment is represented by using three rows of characters (three char arrays).
// If the lengths of both sequences is zero, prints out an error message.

// Note that the length of an optimal alignment cannot exceed the sum of
// the lengths of the two DNA sequences.
{
  if(matobj.getOrigin().getLength() == 0 || matobj.getDerived().getLength() == 0)
  {
    fatal("Origin or derived is length 0");
  } 
  int m = matobj.getRowInd();
  int n = matobj.getColInd();

  char* arrtop = new char[m+n];
  char* arrmid = new char[m+n];
  char* arrbot = new char[m+n];
  


  int intlen = 0;
  editnum = subinsertlen = 0;
  int q = matobj.getMat('D')[m][n];
  int r = matobj.getMat('D')[m-1][n] - q;
  int p = 0;
  int i = 0;
  int j = 0;
  char mat = 'S';
  while(i <= m && j <= n)
  {
    if(mat == 'S')
    {
      if(i==m && j ==n)
      {
          break;
      }
      if(j==n || matobj.getMat('S')[i][j] == matobj.getMat('D')[i][j])
      {
          mat = 'D';
          editnum++;
          continue;
      }
      if(i == m || matobj.getMat('S')[i][j] == matobj.getMat('I')[i][j])
      {
          mat = 'I';
          editnum++;
          intlen = 0;
          continue;
      }
      arrtop[p] = origin.getSeq()[i];
      arrbot[p] = derived.getSeq()[j];
      if(origin.getSeq()[i] == derived.getSeq()[j])
      {
          arrmid[p] = '|';
      }
      else
      {
          arrmid[p] = ' ';
      }
      p++;
      i++;
      j++;
      continue;
    }
    if(mat == 'D')
    {
      arrtop[p] = ' ';
      arrmid[p] = '-';
      arrbot[p] = derived.getSeq()[j];
      if(i == (m-1) || matobj.getMat('D')[i][j] == (matobj.getMat('S')[i+1][j] - q - r))
      {
          mat = 'S';
      }
      i++;
      continue;
    }
    if(mat == 'I')
    {
      arrtop[p] = origin.getSeq()[i];
      arrmid[p] = '-';
      arrbot[p] = ' ';
      intlen++;
      if(j ==n || matobj.getMat('I')[i][j] == (matobj.getMat('S')[i][j+1] - q - r))
      {
          mat = 'S';
          subinsertlen += intlen;
      }
      j++;
      continue;
    }
  }
  top = arrtop;
  mid = arrmid;
  bot = arrbot;
  score = matobj.getMat('S')[0][0];
  alignlen = p;
}

// Frees heap memory.
Alignment::  ~Alignment() // destructor definition
{
  delete[] top;
  delete[] mid;
  delete[] bot;
}

Direct& Alignment:: getOrigin() const
// returns origin.
{
 return origin;
}

Direct& Alignment:: getDerived() const
// returns derived.
{
  return derived; 
}

int Alignment:: getScore() const
// returns alignment score.
{
  return score;
}

int Alignment:: getEditNum() const
// returns editnum.
{
  return editnum;
}

int Alignment:: getSubInsertLen() const
// Returns subinsertlen.
{
  return subinsertlen;
}

int Alignment:: getAlignLen() const
// Returns alignment length.
{
  return alignlen;
}

char* Alignment:: getRow(char kind) const
// Returns top if kind is 'T'.
// Returns mid if kind is 'M'.
// Returns bot if kind is 'B'.
// Prints out an error if kind has no expeced value.
{
  if(kind == 'T')
  {
    return top;
  }
  if(kind == 'M')
  {
    return mid;
  }
  if(kind == 'B')
  {
    return bot;
  }



}

string Alignment:: toString() const
// The function returns a summary of sequence and alignment information
// and the alignment in the form of string.
// The summary includes the name and length of each sequence,
// the score and length of the alignment. the number of edit operations,
// and the length of substitutions and insertions.
// The alignment is reported in sections of 70 characters, with each section consisting
// of three rows. The sequence positions of the first DNA letters in each section are
// reported in the left margin of 10 spaces.
{
  cout << "Sequence A: " << getOrigin().getName() << endl;
  cout << "Length : " << getOrigin().getLength() << endl;
  cout << "Sequence B: " << getDerived().getName() <<endl;
  cout << "Length: " << getDerived().getLength() <<endl;
  cout << "Alignment Score: " << getScore() <<endl;
  cout << "Length: " << getAlignLen() <<endl;
  cout << "Number of Edit Operations: " << getEditNum() <<endl;
  cout << "Number of Substitutions and Insertions: " << getSubInsertLen() << endl; 

  int charnum = 1;
  int length = alignlen;
  char *ptop = top;
  char *pmid = mid;
  char *pbot = bot;
  int tcount = 0;
  int mcount = 0;
  int bcount = 0;

  while(length > 0)
  {
      int j = 0;
      //Print dot structure with . for every 5 and : for every 10.
      while(j < 70)
      {
          printf("\t%i\t", charnum);
          if(charnum % 10 == 0)
          {
              printf("%c", ':');
              j++;
              charnum++;
          }
          else if((charnum % 5 == 0) && !(charnum % 10 == 0))
          {
              printf("%c", '.');
              j++;
              charnum++;
          }
          else
          {
              printf("%c\n", ' ');
              charnum++;
              j++;
          }
      }
      
      printf("\n\t\t");
      
      int k;
      if(length < 70)
      {
          k = length;
      }
      else
      {
          k = 70;
      }
      
      for(int l = 0; l < k; l++)
      {
          printf("%c\n", ptop[tcount]);
          tcount++;
      }
      
      printf("\n\t\t");
      
      for(int l = 0; l < k; l++)
      {
          printf("%c\n", pmid[mcount]);
          mcount++;
      }
      
      printf("\n\t\t");
      
      for(int k = 0; k < 70; k++)
      {
          printf("%c\n", pbot[bcount]);
          bcount++;
      }
      
      printf("\n");
      length = length - k;
      j = 0;
  }
}