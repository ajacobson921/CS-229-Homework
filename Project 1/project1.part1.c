// Part 1 of Project 1
//
// @author Aaron Jacobson ajacob1
//
// Acknowledgements
//
// 
//

#include <stdio.h>  // A C library with I/O functions like printf
#include <stdlib.h> // A library with malloc(), realloc(), free(), exit().
#include <string.h> // A library with strcpy(), strcmp(), strlen().


struct seqtp   // a structure for three features of a sequence
{ char *name; // pointer to a string for keeping the name of a sequence
  char *seq;  // pointer to a char array for storing the sequence
  int  slen;  // the length of the sequence = char array length - 1,
};            // where seq[1], seq[2], ... seq[n] are used
// to keep the letters of a sequence of length n.

struct mattp    // a structure for three matrices
{   int  rind;  // max row index
    int  cind;  // max column index
    int  **Dpt; // an int pointer to 2-dimensional array D[rind + 1][cind + 1]
    int  **Ipt; // an int pointer to 2-dimensional array I[rind + 1][cind + 1]
    int  **Spt; // an int pointer to 2-dimensional array S[rind + 1][cind + 1]
};

struct scoretp  // a structure for scoring parameters
{ 
    int  match;  // a positive score for a pair of identical DNA letters
    int  mismat; // a negative score for a pair of different DNA letters
    int  gopen;  // a negative score for a gap
    int  gext;   // a negative score for each letter in a gap
};

struct algtp    // a structure for an optimal alignment
{ int  score;  // the score of the alignment
    int  alen;   // the length of the alignment
    char *top;   // pointer to a char array for the top row of the alignment
    char *mid;   // pointer to a char array for the mid row of the alignment
    char *bot;   // pointer to a char array for the bottom row of the alignment
};              // each array is of length alen.

void fatal(const char *msg);
FILE *ckopen(const char *name, const char *mode);
void *check_malloc(size_t amount);
void *check_realloc(void *ptr, size_t amount);
void getseq(const char *fname, struct seqtp *seqrec);
int  **get2dspace(int rind, int cind);
void computemats(struct seqtp *sonept, struct seqtp *stwopt, struct scoretp *papt, struct mattp *matspt);
void outputmat(char kind, struct seqtp *sonept, struct seqtp *stwopt, int **Mpt);
void freeseq(struct seqtp *seqrec);
void free2dspace(int rind, int **arr);
void freemat(struct mattp *matspt);
void producealg(struct seqtp *sonept, struct seqtp *stwopt, struct mattp *matspt, struct algtp *algopt);
void freealg(struct algtp *algopt);
void outputalg(struct seqtp *sonept, struct seqtp *stwopt, struct scoretp *papt, struct algtp *algopt);
int max(int a, int b);
int sigma(int a, int b, int match, int penalty);

// The main() function calls the other functions
// to get the two files of sequences in fasta format
// into two seqtp structures, sets up the mattp structure,
// computes the matrices, prints out the matrices,
// produces an optimal alignment, and prints out
// the alignment. allocates memory, and frees memory.
//
// A DNA sequence in FASTA format begins with a single-line description,
// followed by lines of DNA letters in the sequence.
// The description line begins with the symbol '>' and continues with
// the name of the sequence. There is no space between the '>' and
// the first letter of the name, and no space within the name.
// The rest of the line is called a description (optional). In other words,
// if there is a description, then the name and the description is separated
// by one or more space. In this project, it is fine to assume that there is
// no description after the name. An example sequence in FASTA format is
// included in the .zip folder.
int main(int argc, char *argv[])
{
    if ( argc != 6 )
    { fprintf(stderr,"Usage: %s Seq1 Seq2 mismatch gap_open gap_extend\n\n", argv[0]);
        fprintf(stderr,"Seq1        file of one sequence in FASTA format\n");
        fprintf(stderr,"Seq2        file of one sequence in FASTA format\n");
        fprintf(stderr,"mismatch    a negative integer\n");
        fprintf(stderr,"gap_open    gap open penalty, a non-negative integer \n");
        fprintf(stderr,"gap_extend  gap extension penalty, a positive integer \n");
        // Each base match score is always 10.
    }
        struct scoretp scores =
        {
            .match = 10,
            .mismat = atoi(argv[3]),
            .gopen = atoi(argv[4]),
            .gext = atoi(argv[5])
        };
    
    printf("Match Score\tMismatch Score\tGap-Open Penalty \tGap-Extension Penalty\n");
    printf("\t\t%i\t\t\t\t%i\t\t\t\t%i\t\t\t\t\t\t%i", 10, scores.mismat, scores.gopen, scores.gext);
    
    
        const char *seq1arr = argv[1];
        const char *seq2arr = argv[2];
        struct seqtp seq1 =
        {
            .name = NULL,
            .seq = NULL,
            .slen = 0
        };
        
        struct seqtp seq2 =
        {
            .name = NULL,
            .seq = NULL,
            .slen = 0
        };
        
        struct mattp matrices =
        {
            .rind = 0,
            .cind = 0,
            .Dpt = NULL,
            .Ipt = NULL,
            .Spt = NULL
        };
        
        getseq(seq1arr, &seq1);
        getseq(seq2arr, &seq2);
        
        computemats(&seq1, &seq2, &scores, &matrices);
        
        struct algtp optimal =
        {
            .score = 0,
            .alen = 0,
            .top = NULL,
            .mid = NULL,
            .bot = NULL
        };
        
        producealg(&seq1, &seq2, &matrices, &optimal);
        
        outputalg(&seq1, &seq2, &scores, &optimal);
        
        freeseq(&seq1);
        freeseq(&seq2);
        freemat(&matrices);
        freealg(optimal);
        
        exit(1);
}
    

    


void getseq(const char *fname, struct seqtp *seqrec)
{
    if(fname == NULL)
    {
        fatal("the file name is null");
    }
    FILE* f = ckopen(fname, "r");
    
    if(f == NULL)
    {
        fatal("Null file");
    }
    
    //get length of the file
    int count;
    int c;
    while((c = fgetc(f) != EOF))
    {
        count++;
    }
    
    
    //get name
    char d[1024];
    fgets(d, 1024, f);
    
    
    int g;
    int ncount = 0;
    fseek(f, 0, SEEK_SET);
    while((g = d[ncount] != '\n'))
    {
        ncount++;
    }
    char* name_arr = check_malloc(ncount * sizeof(char));
    
    c = fgetc(f);
    if (c != '>')
    {
        fatal("No name");
    }
    int e;
    for(int i = 0; i < ncount -1; i++)
    {
        e = fgetc(f);
        name_arr[i] = (char) e;
    }
    
    //get sequence
    int dcount = count - ncount;
    char* sequence = check_malloc((dcount) * sizeof(char));
    e = fgetc(f);
    for(int i = 0; i < ncount; i++)
    {
        e = fgetc(f);
        sequence[i] = e;
    }
    seqrec->slen = dcount;
    seqrec->name = name_arr;
    seqrec->seq = sequence;
    fclose(f);
}
// Input parameter fname is a pointer to a string for storing the name of a DNA sequence file.
// Output parameter seqrec is a pointer to a structure whose members are used to
// save the name, length and DNA letters of the sequence in the file.
// A proper amount of memory needs to be allocated for each pointer member.

// This function opens the file whose name is pointed to by fname,
// reads the name, length and DNA letters of the sequence in the file,
// dynamically allocates a proper amount of memory for each pointer member
// in the structure pointed to by seqrec, and saves the data into the member variables.
// The function prints out a proper error message if fname is NULL,
// no file exists with the given name, or the sequence/its name is empty.

void freeseq(struct seqtp *seqrec)
{
    free(seqrec->name);
    free(seqrec->seq);
    seqrec->name = NULL;
    seqrec->seq = NULL;
    seqrec->slen = 0;
    seqrec = NULL;
    
    
}
// Frees each dynamically allocated memory block in the seqtp structure.

int **get2dspace(int rind, int cind)
{
    if(rind < 1 || cind < 1)
    {
        fatal("Cannot make an array with less than 1!");
    }
    int **arr = check_malloc((rind + 1) * sizeof(int));
    int i;
    for(i = 0; i < rind + 1; i++)
    {
        arr[i] = (int*) check_malloc((cind+1) * sizeof(int));
    }
    return arr;
}
// Allocates a 2-dimensional int array of (rind + 1) by (cind + 1)
// and returns its memory address.
// Prints out an error message if rind or cind is less than 1.

void free2dspace(int rind, int **arr)
{
    if(rind < 0)
    {
        fatal("Number less than 0");
    }
    int i;
    for(i = 0; i < rind + 1; i++)
    {
        free(arr[i]);
    }
    free(arr);
}
// Deallocates a 2-dimensional int array of first dimension
// (rind + 1) by releasing the memory for arr[j] at each j
// and then releasing the memory for arr.
// Prints out an error message if rind is less than 0.

void freemat(struct mattp *matspt)
{
    free2dspace(matspt->rind, matspt->Dpt);
    free2dspace(matspt->rind, matspt->Ipt);
    free2dspace(matspt->rind, matspt->Spt);
    free(matspt->Dpt);
    free(matspt->Ipt);
    free(matspt->Spt);
    matspt->rind = 0;
    matspt->cind = 0;
    matspt = NULL;
}
// Frees each dynamically allocated memory block in the mattp structure.

void computemats(struct seqtp *sonept, struct seqtp *stwopt, struct scoretp *papt, struct mattp *matspt)
{
    matspt->rind = sonept->slen;
    matspt->cind = stwopt->slen;
    matspt->Dpt = get2dspace((matspt->rind) +1,(matspt->cind) +1);
    matspt->Ipt = get2dspace((matspt->rind) +1,(matspt->cind) +1);
    matspt->Spt = get2dspace((matspt->rind) +1,(matspt->cind) +1);
    
    
    int **Spt = matspt->Spt;
    int **Dpt = matspt->Dpt;
    int **Ipt = matspt->Ipt;
    
    int m = matspt->rind;
    int n = matspt->cind;
    int q = papt->gopen;
    int r = papt->gext;
    
    Spt[m][n] = 0;
    Dpt[m][n] = (Spt[m][n]) - q;
    Ipt[m][n] = (Spt[m][n]) - q;
    for(int j = n; j > 0; j--)
    {
        Ipt[m][j] = Ipt[m][j-1] - r;
        Spt[m][j] = Ipt[m][j];
        Dpt[m][j] = Spt[m][j] - q;
    }
    for(int i = m; i > 0; i--)
    {
        Dpt[i][n] = Dpt[i+1][n] - r;
        Spt[i][n] = Dpt[i][n];
        Ipt[i][n] = Spt[i][n] - q;
        for(int j = n; j > 0; j--)
        {
            Dpt[i][j] = max(Dpt[i+1][j] - r, Spt[i+1][j] - q - r);
            Ipt[i][j] = max(Ipt[i][j+1] - r, Spt[i][j+1] - q - r);
            Spt[i][j] = max(Spt[i+1][j+1] + sigma(sonept->seq[i], stwopt->seq[i], 10, papt->mismat), max(Dpt[i][j], Ipt[i][j]));
        }
    }
    
    
    
}
// Input parameter sonept is a pointer to a structure with the features of one DNA sequence.
// Input parameter stwopt is for another DNA sequence.
// Input parameter papt is for the scoring parameters.
// Output parameter matspt is a pointer to a structure with three matrices.

// The function sets the rind member of the structure pointed to
// by matspt to the slen member of the structure pointed to by sonept,
// and cind to the slen member by stwopt. Then it allocates space for each
// matrix in the mattp structure by calling get2dspace(). Next it computes,
// in reverse order, the three matrices by the dynamic programming algorithm
// for aligning the two sequences.

void outputmat(char kind, struct seqtp *sonept, struct seqtp *stwopt, int **Mpt)
{
 //   printf("Matrix type: " + "%s\n", kind);
    
    
    
    
    
    
    
}
// Input parameter kind denotes matrix type: 'D', 'I', or 'S'.
// Input parameter sonept is a pointer to a structure with a sequence of length M
// Input parameter sonept is a pointer to a structure with a sequence of length N
// Input parameter Mpt is a pointer to a 2-dimensional array (matrix)
// of (M + 1) by (N + 1).

// The function reports the matrix type and each value in the matrix
// on the stdout. The values are reported first in order of row index i
// and then in order of column index j in the form of (i, j, Mpt[i][j])
// so that the row and column coordinates of a value are easily recognized.
// This function is for your own use to check on each matrix,
// so any format is OK. For small matrices, you can also report
// them in the format of an output file included in the .zip file.

void producealg(struct seqtp *sonept, struct seqtp *stwopt, struct mattp *matspt, struct algtp *algopt)
{
    int m = matspt->rind;
    int n = matspt->cind;
    
    int **Spt = matspt->Spt;
    int **Dpt = matspt->Dpt;
    int **Ipt = matspt->Ipt;
    
    char *arrA = sonept->seq;
    char *arrB = stwopt->seq;
    int p = 0;
    char *arrtop = check_malloc((m+n) * sizeof(char));
    char *arrmid = check_malloc((m+n) * sizeof(char));
    char *arrbot = check_malloc((m+n) * sizeof(char));
    
    int q = Dpt[m][n];
    int r = Dpt[m-1][n] - q;
    
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
            if(j==n || Spt[i][j] == Dpt[i][j])
            {
                mat = 'D';
                continue;
            }
            if(i == m || Spt[i][j] == Ipt[i][j])
            {
                mat = 'I';
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
            if(i == (m-1) || Dpt[i][j] == (Spt[i+1][j] - q - r))
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
            if(j ==n || Ipt[i][j] == (Spt[i][j+1] - q - r))
            {
                mat = 'S';
            }
            j++;
            continue;
        }
    }
    algopt->score = matspt->Spt[0][0];
    algopt->alen = p;
    algopt->top = arrtop;
    algopt->mid = arrmid;
    algopt->bot = arrbot;
}
// Input parameter sonept is a pointer to a structure with the features of one DNA sequence.
// Input parameter stwopt is for another DNA sequence.
// Input parameter matspt is a pointer to a structure with three matrices.
// Output parameter algopt is a pointer to a structure for an optimal alignment.

// The function allocates memory for each row in the algtp structure,
// produces an optimal alignment by tracing through the matrices in the mattp structure,
// and saves the alignment along with it score and length in the algtp structure.
// The alignment is represented by using three rows of characters (three char arrays).

// Note that the length of an optimal alignment cannot exceed the sum of
// the lengths of the two DNA sequences.

void freealg(struct algtp *algopt)
{
    free(algopt->top);
    free(algopt->mid);
    free(algopt->bot);
    algopt->alen = 0;
    algopt->score = 0;
    algopt = NULL;
}
// Frees each dynamically allocated memory block in the algtp structure.

void outputalg(struct seqtp *sonept, struct seqtp *stwopt, struct scoretp *papt, struct algtp *algopt)
{
    printf("\nSequence 1 name: %s\n", sonept->name);
    printf("\nSequence 1 length: %i\n", sonept->slen);
    printf("\nSequence 2 name: %s\n", stwopt->name);
    printf("\nSequence 2 length: %i\n", stwopt->slen);
    printf("\nAlignment score: %i\n", algopt->score);
    printf("\nAlignment length: %i\n", algopt->alen);
    
    int charnum = 1;
    int length = algopt->alen;
    char *top = algopt->top;
    char *mid = algopt->mid;
    char *bot = algopt->bot;
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
            printf("%c\n", top[tcount]);
            tcount++;
        }
        
        printf("\n\t\t");
        
        for(int l = 0; l < k; l++)
        {
            printf("%c\n", mid[mcount]);
            mcount++;
        }
        
        printf("\n\t\t");
        
        for(int k = 0; k < 70; k++)
        {
            printf("%c\n", bot[bcount]);
            bcount++;
        }
        
        printf("\n");
        length = length - k;
        j = 0;
    }
}
// Input parameter sonept is a pointer to a structure with the features of one DNA sequence.
// Input parameter stwopt is for another DNA sequence.
// Input parameter matspt is a pointer to a structure with three matrices.
// Input parameter algopt is a pointer to a structure for an optimal alignment.

// The function reports, on the stdout, a summary of sequence and alignment information
// and the alignment. The summary includes the name and length of each sequence,
// the values of the four scoring parameters, and the score and length of the alignment.
// The alignment is reported in sections of 70 characters, with each section consisting
// of three rows. The sequence positions of the first DNA letters in each section are
// reported in the left margin of 10 spaces.


// Students are welcome to use the functions below.

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



// Checks whether realloc() succeeds if amount is not 0.
void *check_realloc(void *ptr, size_t amount)
{
    void *tpt;
    // Returns the old memory block and allocates a new one in amount bytes.
    tpt = realloc(ptr, amount);
    
    /* Checks if it was successful. */
    if ( amount != 0 && tpt == NULL )
    { // prints a message to standard error device (console).
        fprintf(stderr, "No memory of %lu bytes\n", amount);
        //   printf("No memory of %lu bytes\n", amount);
        exit(1); // stops unexpectedly.
    }
    
    return tpt;
}

// Checks if malloc() succeeds.
void *check_malloc(size_t amount)
{
    void *tpt;
    /* Allocates a memory block in amount bytes. */
    tpt = malloc( amount );
    
    /* Checks if it was successful. */
    if ( tpt == NULL )
    { // prints a message to standard error device (console).
        fprintf(stderr, "No memory of %lu bytes\n", amount);
        //  printf("No memory of %lu bytes\n", amount);
        exit(1); // stops unexpectedly.
    }
    
    return tpt;
}

// Prints out an error message and stops.
void fatal(const char *msg)
{
    fprintf(stderr, "Error message: %s\n", msg);
    //  printf("Error message: %s\n", msg);
    exit(1); // stops unexpectedly.
}

/* ckopen - open file; check for success */
FILE *ckopen(const char *name, const char *mode)
{
    FILE *fp;
    // *fopen() in an early template is removed.
    
    if ((fp = fopen(name, mode)) == NULL)
    { fprintf(stderr, "Cannot open %s\n", name);
        //    printf("Cannot open %s\n", name);
        exit(1); // stops unexpectedly.
    }
    return(fp);
}
