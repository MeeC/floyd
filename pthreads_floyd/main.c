#include "globals.h"
#include "functions.h"
#include "pfloyd.h"

int main(int argc, char *argv[])
{
	int nodes_num=500,max_distance=200,num_of_threads=4,graph_type=RANDOM,proc_mode=THREAD_PAR,thread_proc_type=HORIZONTAL,print_mode=0;
	char * filename;

	/*default arguments if not given*/
	if(argc<4)
	{
		nodes_num=500;		// default number of nodes
		max_distance=300;	// default maximum distance between two nodes
		num_of_threads=4;	// default number of threads
		filename="test_graph_10.txt";
		graph_type=RANDOM;
		proc_mode=THREAD_PAR;
		thread_proc_type=HORIZONTAL;
	}
	if(argc==6)	// user provides arguments
	{
		nodes_num=atoi(argv[1]);	// user given number of nodes
		if (nodes_num==1)
		{
			graph_type=IN_FILE;
			filename=argv[2];
		}
		else
		{
			graph_type=RANDOM;
			max_distance=atoi(argv[2]);	// user given maximum distance between two nodes
		}
		proc_mode=atoi(argv[3]);
		num_of_threads=atoi(argv[4]);	// user_given number of threads		
		thread_proc_type=atoi(argv[5]);		
	}
	else if(argc==5)	// user provides arguments
	{
		nodes_num=atoi(argv[1]);	// user given number of nodes
		if (nodes_num==1)
		{
			graph_type=IN_FILE;
			filename=argv[2];
		}
		else
		{
			graph_type=RANDOM;
			max_distance=atoi(argv[2]);	// user given maximum distance between two nodes
		}
		proc_mode=atoi(argv[3]);
		num_of_threads=atoi(argv[4]);	// user_given number of threads		
		thread_proc_type=HORIZONTAL;		
	}
	else if(argc==4)	// user provides arguments
	{
		nodes_num=atoi(argv[1]);	// user given number of nodes
		if (nodes_num==1)
		{
			graph_type=IN_FILE;
			filename=argv[2];
		}
		else
		{
			graph_type=RANDOM;
			max_distance=atoi(argv[2]);	// user given maximum distance between two nodes
		}
		proc_mode=atoi(argv[3]);
		num_of_threads=4;	// user_given number of threads		
		thread_proc_type=HORIZONTAL;		
	}

	
	system("clear");
	srand ( (unsigned)time(NULL) );

	if(graph_type==RANDOM)create_random_graph(nodes_num,max_distance);
	else if(graph_type==IN_FILE)nodes_num=create_file_based_graph(filename);

	if(print_mode==1)print_matrix(nodes_num);

	if(proc_mode==SEQUANTIAL)floyd_algorithm(nodes_num);
	else if(proc_mode==THREAD_PAR)p_floyd_algorithm(nodes_num,num_of_threads,thread_proc_type);

	printf("\n");
	if(print_mode==1)print_matrix(nodes_num);


	
	return 0;
}
