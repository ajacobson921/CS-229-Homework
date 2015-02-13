#ifndef WRAPPER_G_H  // A header guard is introduced to avoid including
#define WRAPPER_G_H  // this header file twice in any source code file.

void fatal(const char *msg); // a wrapper function
void fatal(const char *msg, const char *name); // a wrapper function
void ckopeninf(ifstream &infile, const char *fname); // a wrapper function

#endif
