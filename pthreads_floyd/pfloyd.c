#include "globals.h"
#include "functions.h"

/*sequential version of floyd_algorithm. finds the shortest paths in the matrix*/
void floyd_algorithm(int num_of_nodes)
{
	int k,u,v;

	double t1=return_time();

	for(k=0;k<num_of_nodes;k++)
	{
		for(u=0;u<num_of_nodes;u++)
		{
			for(v=0;v<num_of_nodes;v++)
			{
				g.nodes[u][v]=min(g.nodes[u][v], g.nodes[u][k]+g.nodes[k][v]);
			}
		}
	}

	double t2=return_time();
	printf("%.6lf seconds elapsed\n", t2-t1);

}


/*this function is called by every thread.*/
void p_floyd_func(void * ptr)
{
	int k,u,v, col_min, col_max, row_min, row_max;

	/* void pointer is casted into thread_info type pointer and passed to ptinfo pointer						*/
	/* after that ptinfo will be a pointer to stuct so -> will be used to access any struct variables			*/
	/* the contains the information every thread is needed in order to execute the right part of the fuction	*/
	thread_info *ptinfo=(thread_info*)ptr;

	/* Depending on proccessing type and its struct info each thread creates the variables
	 * needed in order to perform a part of the algorithm*/
	if(ptinfo->proc_type==HORIZONTAL)
	{
		col_min=0, col_max=ptinfo->nodes_num; 
		row_min=ptinfo->first_node, row_max=ptinfo->last_node+1;
	}

	else if(ptinfo->proc_type==VERTICAL)
	{
		row_min=0, row_max=ptinfo->nodes_num; 
		col_min=ptinfo->first_node, col_max=ptinfo->last_node+1;

	}
	else if(ptinfo->proc_type==BOX_SECTION)
	{
		row_min=ptinfo->first_row_node, row_max=ptinfo->last_row_node+1; 
		col_min=ptinfo->first_node, col_max=ptinfo->last_node+1;

	}

	/* Each thread runs its own part oft the algorithm*/
	for(k=0;k<ptinfo->nodes_num;k++)
	{
		for(u=row_min;u<row_max;u++)
		{
			for(v=col_min;v<col_max;v++)
			{
				//pthread_mutex_lock(&mutex); 
				g.nodes[u][v]=min(g.nodes[u][v], g.nodes[u][k]+g.nodes[k][v]);
				//pthread_mutex_unlock(&mutex); 
			}
		}
		/*A barrier to keep threads synchronized. Its neccessary because all threads must use the same K.*/
		pthread_barrier_wait(&bar);
	}
}


void thread_info_setup(int number_of_threads, int matrix_size, int type_of_proccessing)
{
	/* check if threads are more than nodes. this application supports threads equal nodes at most */
	if(number_of_threads>matrix_size)number_of_threads=matrix_size;

	/* these variables will be used in order to separete the proccess of floyd in small parts, so 
	 * every thread will know which part it executes. Range is the amount of operations each thread
	 * will execute at least. With rest, the first threads will be loaded with the extra operations	
	 */
	int range=matrix_size/number_of_threads;
	int rest=matrix_size%number_of_threads;
	int r,c; //variables used in box section prccessing

	/*allocate some memory for the thread structs*/
	pt_info=malloc(number_of_threads * (sizeof(thread_info)));
	if(pt_info==NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	/*if we want to use box sections the following algorithm finds how the matrix must split*/
	if(type_of_proccessing==BOX_SECTION)
	{
		int j,k,p=number_of_threads;
		int sub=p;
		/*finds the r and c where Threads=r*c and  |r-c| is minimum*/
		for(j=1;j<p+1;j++)
		{
			for(k=1;k<p+1;k++)
			{
				if(p==(j*k))
				{
					if(abs(j-k)==min(abs(j-k),sub))
					{
						r=j;c=k;sub=abs(j-k);				
					}
				}			
			}
		}
		//printf("r: %d c:%d \n",r,c);

		if(r==p && c==1)type_of_proccessing=HORIZONTAL;//if p is prime number we do hHORIZONTAL proccessing

	}

	/*fill structs with threads information. will be used as function parameter by reference to threads*/
	
	/*this algorithm makes a good distribution of work beetween threads if type of proccessing is not sections*/
	if(type_of_proccessing!=BOX_SECTION)
	{
		int m;
		for(m=0;m<number_of_threads;m++)
		{
			pt_info[m].thread_id=m;
			pt_info[m].nodes_num=matrix_size;
			pt_info[m].proc_type=type_of_proccessing;
		
			if(m==0)pt_info[m].first_node=0;
			else pt_info[m].first_node= pt_info[m-1].last_node+1;

			pt_info[m].last_node=pt_info[m].first_node+range-1;

			/* add an extra operation on the first threads so we have similar distribution of work between threads */
			if(rest!=0)
			{
				pt_info[m].last_node++;
				rest--;
			}
			pt_info[m].first_row_node=0; //not used in this type of proccessing
			pt_info[m].last_row_node=0; //not used in this type of proccessing
					
		}
	}

	/*this algorithm makes a good distribution of work beetween threads if type of proccessing is boc sectors*/
	else if(type_of_proccessing==BOX_SECTION)
	{
		int i,j,m=0;
		for(i=0;i<r;i++)
		{
			for(j=0;j<c;j++)
			{					
				pt_info[m].thread_id=m;
				pt_info[m].nodes_num=matrix_size;
				pt_info[m].proc_type=type_of_proccessing;
				/*if matrix size N is perfectly divided by rows and col we split the matrix easily*/
				if((matrix_size%r==0) && (matrix_size%c==0))
				{
						pt_info[m].first_node=j*(matrix_size/c);
						pt_info[m].last_node=((j+1)*(matrix_size/c))-1;
						pt_info[m].first_row_node=i*(matrix_size/r);
						pt_info[m].last_row_node=((i+1)*(matrix_size/r))-1;				
				}
				/*if matrix size cannot be divided we use some padding in order to fullfil the matrix*/
				else
				{
						pt_info[m].first_node=j*floor(matrix_size/c);
						pt_info[m].last_node=((j+1)*ceil(matrix_size/c))-1;
						pt_info[m].first_row_node=i*floor(matrix_size/r);
						pt_info[m].last_row_node=((i+1)*ceil(matrix_size/r))-1;				
				}
				m++;
			}	
		}
	}
}


/* threads version of floyd_algorithm. finds the shortest paths in the matrix*/
void p_floyd_algorithm(int num_of_nodes, int num_of_threads, int type_of_proc)
{

	thread_info_setup(num_of_threads,num_of_nodes,type_of_proc);

	int i,j;
	pthread_t thread[num_of_threads];//definition of threads
	pthread_mutex_init(&mutex, NULL);
	pthread_barrier_init(&bar,NULL,num_of_threads);

	double t1=return_time();

	/*create the threads with p_floyd_func and pt_info struct as argument*/
	for(i=0;i<num_of_threads;i++)
	{
		pthread_create(&thread[i],NULL, (void*) &p_floyd_func,(void*)&pt_info[i]);
	}
	
	/*join threads when they are finished*/
	for(j=0;j<num_of_threads;j++)
	{
		pthread_join(thread[j], NULL);
	}

	pthread_mutex_destroy(&mutex);

	pthread_barrier_destroy(&bar);

	double t2=return_time();
	printf("Calculation Completed... [%.6lf] \n", t2-t1);

	/*free some memory*/
	deallocate_nodes(num_of_nodes);
	
}


