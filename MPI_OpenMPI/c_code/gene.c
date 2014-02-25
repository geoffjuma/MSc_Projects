/**************************************************************************/
/*                                                                        */
/*   Program Name: mpi_lcs.c                                              */
/*   Program Description:  mpi_lcs.c is a C program providing a first     */
/*                         example in programming parallel algorithms     */
/*                         for bioinformatics applications.  The program  */
/*                         was developed with the support of the          */
/*                         National Science Foundation grant              */
/*                         to develop expertise in computational science. */
/*                         The program is intended to be illustrative     */
/*                         of several parallel programming concepts       */
/*                         required for successful first parallel         */
/*                         programs. It is intended to provide a general  */
/*                         framework that can be employed to develop      */
/*                         additional parallel programs using emerging    */
/*                         parallel computing technologies.               */
/*                                                                        */
/*                         The program implements the longest common      */
/*                         subsequence algorithm used to compare each     */
/*                         bioinformatics sequence to all other sequences */
/*                         in an input file.                              */
/*                                                                        */
/*                         Usage:                                         */
/*                         The program uses one input file, query.fasta   */
/*                         to provide the input data to the program.      */
/*                         The file must reside in the directory in which */
/*                         the program is invoked.                        */
/*                                                                        */
/*                         The usage is extremely simplified and not      */
/*                         generally flexible to reduce the number of     */
/*                         potential problems in using the program in a   */
/*                         classroom environment.                         */
/*                                                                        */
/*                         Modifications are suggested as exercises       */
/*                         for students to aid their learning of skills   */
/*                         required to develop parallel algorithms.       */
/*                         These modifications are discussed in the       */
/*                         supporting document for the computational      */
/*                         science module.                                */
/*                                                                        */
/*  Author: Eric Stahlberg                                                */
/*  Date:   July 2008                                                     */
/*                                                                        */
/**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <sys/time.h>
#include <stdlib.h>

/*                                                                        */
/* macro to compute offset for i,j combination using a matrix with n rows */
/*                                                                        */

#define INDEX(i,j,n)     ((j)*(n)+(i))
#define DIAGONAL_ARROW    -3
#define UP_ARROW          -1
#define LEFT_ARROW	  -2
#define UNSET_ARROW        0
#define FALSE 0
#define TRUE 1

#define TRACE_ON           1
#define TRACE_OFF          0

#define LINE_WIDTH         60

#define MAX_LINE           1024

/* ----------BEGIN SUPPORTING METHODS AND CONSTRUCTS ----------------*/

/* Common parallel abstractions used in developing portable programs */

int PARALLEL_pe_id()
/*********************************************************************/
/* This function provides a portable wrapper to easily extract the   */
/* identity of the processing element within the pool of active      */
/* processes.                                                        */
/* Input: none                                                       */
/* Output: the integer identifier for the processign element         */
/*********************************************************************/
{
   int mpi_rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
   return mpi_rank;
}
int PARALLEL_pe_count()
/*********************************************************************/
/* This function provides a portable wrapper to easily extract the   */
/* identity of the processing element within the pool of active      */
/* processes.                                                        */
/* Input: none                                                       */
/* Output: the integer identifier for the processign element         */
/*********************************************************************/
{
   int pe_count;
   MPI_Comm_size(MPI_COMM_WORLD,&pe_count);
   return pe_count;
}
void PARALLEL_barrier()
/*********************************************************************/
/* This function provides a portable wrapper for a commonly used     */
/* method to synchronize all active processing elements.             */
/* Input: none                                                       */
/* Output: none                                                      */
/*********************************************************************/

{
      MPI_Barrier(MPI_COMM_WORLD);

}
int PARALLEL_master()
/*********************************************************************/
/* This function provides a portable wrapper to easily test whether  */
/* the given processing element is the master process.               */
/* Input: none                                                       */
/* Output: 1 if processing element is the master, otherwise 0        */
/*********************************************************************/
{
   int mpi_rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
   return mpi_rank == 0 ;
}

/* Common memory allocation abstractions used in developing programs */
/* Such abstractions permit more convenient tracing and substitution */
/* of new methods as needed while minimizing changes to the code     */

void *memory_allocate( int size )
/*********************************************************************/
/* generic memory allocation interface                               */
/* wrap C malloc function to provide tracing ability if needed       */
/*                                                                   */
/* input:                                                            */
/*       size : number of bytes to allocate                          */  
/* return value: pointer to allocated memory                         */
/*********************************************************************/
{
      return (void *) malloc(size);
}
void memory_free( void *ptr)
/*********************************************************************/
/* generic memory free interface                                     */
/* wrap C malloc function to provide tracing ability if needed       */
/*                                                                   */
/* input:                                                            */
/*       ptr: pointer to memory block to free                        */  
/*********************************************************************/
{
      free (ptr);
}
int *int_memory_allocate( int size)
/*********************************************************************/
/* generic wrapper to allocate integer lists                        */
/*********************************************************************/
{
    return (int *) memory_allocate ( size * sizeof(int) );
}
char *char_memory_allocate( int size)
/*********************************************************************/
/* generic wrapper to allocate character lists                       */
/*********************************************************************/
{
    return (char *) memory_allocate (size);
}
/* ----------END SUPPORTING METHODS AND CONSTRUCTS -------------------*/

/* ------BEGIN APPLICATION SPECIFIC METHODS AND CONSTRUCTS -----------*/

/**********************************************************************/
/* data structure of task information to pass to parallel process     */
/**********************************************************************/
typedef struct {

    /* parallel environment information */
    int pe_id;                      /* identifier for the PE  */
    int num_pes;                    /* number of PEs          */

    /* application specific information */
    int num_query_sequence;             /* number of sequences        */
    char *query_sequence;               /* pointer to sequence data   */
    int *query_sequence_start;          /* pointer to sequence start points*/
    int *query_sequence_length;         /* pointer to sequence length */

    char *query_header;                 /* pointer to header data     */
    int *query_header_start;            /* pointer to header start points */
    int *query_header_length;           /* pointer to header length   */

} parallel_data;

void displaymatrix(int *m, int nr, int nc )
/************************************************************************/
/* purpose: display integer matrix                                      */
/*                                                                      */
/* input parameters                                                     */
/* m : matrix to display                                                */
/* nr : number of rows                                                  */
/* nc : number of columns                                               */
/*                                                                      */
/************************************************************************/
{
     int i,j;
     for ( i = 0 ; i < nr ; i++ )
        {
        for ( j = 0 ; j < nc ; j++ )
          printf("%4d ",m[INDEX(i,j,nr)]);
        printf("\n");
        }
}

void displaystring(char *string, int length)
/***********************************************************************/
/*  method:  display a character string                                */
/*  purpose: display length characters of a given character string     */
/*                                                                     */
/*  input:   char *string - start of characters to display             */
/*  input:   length - number of characters to display                  */
/***********************************************************************/
{
   int i;
   for (i = 0 ; i < length; i++ ) {
    printf("%c",string[i]);
    if ( ( (i+1) % LINE_WIDTH == 0 ) && i < (length -1) ) 
      printf("\n");
   }
   printf("\n");
}

void displaybacktrack(int *b, int nr, int nc)
/***********************************************************************/
/*  method:  display optimal choice backtrack matrix                   */
/*  purpose: display the optimal choice matrix                         */
/*                                                                     */
/*  input:                                                             */
/*    int *b - matrix of optimal choices for each pair of substrings   */
/*    int nr - number of elements in the string defining the rows      */
/*    int nc - number of elements in the string defining the columns   */
/*    length - number of characters to display                         */
/***********************************************************************/
{
     int i,j;
     for ( i = 0 ; i < nr ; i++ )
      {
        for ( j = 0 ; j < nc ; j++ ) {
          if ( b[INDEX(i,j,nr)] == DIAGONAL_ARROW )  printf("%4s ","\\");
          if ( b[INDEX(i,j,nr)] == UP_ARROW )  printf("%4s ","^");
          if ( b[INDEX(i,j,nr)] == LEFT_ARROW )  printf("%4s ","<");
          if ( b[INDEX(i,j,nr)] == UNSET_ARROW )  printf("%4s ","*");
        }
        printf("\n");
      }
}

int probeFASTA( char *filename, 
                int *number_sequences, 
                int *total_header_length, 
                int *total_sequence_length)
{
/*******************************************************************/
/* probe size of FASTA file                                        */
/* input                                                           */
/*     filename : filename of FASTA file to test                   */
/* output                                                          */
/*   number_sequences: number of sequences in the file             */
/*   total_header_length : number of characters in the header      */
/*   total_sequence_length : number of characters in the sequences */
/*   author: Eric Stahlberg                                        */
/*   date: August 2007                                             */
/*******************************************************************/

   FILE *filep = fopen(filename, "r");  /* file pointer to read */
  
   char buffer[MAX_LINE];  /* maximum line length from an input file */
   char c;                 /* character read in each line */
   int done = FALSE;       /* flag to set when complete   */
   int i;                  /* iterator */
   int n_seq = 0;          /* number of sequences read */
   int len_header = 0;     /* length of individual header space */
   int len_sequence = 0;   /* length of individual sequence space */

   *number_sequences = 0;   /* number of sequences */
   *total_header_length = 0; /* total length of header space */
   *total_sequence_length = 0; /* total length of sequence space */

   if ( filep == NULL ) return;  /* if file pointer invalid, return */

   /* repeat through the file until an EOF is found */
   while ( ! done ) {
      /* fill the buffer by getting and assigning character */
      /* from buffer to line*/
      i = 0;
      do { 
          c = fgetc(filep);
          if ( c != '\n'  && c != EOF  && i < MAX_LINE ) {
          buffer[i] = c;
          i++;
          }
      } while (c != EOF  && c != '\n' );
      buffer[i] = '\0';

      /* check type of line read to take appropriate action */
      /* if first character is FASTA header signal, then use */
      /* prior input as a sequence, updating information  */
       
      if ( buffer[0] == '>' ) {
          n_seq++;
          len_header += strlen(buffer); 
      }
      else {
          len_sequence += strlen(buffer);
      }

      done = ( c == EOF );
   }
   *number_sequences      = n_seq;
   *total_header_length   = len_header;
   *total_sequence_length = len_sequence;
       
}
int readFASTA( char *filename, 
               int *number_of_sequences, 
               int *sequence_length, 
               int *sequence_start, 
               int *header_length, 
               int *header_start, 
               char *headers, char *sequences)
/*******************************************************************/
/* purpose: read FASTA file into allocated storage                 */
/* input                                                           */
/*     filename : name of FASTA file                               */
/* output                                                          */
/*     number_of_sequences : number of sequences read              */
/*     sequence_length : list of sequence lengths                  */
/*     sequence_start : starting offsets for each sequence         */
/*     header_length : list of header lengths                      */
/*     header_start : starting offsets for each header             */
/*     headers : character storage for all header data             */
/*     sequences : character storage for all sequence data         */
/* author: Eric Stahlberg                                          */
/* date: August 2007                                               */
/*******************************************************************/
{
   FILE *filep = fopen(filename, "r");
   char buffer[MAX_LINE];
   char c;
   int done = FALSE;
   int i;
   int n_seq = -1;
   int len_header = 0;
   int len_sequence = 0;


   if ( filep == NULL ) return;
 
   if ( *number_of_sequences <= 0 ) return;
   
   /* initialize return indexes */
   for ( i = 0 ; i < *number_of_sequences ; i++ )
   {
      sequence_length[i] = 0;
      sequence_start[i] = 0;
      header_length[i] = 0;
      header_start[i] = 0;
   }
   while ( ! done ) {
      /* fill the buffer */
      i = 0;
      do { 
          c = fgetc(filep);
          if ( c != '\n'  && c != EOF  && i < MAX_LINE ) {
          buffer[i] = c;
          i++;
          }
      } while (c != EOF  && c != '\n' );
      buffer[i] = '\0';

      /* check type of line read */
      if ( buffer[0] == '>' ) {
          n_seq++;
          /* recover length of the header */
          header_length[n_seq] = strlen(buffer);
          /* update starting position in total buffer */
          if ( n_seq > 0 ) {
             header_start[n_seq] = header_start[n_seq-1]+header_length[n_seq-1];
          }
          /* copy the buffer to the total buffer */
          memcpy(&headers[header_start[n_seq]], buffer, strlen(buffer));
      }
      else {
          if ( n_seq > 0 ) {
             sequence_start[n_seq] = sequence_start[n_seq-1] + sequence_length[n_seq-1];
          }
          if ( n_seq > -1 ) {
             memcpy(&sequences[sequence_start[n_seq] + sequence_length[n_seq]], buffer, strlen(buffer));
             sequence_length[n_seq] += strlen(buffer);
          }
          
      }

      done = ( c == EOF );
   }
   *number_of_sequences      = n_seq+1;

   /* add terminators to stored sequence and header characters */

   headers[header_start[n_seq]+header_length[n_seq]] = '\0';
   sequences[sequence_start[n_seq]+sequence_length[n_seq]] = '\0';

   return 0;

}

void ibubble_sort (int *index, int *values, int length)
/************************************************************************/
/*     method: index bubble sort                                        */
/*     purpose: create a sorted index of entries in the values list     */
/*     input:  int values[]  - list of values to be sorted              */
/*     input:  int length - number of elements in the list to sort      */
/*     output: int index[] - sorted index of values, greatest first     */
/************************************************************************/
{
     int i, j;
     /* initialize index */
     for ( i = 0 ; i < length ; i++ ) 
         index[i] = i;

     for ( i = 0 ; i < length ; i++ )
           for ( j = 0 ; j < length -i - 1 ; j++ )
                if ( values[index[j]] < values[index[j+1]] )  {
                     int tmp = index[j+1];
                     index[j+1] = index[j];
                     index[j] = tmp;
                   }
}
/* -----------END APPLICATION SPECIFIC METHODS AND CONSTRUCTS ---------------*/

/* -----------BEGIN ALGORITHM SPECIFIC METHODS ------------------------------*/

int match_character (char *s1 , char *s2 )
/*****************************************************************************/
/*                                                                           */
/* return character comparison value                                         */
/*                                                                           */
/* input                                                                     */
/* s1 : first character                                                      */
/* s2 : second character                                                     */
/* return value: value of match                                              */
/*               0 = no match                                                */
/*               1 = match                                                   */
/*                                                                           */
/*****************************************************************************/
{ 
    if ( *s1 == *s2 ) return 1;
    else return 0;
}

void LCSdisplay(char *sequence, int *backtrack, int nrow, int i, int j)
/*****************************************************************************/
/*     method: LCSdisplay                                                    */
/*                                                                           */
/*     purpose: display longest common subsequence                           */
/*                                                                           */
/*     input:  char sequence[] - original sequence used to define            */
/*                               the row of the backtrack matrix             */
/*     input: int backtrack[]  - record of optimial choices made in          */
/*                               determining lcs                             */
/*     input: int nrow - number of characters in the row sequence + 1        */
/*     input: int i,j - current optimal choice                               */
/*                                                                           */
/*     NOTE: this method is recursive and prints characters as               */
/*           required to stdout                                              */
/*****************************************************************************/
{
    if ( i == 0 || j == 0 )
        return;

    /* check for a diagonal transition to this point */
    if (backtrack[INDEX(i,j,nrow)] == DIAGONAL_ARROW) {

        LCSdisplay(sequence,backtrack,nrow,i-1,j-1);
        printf("%c",sequence[i-1]);
    }
    else 
        /* check for an up arrow transition to this point */
        if (backtrack[INDEX(i,j,nrow)] == UP_ARROW )
           LCSdisplay(sequence,backtrack,nrow,i-1,j);
        else
           LCSdisplay(sequence,backtrack,nrow,i,j-1);
}


void LCSupdate(int i, int j, int nrow, int ncol, int *matrix, 
               int *backtrack, char *string1, char *string2)
/*****************************************************************************/
/*    method: LCSupdate                                                      */
/*    purpose: update the score and backtrack matrix for substrings          */
/*             ending at position i and position j                           */
/*                                                                           */
/*    input:                                                                 */
/*    int i - index of character in row string                               */
/*    int j - index of cahracter in column                                   */
/*    int nrow - number of rows in the score matrix                          */
/*    int ncol - number of columns in the score matrix                       */
/*    int matrix[] - 2D matrix for cumulative optimal scores                 */
/*    int backtrack[] - 2D record of choices made                            */
/*    char string1[] - characters corresponding to each row of the matrix    */
/*    char string2[] - characters corresponding to each column of the matrix */
/*                                                                           */
/*    output:                                                                */
/*    int matrix[] - updated 2D matrix of optimal scores                     */
/*    int backtrack[] - updated 2D matrix of optimal selections              */
/*                                                                           */
/*****************************************************************************/
{
    int s1, s2;

    /* if i or j index is not even a part of a sequence, return */
    if ( i == 0 || j == 0 ) { 
         matrix[INDEX(i,j,nrow)] = 0; 
         return;
    }
    /* Check status of match character on the position i-1 and j-1 and */
    /* update score and backtrack matrix only if the characters do match */

    if ( match_character( &string1[i-1], &string2[j-1]) ) {

         matrix[INDEX(i,j,nrow)] = matrix[INDEX(i-1,j-1,nrow)]+1;

         if ( backtrack != NULL ) backtrack[INDEX(i,j,nrow)] = DIAGONAL_ARROW;

         return;
    }
    /* Now you know characters don't match, so find best value thus far to
    /* keep moving longest common sequence count forward */
    
    s1 = matrix[INDEX(i-1,j,nrow)] ;

    s2 = matrix[INDEX(i,j-1,nrow)] ;

    if ( s1 > s2 ) {
       matrix[INDEX(i,j,nrow)] = s1;
       if ( backtrack != NULL ) backtrack[INDEX(i,j,nrow)] = UP_ARROW;
    }
    else {
       matrix[INDEX(i,j,nrow)] = s2;
       if ( backtrack != NULL ) backtrack[INDEX(i,j,nrow)] = LEFT_ARROW;
    }

    return;

}

int computeLCS( char *string1, int len1, char *string2, int len2, int display)
/***********************************************************************/
/*                                                                     */
/* purpose: compute longest common sub-sequence comparison             */
/*                                                                     */
/* input                                                               */
/* string1 : pointer to start of first string                          */
/* len1    : number of characters to compare in first string           */
/* string2 : pointer to start of second string                         */
/* len2    : number of characters to compare in second string          */
/* display : integer (1 = display result; 0 = no display )             */
/*                                                                     */
/* return value: length of longest common subsequence                  */
/*                                                                     */
/***********************************************************************/
{
      int i,j;
      int max;
      int *matrix;
      int *backtrack;
      int nrow, ncol;
      int maxrow, maxcol;

      if ( display == TRACE_ON ) 
         printf("[LCS_START] Commencing Longest Common Subsequence Algorithm\n");

      /* This method is written to permit comparison of an initial */
      /* subset of a given string. The following steps are used to */
      /* determine how much of the string will be compared based on */
      /* the input arguments to the method.                         */
      /* determine number of characters to compare from string 1 */
      nrow = strlen(string1);

      if ( nrow > len1 ) nrow = len1;

      /* determine number of characters to compare from string 2 */
      ncol = strlen(string2);
      if ( ncol > len2 ) ncol = len2;
      
      /* Account for storage space needed for base condition */
      /* index 0 is for the base condition of the recurrence relation */
      /* in the scoring and backtrack matrices */

      nrow++;
      ncol++;
      
      /* allocate integer scoring matrix of the entire size */
      matrix = int_memory_allocate( nrow * ncol );

      /* inititalize the scoring matrix base conditions */

      for ( i = 0 ; i < nrow ; i++ ) 
        matrix[ INDEX(i,0,nrow) ] = 0;
      for ( j = 0 ; j < ncol ; j++ )
        matrix[ INDEX(0,j,nrow) ] = 0;


      /* allocate storage for the backtracking matrix if required */
      /* Note: from this point forward, if no storage has been    */
      /* allocated for the backtracking matrix, then it will not  */
      /* be updated.                                              */
      if ( display == TRACE_ON )  {

          backtrack = int_memory_allocate( nrow*ncol );

          /* inititalize backtrack matrix */
          for ( i = 0 ; i < nrow ; i++ ) 
            backtrack[ INDEX(i,0,nrow) ] = 0;
          for ( j = 0 ; j < ncol ; j++ )
            backtrack[ INDEX(0,j,nrow) ] = 0;
      }
      else
          backtrack = NULL;


      /* evaluate LCS matrix for all elements in order */
      for ( i = 0 ; i  < nrow ; i++ )
         for ( j = 0 ; j < ncol ; j++ ) 
           {
             LCSupdate(i, j, nrow, ncol, matrix, backtrack, string1, string2);

           }
      /* Determine the longest subsequence by examining the scoring matrix */

      max = 0;    /* maximum length of the longest common subsequence */
      maxrow = 0; /* row where the maximum score was located */
      maxcol = 0; /* column where the maximum score was located */

      /* check the last column of the scoring matrix for the largest value */

      for ( i = 1 ; i < nrow ; i++ )  {
         if ( matrix[INDEX(i,ncol-1,nrow)] > max ) 
                          { max = matrix[INDEX(i,ncol-1,nrow)]; 
                            maxrow = i; 
                            maxcol = ncol-1;}
      }

      /* check the last row of the scoring matrix for the largest value*/

      for ( j = 1 ; j < ncol ; j++ )  {
         if ( matrix[INDEX(nrow-1,j,nrow)] > max ) 
                          { max = matrix[INDEX(nrow-1,j,nrow)]; 
                            maxrow = nrow-1;
                            maxcol = j;} 
      }

      /* If displaying detail information, then print the score matrix */
      if (display == TRACE_ON )  {
          printf("[LCS_SCORE] Score matrix \n");
          displaymatrix(matrix, nrow, ncol);
      }

      /* If the backtracking matrix is available, then print information */
      /* available through the backtracking matrix, including the LCS    */

      if ( backtrack != NULL )  {
          printf("[LCS_TRACE] Backtrack matrix \n");
          displaybacktrack(backtrack, nrow, ncol);
          printf("[LCS_SEQUENCE] Sequence \n");
          LCSdisplay(string1, backtrack, nrow, maxrow, maxcol);
          printf("\n[END_LCS_SEQUENCE]\n");
      }

      /* Clear allocated storage needed to complete the LCS evaluation */
      if ( display == TRACE_ON ) 
         memory_free(backtrack);

      memory_free(matrix);

      /* return the value of the longest common subsequence */
    
      return max;
}

/* -----------END ALGORITHM SPECIFIC METHODS ------------------------------*/

/* -----------BEGIN PARALLEL WORK MANAGEMENT SECTION ----------------------*/

void *perform_parallel_work(void *arg)
/***********************************************************************/
/*                                                                     */
/* purpose: function to perform work using multiple processing elements*/
/*                                                                     */
/* input                                                               */
/* void * arg: pointer to thread specific data and information         */
/*                                                                     */
/* return value: NULL pointer                                          */
/*                                                                     */
/***********************************************************************/
{
    /* get the reference to data to pass to the parallel PE */

    parallel_data *p = (parallel_data *) arg;

    /* transfer the data passed to the processing element to local values */

    int pe_id                    = p->pe_id;
    int num_pes                  = p->num_pes;

    int num_query_sequence       = p->num_query_sequence;
    char *query_sequence         = p->query_sequence;
    int *query_sequence_start    = p->query_sequence_start;
    int *query_sequence_length   = p->query_sequence_length;

    char *query_header           = p->query_header;
    int *query_header_start      = p->query_header_start;
    int *query_header_length     = p->query_header_length;

    int i, j;                     /* index variables              */
    int lcs_val;                  /* accumulated value of lcs     */
    int target_sequence;          /* target sequence to be displayed */


    int *scores;  /* list of scores for each sequence            */
    int *index;   /* ordered index of hits per compared sequence */

    /*                                                            */
    /* Allocate local storage for use in building list of scores  */
    /*                                                            */

    scores = int_memory_allocate(num_query_sequence);
    index  = int_memory_allocate(num_query_sequence);

    /* initialize comparison scores storage matrices */
    for ( i = 0 ; i < num_query_sequence ; i++) {
       scores[i] = 0;
    }

    /* Algorithmic overview:                                       */
    /* Loop through all query sequences                            */
    /*   Loop in parallel through all target sequences             */
    /*       Compare query with unique target per PE               */
    /*   Recover all target scores for a given query               */
    /*   onto master node.                                         */
    /*   Create sorted index of scores                             */
    /*   Display results in hit score order                        */
    /* Continue with next query sequence                           */

    for ( i = 0 ; i < num_query_sequence; i++  )  {
 

    /* sychronize processing elements before starting a parallel task section */

      fflush(NULL); /* ensure all output buffers are flushed */

      PARALLEL_barrier();


      if ( PARALLEL_master() ) 
        printf("[BEGIN_ACTION] Find LCS between sequences for query \n", i);


      /* initialize auxilliary storage matrices */
      for ( j = 0 ; j < num_query_sequence ; j++) {
          scores[j] = 0;
      }

      /* evenly distribute the workload across processing elements */
      /* by assigning specific values of j to each processing element  */

      for ( j = 0 + pe_id; j < num_query_sequence ; j+= num_pes ) 
        {

         /* Compute LCS score */
         /* This section is the actual method that compares the two */
         /* sequences using the LCS algorithm.                      */
         /* Different comparison algorithms can be substituted      */
         /* as desired for different score results                  */


         scores[j] = computeLCS(&query_sequence[query_sequence_start[i]],
                                 query_sequence_length[i],
                                &query_sequence[query_sequence_start[j]],
                                 query_sequence_length[j],TRACE_OFF);

         /* this next line provides per PE detail of the results */
         printf("[%d] scores for %d vs %d = %d \n",pe_id,i,j,scores[j]);
        }

      fflush(NULL); /* ensure all output buffers are flushed */

      PARALLEL_barrier(); /* wait for processing elements all catch */

      fflush(NULL); /* ensure all output buffers are flushed */

      if ( PARALLEL_master() ) 
        printf("[END_ACTION]\n");

      /* accumulate the scores analyzed for each sequence using   */ 
      /* a reduction process that accumlates the maximum for each */
      /* position in the array. The index array is used as a      */
      /* temporary storage area for this process.                 */
      /* This process works because all values for index have     */
      /* been initialized to zero and therefore the comparison    */
      /* scores will be greater than or equal to the zero value   */

      MPI_Reduce(scores, index, num_query_sequence, MPI_INT, 
                   MPI_MAX, 0, MPI_COMM_WORLD);

      /* save the maximum scores for each compared sequence back */
      /* in the best score list                                  */

      for ( j = 0 ; j < num_query_sequence ; j++ ) 
              scores[j] = index[j];

      /* Master processing element prints out the details for the hits */
      /* preventing multiple copies in the output                      */

      if ( PARALLEL_master() ) {

         /* sort the list to create order by size of score */
         /* note: bubble sort is used for its simplicity and not efficiency */
 
         ibubble_sort(index, scores, num_query_sequence);

         /* print a summary of the hits and scores with query header*/

         printf("[QUERY SCORE SUMMARY]\n");
         printf("[QUERY HEADER] Sequence %d Header: \n", i);
         displaystring(&query_header[query_header_start[i]],
                      query_header_length[i]);
         printf("HIT #\t Score\tDescription\n"); 

         /* print out summary for each query in best score order */

         for ( j = 0 ; j < num_query_sequence ; j++ ) {
             target_sequence=index[j];
             printf("%d\t%d\t",j,scores[target_sequence]);
             displaystring(&query_header[query_header_start[target_sequence]],
                       query_header_length[target_sequence]);
         }

         /* print the details of the hits and scores with the sequences lists */

         printf("[QUERY SCORE DETAIL]\n");
         printf("[QUERY HEADER] Sequence %d Header: \n", i);
         displaystring(&query_header[query_header_start[i]],
                        query_header_length[i]);
         printf("\n");
         printf("[QUERY SEQUENCE] Sequence %d Sequence: \n",i);
         displaystring(&query_sequence[query_sequence_start[i]],
                      query_sequence_length[i]);
         printf("\n");
         for ( j = 0 ; j < num_query_sequence ; j++ ) {
              target_sequence = index[j];
              printf("[TARGET HEADER] Hit %d Header: \n",j);
              displaystring(&query_header[query_header_start[target_sequence]],
                       query_header_length[target_sequence]);
              printf("\n");
              printf("[TARGET SEQUENCE] Hit %d Sequence: \n",j);
              displaystring(&query_sequence[query_sequence_start[target_sequence]],
                       query_sequence_length[target_sequence]);
              printf("\n");
              printf("[LCS_SCORE] Sequence %d x %d LCS score %d \n",i,
                      target_sequence,scores[target_sequence]);
           }

        }

    /* sychronize processing elements before starting a parallel task section */
    /* all processing elements wait for the master to join */

      fflush(NULL); /* ensure all output buffers are flushed */

      PARALLEL_barrier();

     }
    /* Clean up memory allocated by this processing element for  */
    /* this evaluation */

     memory_free(scores);
     memory_free(index);
    
     return (NULL);
}
/* ----------- END PARALLEL WORK MANAGEMENT SECTION ----------------------*/


int main()
{
   /* STEP 1: Declare variables and storage needed by the application    */

   /* application specific declarations */

   int n_seq;                           /* number of number of sequences */
   int len_header;                      /* length of the header data     */
   int len_sequence;                    /* length of the sequence data   */

   char *headers;                       /* header data                   */
   int *header_start;                   /* header starting offsets       */
   int *header_length;                  /* header lengths                */

   char *sequences;                     /* sequence data                 */
   int *sequence_start;                 /* sequence starting offsets     */
   int *sequence_length;                /* sequence lengths              */

   /* parallel execution specific constructs and parameters              */

   parallel_data *pe_data;              /* loc of parallel element data  */

   char filename[] = "fileName.txt";


   /* STEP 2: Prepare the parallel execution environment */

   MPI_Init(0,0);


   /* STEP 3: Read in data AND distribute to all processing elements   */

   /* Strategy: Read on master PE and distribute to all other PEs      */

   /* Find the size parameters of the input FASTA file                 */

   /* One processing element handles the input to the evaluation       */
   /* in this implemenation.                                           */

   if ( PARALLEL_master() ) {
      probeFASTA(filename,&n_seq, &len_header, &len_sequence);
   }

   /* Share the size parameters with the other PEs so they can allocate*/
   /* sufficient space to receive the incoming input data              */

   MPI_Bcast(&n_seq,1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&len_header,1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&len_sequence,1, MPI_INT, 0, MPI_COMM_WORLD);

   /* Locally allocate storage for FASTA sequence information          */
   /* to be read or received                                           */

   header_start     = int_memory_allocate (n_seq);
   header_length    = int_memory_allocate (n_seq);
   sequence_start   = int_memory_allocate (n_seq);
   sequence_length  = int_memory_allocate (n_seq);
   headers          = char_memory_allocate (len_header+1);
   sequences        = char_memory_allocate (len_sequence);

   /* Read the FASTA file into computer memory on master process.      */
   /* Print information about the file read to the output file for     */
   /* later reference.                                                 */

   if ( PARALLEL_master() )  {

      readFASTA(filename, &n_seq, sequence_length, 
              sequence_start, header_length, 
              header_start, headers, sequences);

      printf("[BEGIN INPUT DESCRIPTION]\n");
      printf("[FILENAME] %s\n",filename);
      printf("[LCS_SEQUENCE_COUNT] Number of sequences %d\n", n_seq);
      printf("[LCS_CHARACTER_COUNT] Number of sequence characters %d\n", 
              len_sequence);
      printf("[LCS_HEADER_COUNT] Number of header characters %d\n", len_header);
      printf("[END INPUT DESCRIPTION]\n");
   }

   /* This implementation uses a 'replicated data' model                   */
   /* for parallel processing.                                             */
   /* While easier to implement and understand, and most similar to a      */
   /* shared memory programming model, it is not as efficient for          */
   /* large files and numbers of processing elements. A 'distributed data' */
   /* model would be more efficient for                                    */
   /* processing large files. Yet another approach would be a              */
   /* 'distributed task' model where tasks are distributed among PEs       */
   /* with the data required for completing the task.                      */
   /* Both of these approaches require more advanced implementations to    */
   /* determine which information is provided to which processing element  */

   /* Broadcast all sequence information to all processing elements        */
   

   MPI_Bcast(sequence_length, n_seq, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(sequence_start,  n_seq, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(header_length,   n_seq, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(header_start,    n_seq, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(headers,         len_header+1, MPI_CHAR, 0, MPI_COMM_WORLD);
   MPI_Bcast(sequences,       len_sequence, MPI_CHAR, 0, MPI_COMM_WORLD);


   /* STEP 4: Prepare information required for concurrent execution       */

   /* Allocate an explicit data context for each processing element.      */
   /* This approach of using a combined structure to pass information     */
   /* to the computation kernel of the application alleviates potential   */
   /* problems resulting from long argument lists.                        */
   /* This approach has the added benefit of clearly identifying the      */
   /* data needed by the parallel task, avoiding dependence on globally   */
   /* scoped variables which lead to potential problems in synchronization*/

   pe_data = (parallel_data *) memory_allocate( sizeof(parallel_data)  );

   /* Construct the data needed by the kernel that will be doing the task */

   /* read environment specific data for number of processing elements and */
   /* the identity of this processing element                              */

   pe_data->num_pes = PARALLEL_pe_count();
   pe_data->pe_id   = PARALLEL_pe_id();


   /* store problem specific data in a structure to pass to the task */
   /* this approach explicitly tracks the information passed to the  */
   /* concurrent task pool without requiring an excessive calling    */
   /* interface.                                                     */

   pe_data->num_query_sequence    = n_seq;
   pe_data->query_sequence        = sequences;
   pe_data->query_sequence_start  = sequence_start;
   pe_data->query_sequence_length = sequence_length;
   pe_data->query_header          = headers;
   pe_data->query_header_start    = header_start;
   pe_data->query_header_length   = header_length;

/* ACTUALLY COMPUTE WORK CONCURRENTLY */

   perform_parallel_work(pe_data);

/* OVERHEAD CLEANUP TO END PARALLEL EXECUTION */

   MPI_Finalize();

   /* Free the context used for communicating the parallel data */
      
   memory_free(pe_data);

/* END REGION OF OVERHEAD TO PARALLEL EXECUTION */

   /* Free memory allocated by the program */

   memory_free(header_start);
   memory_free(header_length);
   memory_free(sequence_start);
   memory_free(sequence_length);
   memory_free(headers);
   memory_free(sequences);

   exit(0);
}
