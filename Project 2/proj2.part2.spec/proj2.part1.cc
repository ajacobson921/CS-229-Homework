// Template for Part 1 of Project 2
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

void fatal(const char *msg); // a wrapper function
void fatal(const char *msg, const char *name); // a wrapper function
void ckopeninf(ifstream &infile, const char *fname); // a wrapper function
int max(int a, int b);
int sigma(int a, int b, int match, int penalty);

struct scoretp  // a structure for scoring parameters
 { int  match;  // a positive score for a pair of identical DNA letters
   int  mismat; // a negative score for a pair of different DNA letters
   int  gopen;  // a negative score for a gap
   int  gext;   // a negative score for each letter in a gap
 };

struct Edit        // a structure for an edit operation
 { int   position; // a sequence position to which the edit operation is applied
   short indel;    // indel < 0, the edit is a deletion gap of length -indel;
                   // indel > 0, the edit is an insertion gap of length indel;
                   // indel = 0, the edit is a substitution.
 };

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

class Matrix   // a class for an object holding the three matrices
 { private:
    Direct &origin;  // reference to a Direct object called an original sequence
    Direct &derived; // reference to a Direct object called a derived sequence
    int  rowind; // max row index
    int  colind; // max column index
    int  **Dmat; // 2-dimensional array D[rowind + 1][colind + 1]
    int  **Imat; // 2-dimensional array I[rowind + 1][colind + 1]
    int  **Smat; // 2-dimensional array S[rowind + 1][colind + 1]

    int** get2dspace(int rowind, int colind); // allocates a 2D array on heap.
    void free2dspace(int rowind, int **arr);  // frees a 2D array on heap.

   public:
    Matrix(Direct &seqone, Direct &seqtwo, struct scoretp &param); // normal constructor
    ~Matrix();                  // destructor
    Direct& getOrigin() const;  // returns origin.
    Direct& getDerived() const; // returns derived.
    int getRowInd() const;      // returns rowind.
    int getColInd() const;      // returns rowind.
    int **getMat(char kind) const;    // returns a specified matrix.
    string toString(char kind) const; // generates a string form of a specified matrix.
 };

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
  *origin = &seqone;
  *derived = &seqtwo;
  rowind = origin.getSeq().getLength();
  colind = derived.getSeq().getLength();
  if(rowind < 1 || colind < 1)
  {
    fatal("Can't make a matrix with 0 dimension");
  }
  Dmat = get2dspace(rowind+1, colind+1);
  Smat = get2dspace(rowind+1, colind+1);
  Imat = get2dspace(rowind+1, colind+1);
  int q = param->gopen;
  int r = param->gext;
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
          Smat[i][j] = max(Smat[i+1][j+1] + sigma(so nept->seq[i], stwopt->seq[i], 10, papt->mismat), max(Dmat[i][j], Imat[i][j]));
      }
  }
}

// Frees heap memory.
Matrix:: ~Matrix()
{
 
  delete &origin;
  delete &derived;
  delete colind;
  free2dspace(rowind, Dmat);
  free2dspace(rowind, Imat);
  free2dspace(rowind, Smat);
  delete Dmat;
  delete Imat;
  delete Smat;
  delete rowlind;
}

int** Matrix:: get2dspace(int rowind, int colind)
// Allocates a 2-dimensional int array of (rowind + 1) by (colind + 1)
// and returns its memory address.
// Prints out an error message if rowind or colind is less than 0.
{
  int** mat[rowind+1][colind+1];
  return &mat;
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
    for(int i = 0; i < rowind+1, i++)
    {
      delete arr[i];
    }
    delete arr;
}

Direct& Matrix:: getOrigin() const
// Returns origin.
{
  return &origin;
}

Direct& Matrix:: getDerived() const
// Returns derived.
{
  return &derived;
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

Alignment:: Alignment(Matrix &matobj, struct scoretp &param)
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
  if(matobj.getOrigin.getLength() == 0 || matobj.getDerived().getLength() == 0)
  {
    fatal("Origin or derived is length 0");
  }
  &origin = matobj.getOrigin();
  &derived = matobj.getDerived();
  int m = matobj.getRowInd();
  int n = matobj.getColInd();

  char* arrtop[rowind+colind];
  char* arrmid[rowind+colind];
  char* arrbot[rowind+colind];
 

  intlen = 0;
  editnum = subinsertlen = 0;
  int q = matobj.Dmat[m][n];
  int r = matobj.Dmat[m-1][n] - q;
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
      if(j==n || matobj.Smat[i][j] == matobj.Dmat[i][j])
      {
          mat = 'D';
          editnum++;
          continue;
      }
      if(i == m || matobj.Smat[i][j] == matobj.Imat[i][j])
      {
          mat = 'I';
          editnum++;
          intlen = 0;
          continue;
      }
      arrtop[p] = arrA[i];
      arrbot[p] = arrB[j];
      if(arrA[i] == arrB[j])
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
      arrbot[p] = arrB[j];
      if(i == (m-1) || matobj.Dmat[i][j] == (matobj.Smat[i+1][j] - q - r))
      {
          mat = 'S';
      }
      i++;
      continue;
    }
    if(mat == 'I')
    {
      arrtop[p] = arrA[i];
      arrmid[p] = '-';
      arrbot[p] = ' ';
      intlen++;
      if(j ==n || matobj.Imat[i][j] == (matobj.Smat[i][j+1] - q - r))
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
  score = matobj.Smat[0][0];
  alignlen = p;
}

// Frees heap memory.
Alignment::  ~Alignment() // destructor definition
{
  delete origin;
  delete derived;
  delete score;
  delete editnum;
  delete subinsertlen;
  delete alignlen;
  delete[] top;
  delete[] mid;
  delete[] bot;
}

Direct& Alignment:: getOrigin() const
// returns origin.
{
 return &origin;
}

Direct& Alignment:: getDerived() const
// returns derived.
{
  return &derived; 
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
    return *top;
  }
  if(kind == 'M')
  {
    return *mid;
  }
  if(kind == 'B')
  {
    return *bot;
  }



}

string Alignment:: toString() const
// Input parameter sonept is a pointer to a structure with the features of one DNA sequence.
// Input parameter stwopt is for another DNA sequence.
// Input parameter matSmat is a pointer to a structure with three matrices.
// Input parameter algopt is a pointer to a structure for an optimal alignment.

// The function returns a summary of sequence and alignment information
// and the alignment in the form of string.
// The summary includes the name and length of each sequence,
// the score and length of the alignment. the number of edit operations,
// and the length of substitutions and insertions.
// The alignment is reported in sections of 70 characters, with each section consisting
// of three rows. The sequence positions of the first DNA letters in each section are
// reported in the left margin of 10 spaces.
{
  cout << "Sequence A: " + getOrigin().getName();
  cout << "Length : " + getOrigin().getLength();
  cout << "Sequence B: " + getDerived().getName();
  cout << "Length: " + getDerived().getLength();
  cout << "Alignment Score: " + getScore();
  cout << "Length: " + getAlignLen();
  cout << "Number of Edit Operations: " + getEditNum();
  cout << "Number of Substitutions and Insertions: " + getSubInsertLen(); 

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

int max(int a, int b)
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

int sigma(int a, int b, int match, int penalty)
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
