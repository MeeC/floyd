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

#define HORIZONTAL 1
#define VERTICAL 2
#define BOX_SECTION 3

#define RANDOM 1
#define IN_FILE 2

#define SEQUANTIAL 1
#define THREAD_PAR 2


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

/*defines the pthread mutex*/
pthread_mutex_t mutex;

/*defines the pthread barrier*/
pthread_barrier_t bar;

/*struct holding information about a thread*/
typedef struct 
{
	int thread_id;
	int nodes_num;
	int first_node;
	int last_node;
	int proc_type;
	int first_row_node;
	int last_row_node;

}thread_info;

/*define a pointer-to-thread_info-struct. this will be used to point to an array of structs*/
thread_info * pt_info;


#endif
