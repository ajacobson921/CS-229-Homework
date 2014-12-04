// @author Aaron Jacobson ajacob1

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
#include "Direct.hh"

class Direct   // a class for an object holding a sequence
 { private:
    string name;   // the name of the sequence
    int    length; // the length of the sequence
    char   *seq;   // an array holding the sequence from index 1 to index length
                   // with seq[0] set to ' ' and seq[length+1] set to '\0'.
		   // The length of seq is length + 2.

   public:
    Direct(const char *fname); // normal constructor
    Direct(string &stag, int slen, char *sarr); // an alternative constructor
    Direct(const Direct &obj); // copy constructor
    Direct();                  // default constructor
    ~Direct();                 // destructor
    string getName() const;    // returns name.
    int    getLength() const;  // returns length.
    char*  getSeq() const;     // returns seq.
 };

Direct:: Direct(const char *fname)
// Input parameter fname is a pointer to a string for storing the name of a DNA sequence file.
// A Direct object is constructed to save the name,
// length and DNA letters of the sequence in the file.
// A proper amount of memory needs to be allocated for the seq member.

// This constructor opens the file whose name is pointed to by fname,
// reads the name, length and DNA letters of the sequence in the file,
// dynamically allocates a proper amount of memory for the seq member
// and saves the data into the member variables.
// The constructor prints out a proper error message if fname is NULL,
// no file exists with the given name.
{
  ifstream in;
  string line;
  string toAdd; 
  ckopeninf(&in, fname);
  if(in.is_open())
  {
    if(getline(in, name))
    {
      name << '\n';
    }
    if(getline(in, seq))
    {
      while(!in.eof())
      {
        while(getline(in, toAdd))
        {
          toAdd << '\n';
          seq += toAdd;
          toAdd = NULL;
        }
      }
    }
  }
  length = seq.getLength();
}

Direct:: Direct(string &stag, int slen, char *sarr)
// Input parameter stag is a reference to a string object as the name of a sequence.
// Input parameter slen is the length of the sequence.
// Input parameter sarr is a pointer to a char array with the sequence
// saved at index 1 through index slen.

// If slen is negative or sarr is NULL, prints out an error message with fatal. 
// A copy of the string object is saved in the member name.
// The value slen is saved in the member length.
// Memory is allocated to the member seq and a copy of the sequence is saved in seq.
{
	if(slen < 0)
	{
		fatal(" slen is negative ")
	}
	if(sarr == NULL)
	{
		fatal("the seq array is null")
	}
	*name = &stag;
	length = slen;
	*seq = &sarr;
}

Direct:: Direct(const Direct &obj)
// A deep copy is made.
{ 
	name = obj.name;
	length = obj.length;
	seq = obj.seq;
}

Direct:: Direct()
// Sets seq to NULL.
{
  seq = NULL;
}

Direct:: ~Direct()
// Frees heap memory if seq is not NULL.
{
	if(seq != NULL)
	{
		delete[] seq;
	}
}

string Direct:: getName() const
// Returns the member name.
{
  return name;
}

int Direct:: getLength() const
// Returns the member length.
{
  return length;
}

char* Direct:: getSeq() const
// Returns the member seq.
{
 return *seq;
}
