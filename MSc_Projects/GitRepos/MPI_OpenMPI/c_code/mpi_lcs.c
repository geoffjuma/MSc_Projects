#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "mpi.h"

#define MAX( a, b ) ( a > b ? a : b )
#define MIN( a, b ) ( a < b ? a : b )

#define BLOCK 1024

/* global variable buffer*/
int *buf[2];

/* the longest common subsequence function*/
int lcs( int len1, int *left, char *s1, int *right, int len2, int *top, char *s2, int *buttom )
{
	int i, j;
	int lastindex, currindex;

	/*  */
	for(i = 0; i < len2; i++) 
        {
		buf[1][i] = top[i];
	}

	for( i = 0; i < len1; i++) {

		/* ¨C¦¸¥u allocate ¨â¦æ¬ö¿ý¤º®e */
		lastindex = (i & 1) ^ 1;
		currindex = (i & 1) ^ 0;

		/* ±N²Ä¤@ column ¿W¥ß¥X¨Ó³B²z */
		if( s1[i] == s2[0] ) {
			if( i == 0 ) {
				buf[currindex][0] = 1;
			} else {
				buf[currindex][0] = left[i - 1] + 1;
			}
		} else {
			buf[currindex][0] = MAX( left[i], buf[lastindex][0]);
		}

		/* ¼Ð·Çªº lcs ­pºâ */
		for( j = 1; j < len2; j++) {
			if( s1[i] == s2[j] ) {
				buf[currindex][j] = buf[lastindex][j - 1] + 1;
			} else {
				buf[currindex][j] = MAX( buf[currindex][j - 1], buf[lastindex][j] );
			}
		}

		/* ­n¶Çµ¹¥kÃäªº */
		right[i] = buf[currindex][len2 - 1];
	}

	/* ­n¶Çµ¹¤U­±ªº */
	for( j = 0; j < len2; j++ ) {
		buttom[j] = buf[currindex][j];
	}

	return buf[currindex][len2 - 1];
}

int myCeil( int x, int y )
{
	int r = x / y;
	if( y * r == x ) {
		return r;
	}
	return r + 1;
}

int main( int argc, char *argv[])
{
	int tasks, iam;
	int len1, len2, len3, block1, block2, blen1, blen2, result, answer = -1;
	int i, j, k, ii, jj, m, n;
	char *s1, *s2, *s3;
	int *left, *top, *right, *buttom;
	MPI_Status mpist;

                char a[]="APBCADCQER"; 
                char b[]="RASBTAUCVE";
                len1 = strlen(a); 
                len2 = strlen(b); 

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &tasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &iam);

	if( iam == 0 ) {
		/* iam == 0 ®ÉÅª¤JÀÉ®×¡A¨Ã§âªø«×»P¤º®e broadcast µ¹¤j®a */

		MPI_Bcast( &len1, 1, MPI_INT, 0, MPI_COMM_WORLD );
		MPI_Bcast( &len2, 1, MPI_INT, 0, MPI_COMM_WORLD );

		MPI_Bcast( a, len1, MPI_CHAR, 0, MPI_COMM_WORLD );
		MPI_Bcast( b, len2, MPI_CHAR, 0, MPI_COMM_WORLD );

	}
      else 
        {
		/* ¨ä¥L¤H±µ broadcast */
		MPI_Bcast( &len1, 1, MPI_INT, 0, MPI_COMM_WORLD );
		MPI_Bcast( &len2, 1, MPI_INT, 0, MPI_COMM_WORLD );

		a = (char *)malloc( len1 );
		b = (char *)malloc( len2 );

		MPI_Bcast( a, len1, MPI_CHAR, 0, MPI_COMM_WORLD );
		MPI_Bcast( b, len2, MPI_CHAR, 0, MPI_COMM_WORLD );
	}

	/* row ¼Æµ¥©ó processor ¼Æ */
	m = tasks;
	block1 = myCeil( len1, m);

	/* ¨C BLOCK ¤Á¤@­Ó column, ¨Ã«OÃÒ column ¤@©w¤ñ row ¦h */
	n = myCeil( len2, BLOCK);
	if( n < m ) 
               {
		n = m;
		block2 = myCeil( len2, n);
	} else {
		block2 = BLOCK;
	}

	left = (int *)malloc( block1 * sizeof(int) );
	right = (int *)malloc(block1 * sizeof(int) );
	top = (int *)malloc( block2 * sizeof(int) );
	buttom = (int *)malloc( block2 * sizeof(int) );

	buf[0] = (int *)malloc( block2 * sizeof(int) );
	buf[1] = (int *)malloc( block2 * sizeof(int) );

	/* ±×¤Áªº®y¼Ð */
	for( j = 0; j < m + n - 1; j++)
            {
		for( i = 0;; i++) 
                      {

			/* ºâ¥X¯u¥¿ªº®y¼Ð */
			if( j < n )
                             {
				ii = i;
				jj = j - i;
			} else {
				ii = j - n + 1 + i;
				jj = n -1 - i;
			}

			if( jj < 0 || ii >= m ) {
				break;
			}

			/* ²Ä ii row µ¹½s¸¹ ii ªº processor °µ */
			if( iam != ii ) {
				continue;
			}

			
			printf( "node %d -> %d, %d\n", iam, ii, jj );
			

			/* ºâ¥X«Ý­pºâ°Ï°ìªº dimension */
			if( ii != m - 1 ) {
				blen1 = block1;
			} else {
				blen1 = len1 - ii * block1;
			}

			if( jj != n - 1 ) {
				blen2 =  block2;
			} else {
				blen2 = len2 - jj * block2;
			}
			
			/* ±µ¦¬¤W¤@ row ¶Ç¨Óªºµ²ªG */
			if( ii == 0 ) {
				bzero( top, blen2 * sizeof(int) );
			} else {
				MPI_Recv( top, blen2, MPI_INT, ii - 1, 0, MPI_COMM_WORLD, &mpist );
			}

			/* ±q¥ªÃä¨Óªºµ²ªG´N¦b¦Û¤v¨­¤W¡Aª½±µ½Æ»s¹L¨Ó´N¦n */
			if( jj == 0 ) {
				bzero( left, blen1 * sizeof(int) );
			} else {
				for( k = 0; k < blen1; k++) {
					left[k] = right[k];
				}
			}

			/* ­pºâ³¡¥÷ªº LCS */
			result = lcs( blen1, left, a + ii * block1, right, blen2, top, b + jj * block2, buttom );

			/* §âµ²ªG°eµ¹¤U¤@ row ªº */
			if( ii != m - 1 ) {
				MPI_Send( buttom, blen2, MPI_INT, ii + 1, 0, MPI_COMM_WORLD);
			}

			/* ¦pªG¬O³Ì«á¤@¶ô«h result ´N¬O³Ì«áªºµª®× */
			if( ii == m - 1 && jj == n - 1 ) {
				answer = result;
			}
		}
	}

	/* ¦L¥Xµª®× */
	if( answer == result) {
		printf( "Answer: %d\n", answer );
	}

	free(left);
	free(right);
	free(top);
	free(buttom);
	free(buf[0]);
	free(buf[1]);
	
	MPI_Finalize();

	return 0;
}
