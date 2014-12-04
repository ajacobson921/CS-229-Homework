#ifndef Direct_G_H
#define Direct_G_H


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

 #endif