//@author Aaron Jacobson ajacob1
#ifndef Prototype_H
#define Prototype_H

struct seqtp   // a structure for three features of a sequence
{ char *name; // pointer to a string for keeping the name of a sequence
  char *seq;  // pointer to a char array for storing the sequence
  int  slen;  // the length of the sequence = char array length - 1,
};            // where seq[1], seq[2], ... seq[n] are used
// to keep the letters of a sequence of length n.


struct scoretp  // a structure for scoring parameters
{ 
	int match;  // a positive score for a pair of identical DNA letters
	int mismat; // a negative score for a pair of different DNA letters
	int gopen;  // a negative score for a gap
	int gext;   // a negative score for each letter in a gap
};

struct Edit        // a structure for an edit operation
 { 
 	int position; // a sequence position to which the edit operation is applied
  	short indel;  // indel < 0, the edit is a deletion gap of length -indel;
                  // indel > 0, the edit is an insertion gap of length indel;
                  // indel = 0, the edit is a substitution.
 };

#endif