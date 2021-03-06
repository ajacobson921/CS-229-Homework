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
#include "Encoded.hh"

Encoded:: Encoded(Alignment &obj): origin(obj.getOrigin())
// Input parameter obj is a reference to an Alignment object holding an optimal
// alignment along with sequence and other information.

// The constructor initializes some members by calling public functions
// of Alignment from the object obj. Among these members are
// origin, the reference to the original sequence,
// editnum, the size of the array operation, and
// subinsertlen, the size of the array subinsertion.
// the name and length of the derived sequence.
// The constructor allocates memory to operation and subinsertion.
// Then it constructs the two arrays operation and subinsertion
// by follwing the method in Figure 2 of the project descrImation.
// The construction uses top & mid & bot, the three rows of the optimal alignment,
// which are provided by the public functions of Alignment from the object obj.
{
  editnum = obj.getEditNum();
  subinsertlen = obj.getSubInsertLen();
  
  int i = 1;
  int j = 1;
  int opind = 0;
  int snind = 0;
  for(int ind = 0; i < obj.getAlignLen();)
  {
    char top = obj.getRow('T')[ind];
    char mid = obj.getRow('M')[ind];
    char bot = obj.getRow('B')[ind];

    //same
    if(top == bot  && mid == '|')
    {
      i++;
      j++;
      ind++;
      continue;
    }

    //substitution
    if(top != bot && mid == ' ')
    {
      operation[opind].position = i;
      operation[opind++].indel = 0;
      subinsertion[snind++] = bot;
      i++; 
      j++; 
      ind++; 
      continue;
    }

    //deletion
    int gaplen = 1;
    int q = ind;
    while(obj.getRow('M')[q++] == '-')
    {
      gaplen++;
      q++;
    }
    operation[opind].position = i;
    if(bot == ' ' && mid == '-')
    {
      i += gaplen;
      operation[opind++].indel = -gaplen;
      ind += gaplen;
    }
    //insertion
    else
    {
      j+= gaplen;
      operation[opind++].indel = gaplen;
      for (; gaplen; gaplen--)
      {
        subinsertion[snind++] = obj.getRow('B')[ind++];
      }
    }

  }


}

Encoded:: ~Encoded()
// Frees heap memory.
{
  delete[] subinsertion;
  delete[] operation;

}

char* Encoded:: getDSeq() const
// Derives a sequence of length dlength and returns it in a char array
// of length dlength + 2. The sequence is in the array from index 1
// to index dlength, with the element at index 0 set to ' '
// and that at index dlength + 1 to '\0'.
// The function allocates memory to the array and generates
// the sequence B by using the method in Figure 3 of the project descrImation.
// The information on the sequence A is obtained by calling the public
// functions of Direct from the object reference origin.
{
  char *A = getOrigin().getSeq();
  
  char B[dlength + 2];
  B[dlength + 1] = '\0';


  int i = 1;
  int j = 1;
  for(int snind = 0, opind = 0; opind < editnum; opind++)
  {
 	int oppos = operation[opind].position;
 	int indel = operation[opind].indel;

 	if(i < oppos)
 		for(int k = 0; k < oppos-1-i; k++)
 		{
 			B[j+k] = A[i+k];
 		}
 		i = oppos;
 		j =+ oppos - i;
 	}
  return B;
}

Direct& Encoded:: getOrigin() const
// Returns origin.
{
 return origin;
}

int Encoded:: getEditNum() const
// Returns editnum.
{
  return editnum;
}

struct Edit* Encoded:: getOperation() const 
// Returns operation.
{
 return operation;
}

int Encoded:: getSubInsertLen() const
// Returns subinsertlen.
{
  return subinsertlen;
}

char* Encoded:: getSubInsertion() const
// Returns subinsertion.
{
  return subinsertion;
}

int Encoded:: getDLength() const
// Returns dlength.
{
  return dlength;
}

string Encoded:: getDName() const
// Returns dname.
{
  return dname;
}

string Encoded:: toString() const
// Generates and returns a string form of its contents.
// The information includes the following lines:
// Name of the encoded sequence: ...
// Length of the encoded sequence: ...
// Number of edit operations: ...
// Length of substitutions and insertions: ...
// Concatenation of subs and inserts in order: ...
//
// and each of the operations in the array operation.
{
  cout << "Name of the sequence: " << dname <<endl;
  cout << "Length of the sequence: "  << dlength <<endl;
  cout << "Number of edits: " << editnum <<endl;
  cout << "Length of subsititutions and insertions: "<<  getSubInsertLen() <<endl;
  cout << "Concatenation of subs and inserts: " << getSubInsertion() <<endl;
  int subin = 0;
  for(int h = 0; h < editnum; h++)
  {
    if(operation[h].indel < 0)
    {
      cout << "Operation " << h << ":: Position: " << operation[h].position << " Indel: " << operation[h].indel << " Deletion "; 
    }
    else if(operation[h].indel == 0)
    {
      cout << "Operation " << h << ":: Position: " <<  operation[h].position << " Indel: " << operation[h].indel << " Substitution: " << subinsertion[subin];
      subin++; 
    }
    else
    {
      cout << "Operation " << h << ":: Position: " << operation[h].position << " Indel: " << operation[h].indel << " Insertion: " << subinsertion[subin];
      subin++;
    } 
  }
}
bool Encoded::  operator<=(Encoded &rightobj) const
{
   int getright = rightobj.getNumDiff();
   if(getNumDiff() < getright)
   {
    return true;
   }
   if(getNumDiff() == getright)
   {
      if(subinsertlen <= rightobj.getSubInsertLen())
      {
        return true;
      }
   }
   return false;
}

int Encoded :: getNumDiff() const
{
  int num = 0;
  struct Edit *op = getOperation();
  for(int i = 0; i < getEditNum(); i ++)
  {
    int toAdd = op[i].indel;
    if(toAdd < 0)
    {
      toAdd = 0 - op[i].indel;
    }
    num += toAdd;
  }
  return num;
}

Compressed:: Compressed(Alignment &obj) : Encoded(obj)
{
}

bool Compressed :: operator<=(Encoded &rightobj) const
{
   int getright = rightobj.getNumDiff();
   if(getNumDiff() < getright)
   {
    return true;
   }
   if(getNumDiff() == getright)
   {
      if(subinsertlen < rightobj.getSubInsertLen())
      {
        return true;
      }
      else if(subinsertlen == rightobj.getSubInsertLen())
      {
        string left(subinsertion);
        string right(rightobj.getSubInsertion());
        if(left <= right)
        {
          return true;
        }
      }
   }
   return false;
}
