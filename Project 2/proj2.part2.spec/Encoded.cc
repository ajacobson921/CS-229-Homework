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
#include "Encoded.hh"

class Encoded        // a class for an object holding compression information.
 { private:
    Direct &origin;  // reference to a Direct object called an original sequence
    char *subinsertion; // an array holding a concatenated sequence of parts in subs and insertions
    int  subinsertlen;  // the length of the concatenated sequence
    struct Edit *operation; // an array holding a number of edit operations
    int  editnum;       // the number of edit operations
    string dname;	// the name of the derived sequence
    int   dlength;      // the length of the drived sequence

   public:
    Encoded(Alignment &obj); // a normal constructor
    ~Encoded();              // destructor
    int getEditNum() const;  // returns editnum.
    struct Edit* getOperation() const; // returns operation.
    int getSubInsertLen() const; // returns subinsertlen.
    char* getSubInsertion() const; // return subinsertion.
    int getDLength() const; // return dlength.
    string getDName() const; // return dname.
    Direct& getOrigin() const; // return origin.
    string toString() const; // generates a string form of its contents.
    char* getDSeq() const;   // deirves a sequence and turns it in a char array.
 };

Encoded:: Encoded(Alignment &obj)
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
  &origin = obj.getOrigin;
  editnum = obj.getEditNum();
  subinsertlen = obj.getSubInsertLen();
  
  int i = j = 1;
  int opind = 0;
  int snind = 0;
  for(int ind = 0; i < obj.getAlignLen(); )
  {
    char top = getRow('T')[ind];
    char mid = getRow('M')[ind];
    char bot = getRow('B')[ind];

    //same
    if(top == bot  && mid == '|')
    {
      i++;
      j++;
      ind++;
      continue;
    }

    //substitution
    if(top != bot && mid = ' ')
    {
      operation[opind].position = i;
      operation[opind++].indel = 0;
      subinsertion[snind++] = bot[ind];
      i++; 
      j++; 
      ind++; 
      continue;
    }

    //deletion
    int gaplen = 1;
    int q = ind;
    while(mid[q++] == '-')
    {
      gaplen++;
      q++
    }
    operation[opind].position = i;
    if(bot == ' ' && mid = '-')
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
        subinsertion[snind++] = bot[ind++];
      }
    }

  }


}

Encoded:: ~Encoded()
// Frees heap memory.
{
  delete origin;
  delete[] subinsertion;
  delete subinsertlen;
  delete operation.position;
  delete operation.indel;
  delete operation;
  delete editnum;
  delete dname;
  delete dlength;
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
  for(int snind = opind = 0; opind < editnum; opind++)
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
 return &origin;
}

int Encoded:: getEditNum() const
// Returns editnum.
{
  return editnum;
}

struct Edit* Encoded:: getOperation() const 
// Returns operation.
{
 return *operation;
}

int Encoded:: getSubInsertLen() const
// Returns subinsertlen.
{
  return subinsertlen;
}

char* Encoded:: getSubInsertion() const
// Returns subinsertion.
{
  return *getSubInsertion;
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
  cout << "Name of the sequence: " + dname;
  cout << "Length of the sequence: "  + dlength;
  cout << "Number of edits: " + editnum;
  cout << "Length of subsititutions and insertions: "+ subinsertlen;
  cout << "Concatenation of subs and inserts: " + getSubInsertion();
  int subin = 0;
  for(int h = 0; h < editnum; h++)
  {
    if(operation[h].indel < 0)
    {
      cout << "Operation " + h + ":: Position: " +  operation[h].position + " Indel: " + operation[h].indel + " Deletion "; 
    }
    else if(operation[h] == 0)
    {
      cout << "Operation " + h + ":: Position: " +  operation[h].position + " Indel: " + operation[h].indel + " Substitution: " + subinsertion[subin];
      subin++: 
    }
    else
    {
      cout << "Operation " + h + ":: Position: " +  operation[h].position + " Indel: " + operation[h].indel + " Insertion: " + subinsertion[subin];
      subin++:
    } 
  }
}
