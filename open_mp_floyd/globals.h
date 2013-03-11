#ifndef GLOBALS_H
#define GLOBALS_H
	
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>			/* header file for signal handling */
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>			/* header file for time functions*/
#include <sys/time.h>		/* header file for time functions*/
#include <pthread.h>		/* header file for threads*/	
#include <math.h>			/* header file for mathematical functions*/
#include <omp.h>			/* header file for the openMP api*/

#define RANDOM 1
#define IN_FILE 2

#define SEQUANTIAL 1
#define OMP_PAR 2

/* struct containing information about a graph. It includes the size of graph and a	 */
/* point-to-point-to-int variable which is used to eventually point to a dynamically */
/* allocated adjacency matrix which is actually a multi-dimensional array			 */
struct graph
{
	int **nodes;
	int matrix_size;
}g;

/*struct holding secs and microsecs. used by the time functions*/
struct timeval tm;


#endif
