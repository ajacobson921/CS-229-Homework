// Topics: another look at the use of a reference local variable,
//         checking the type of a template parameter at runtime,        
//         function template,
//         using references,
//         returning a smallest object from a stack,
//         returning a smallest reference from a stack.

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
#include "templatestack.min.hh"
#include "Direct.hh"
#include "Alignment.hh"
#include "Matrix.hh"
#include "Encoded.hh"
#include "prototype.hh"

int main(int argc, char *argv[])
{
   if ( argc != 6 )
   { cerr << "Usage: " << argv[0] << " Seq1 Seq2 mismatch gap_open gap_extend" << endl << endl;
     cerr << "Seq1        file of one sequence in FASTA format" << endl;
     cerr << "Seq2        file of one sequence in FASTA format" << endl;
     cerr << "mismatch    a negative integer" << endl;
     cerr << "gap_open    gap open penalty, a non-negative integer" << endl;
     cerr << "gap_extend  gap extension penalty, a positive integer" << endl;
     exit(1);
   }

  // Declares and initializes a struct scoretp variable named pararec.
 struct scoretp pararec
 {
    match = 10,
    mismat = argv[3],
    gopen = argv[4],
    gext = argv[5]
 };
  // Declares and constructs a Direct object named seqone for the sequence in file argv[1].
Direct *seqone = new Direct(argv[1]);
  // Declares and constructs a Direct object named seqtwo for the sequence in file argv[2].
Direct *seqtwo = new Direct(argv[2]);
  // Declares and constructs a Matrix object named matobj for the matrices.
Matrix *matobj = new Matrix(&seqone, &seqtwo, &pararec);
  // Declares and constructs an Alignment object named alignobj for the alignment.
Alignment *alignobj = new Alignment(&matobj, &pararec);
  // Declares and constructs an Encoded object named encodobj for the differences.
Encoded *encodobj = new Encoded(&alignobj);
  // Prints out the scoring parameters.
cout << "Match: 10";
cout << "Mismatch: " + pararec->mismat;
cout << "Gap open penalty: " + pararec->gopen;
cout << "Gap extension penalty: " + pararec->gext;
  // Sends the alignment along with other information to cout by calling toString() from alignobj.
cout << alignobj.toString();
  // Sends the contents in the object encodobj to cout by calling toString() from it.
cout << encodobj.toString();
  // Gets the sequence in seqtwo by calling getSeq().
Direct *dseq = seqtwo.getSeq();
  // Derives the sequence in dseq by calling getDSeq() from encodobj.
encodobj.getDSeq();
  // Runs strcmp() on the two sequences and prints out one of the two
  // messages based on the comparison:
  //  The original and derived sequences are identical.
  //  The original and derived sequences are different.
if(strcmp(alignobj.getOrigin().getName(), dseq.getDerived().getName()) == 0)
{
  cout << "The original and derived sequences are identical.";
}
else
{
  cout << "The original and derived sequences are different.";
}
  // Prints out each of the two sequences.
  cout << alignobj.getOrigin().getSeq();
  // sends each matrix to cout by calling the toString() of Matrix from matobj.
  cout << alignlen.getDerived().getSeq();
  // cout << alignobj.getDerived().getSeq();
  // cout << matobj.toString('D');
  // cout << matobj.toString('S');
  // cout << matobj.toString('I');

  return 0;
}