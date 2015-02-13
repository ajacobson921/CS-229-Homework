//@author Aaron Jacobson @ajacob1

#include <iostream> // provides objects like cin and cout for sending data
                    // to and from the standard streams input and output.
		    // These objects are part of the std namespace.
#include <cstdlib>  // has exit etc
#include <typeinfo> // This header file is included for using typeid.
#include <fstream>  // file streams and operations
#include <sstream>  // string streams and operations
using namespace std; // a container for a set of identifiers.
                     // Because of this line, there is no need to use std::cout
#include <stdexcept> // exceptions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wrapper.hh"
#include "Direct.hh"
#include "prototype.hh"
#include "Matrix.hh"
#include "Alignment.hh"
#include "Encoded.hh"
#include "template.stack.min.hh"

int main(int argc, char *argv[])
{
 
  // Declares and initializes a struct scoretp variable named pararec.
  struct scoretp pararec
  {
    match = 10,
    mismat = -20,
    gopen = 40,
    gext = 2
  };
  // Declares and constructs a Direct object named origin for the sequence in file argv[1].
  Direct *origin = new Direct(argv[1]);
  Stack<Encoded> enc;
  Stack<Encoded> comp;

  for(int i = 2; i < argc - 1; i++)
  {
    // Declares and constructs a Direct object named seqtwo for the sequence in file argv[2].
    Direct *derived = new Direct(argv[2]);
    // Declares and constructs a Matrix object named m for the matrices.
    Matrix *m = new Matrix(*origin, *derived, pararec);
    // Declares and constructs an Alignment object named a for the alignment.
    Alignment *a = new Alignment(*m, pararec);
    // Declares and constructs an Encoded object named e for the differences.
    Encoded *e = new Encoded(*a);
    // Push the encoded object on to the Encoded stack
    enc.push(e);
    // Create and construct a COmpress object named c
    Compressed *c = new Compressed(*a);
    // Push the compressed object on to its respective stack
    comp.push(c);
  }
  
  Encoded smallE = findMin(&enc);

  Encoded bigE = findMax(&enc);

  Encoded smallC = findMin(&comp);

  Encoded bigC = findMax(&comp);

  cout << "A stack of Encoded pointers" << endl;
  cout << "Min Encoded number of differences: " << smallE.getNumDiff() << endl;
  cout << "Min Encoded name: " << smallE.getDName() << endl;
  cout << "Min Encoded sequence: " << smallE.getDSeq() << endl << endl;

  cout << "Max Encoded number of differences: " << bigE.getNumDiff() << endl;
  cout << "Max Encoded name: " << bigE.getDName() << endl;
  cout << "Max Encoded sequence: " << bigE.getDSeq() << endl << endl;
  
  cout << "A stack of Compressed pointers" << endl;
  cout << "Min Compressed number of differences: " << smallC.getNumDiff() << endl;
  cout << "Min Compressed name: " << smallC.getDName() << endl;
  cout << "Min Compressed sequence: " << smallC.getDSeq() << endl << endl;

  cout << "Max Compressed number of differences: " << bigC.getNumDiff() << endl;
  cout << "Max Compressed name: " << bigC.getDName() << endl;
  cout << "Max Compressed sequence: " << bigC.getDSeq() << endl << endl;


  return 0;
}