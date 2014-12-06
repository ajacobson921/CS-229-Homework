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

// Prints out an error message and stops.
void fatal(const char *msg)
{
     cerr << "Error message: " << string(msg) << endl;
     exit(1); // stops unexpectedly.
}

// Prints out an error message and stops.
void fatal(const char *msg, const char *name) 
{
     cerr << "Error message: " << string(msg) << string(name) << endl;
     exit(1); // stops unexpectedly.
}

// Opens an input file stream and checks for success.
void ckopeninf(ifstream &infile, const char *fname)
{
	infile.open(fname);
	if ( infile.fail() )
	  fatal("ckopeninf: cannot open input file ", fname);
}

