#ifndef Alignment_G_H  
#define Alignment_G_H 

#include "wrapper.hh"
#include "prototype.hh"
#include "Direct.hh"
#include "Matrix.hh"



class Alignment // a class for an object holding an optimal alignment  
 { private:
    Direct &origin;  // reference to a Direct object called an original sequence
    Direct &derived; // reference to a Direct object called a derived sequence
    int  score;      // the score of an optimal alignment of the two sequences
    int  editnum;    // the number of substitutions, insertion and deletion gaps
    int  subinsertlen; // the total length of substitutions and insertion gaps
    int  alignlen;   // the length of the alignment
    char *top;   // a char array of length alignlen for the top row of the alignment
    char *mid;   // a char array of length alignlen for the mid row of the alignment
    char *bot;   // a char array of length alignlen for the bottom row of the alignment

   public:
    Alignment(Matrix &matobj, struct scoretp &param); // normal constructor
    ~Alignment();                                     // destructor
    Direct &getOrigin() const;  // returns origin.
    Direct &getDerived() const; // returns derived.
    int getScore() const; // returns score.
    int getEditNum() const; // returns editnum.
    int getSubInsertLen() const; // returns insertlen.
    int getAlignLen() const; // returns alignlen.
    char* getRow(char kind) const; // returns top, mid, or bot row.
    string toString() const; // generates a string form of the alignment.
 };
 
 #endif
