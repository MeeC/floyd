
#include "globals.h"

/*returns a random number on the given range */
int randomness(int range)
{
	int rand_num;

	rand_num = rand()%range +1;

	return rand_num;
}

/*allocates the needed memory to hold the adjacency matrix array*/
void allocate_nodes(int num_of_nodes)
{
	g.nodes = malloc(num_of_nodes * sizeof(int *));
	if(g.nodes == NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	int i;
	for(i = 0; i < num_of_nodes; i++)
	{
		g.nodes[i] = malloc(num_of_nodes * sizeof(int));
		if(g.nodes[i] == NULL)
		{
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
	}
	printf("Allocating memory for nodes.....\t[OK]\n");	
}

/*instantiates the adjacency matrix array with random distances. so it creates a random graph snapshot*/
void create_random_graph(int num_of_nodes, int max_distance)
{
	/*firstly allocate memory for matrix*/
	allocate_nodes(num_of_nodes);
	int i,j;
	/*give random distances to every node connection*/
	for(i=0;i<num_of_nodes;i++)
	{
		for(j=0;j<num_of_nodes;j++)
		{
			/*distance between a node and itself is zero*/
			if(i==j)g.nodes[i][j]=0;
			/*nodes paths by 50% chance are infinite which is defined as a large number */			
			else if(randomness(100)<50)g.nodes[i][j]=99999; 
			/*if a node has a path to an other node their distance is a random number between 1 and max-distance*/
			else g.nodes[i][j]=randomness(max_distance);
		}
	}

}

/* This function instantiates the adjacency matrix array with distances written in a file.
 * The file must have a certain format in order the instantiation to be done correctly 
 * Returns the number of nodes. Information about file format can be found on our report. 
 */
int create_file_based_graph(char * filename)
{	//variable and pointer declarations
	FILE * fp;
	char *token;
	char *buffer;
	int i,j,num_nodes;
	
	/* Allocate some memory for a BIG buffer. The buffer must be big enough to be used in large matrixes*/
	buffer=(char*)malloc(5000 * sizeof(char));

	/* Open the file*/	
	fp =fopen(filename,"r");
		if (!fp)perror ("Error opening file\n");

	printf("Opening File: %s .....\t[OK]\n",filename);
	
	/*Read the file line by line until we run out of lines*/
	i=0;
	while(fgets(buffer,1000,fp)!=NULL)
	{
		/* If last character is the new line char, Replace it with the empty char */
		if(buffer[strlen(buffer)-1]=='\n')buffer[strlen(buffer)-1]=0;

		/* the first line holds the matrix size*/
		if(strlen(buffer)==1)
		{	
			num_nodes=atoi(buffer);
			/*allocate memory for matrix*/
			allocate_nodes(num_nodes);	
		}
		/* Each of the next lines is a matrix row*/
		else
		{	/* Use space char as delimeter in order to tokenize the line*/
			token = strtok(buffer," ");

			/* For each token in line (and for every line eventually) we store the values
			 * as integers in the matrix array */
			for(j=0;token!=NULL;j++)
			{	/* store the infinite char # as number 99999 */
				if(strcmp(token,"#")==0){g.nodes[i][j]=99999;}
				else g.nodes[i][j]=atoi(token);

				/* point to next token*/
				token = strtok(NULL," ");
			}
			if(j!=num_nodes)perror("Error in file matrix: Collumns are not maching matrix size\n");
			i++;
		}
	}
	if(i!=num_nodes)perror("Error in file matrix: Rows are not maching matrix size\n");

	/*close file and deallocate memory*/
	fclose(fp);
	free(buffer);
	
	printf("Graph Created via file.....\t[OK]\nMatrix Size:\t[%d]\n",num_nodes);

	return num_nodes;
}

/*prints the adjacency matrix to std output*/
void print_matrix(int num_of_nodes)
{
	int i,j,m;
	for(i=0;i<num_of_nodes;i++)
	{	
		/*print the orizontal node index above the actual adjacency matrix*/
		if(i==0)
		{
			printf("\t");
			for(m=0;m<num_of_nodes;m++)
			{
				printf("%d\t",m);
			}
			printf("\n");
		}

		for(j=0;j<num_of_nodes;j++)
		{
			if(j==0)printf("%d |\t",i);
			/*infinity is presented as double symbol ##*/
			if(g.nodes[i][j]==99999)printf("#\t");		
			else printf("%d \t",g.nodes[i][j]);
		}
		printf("\n");
	}	

}

/*return the minumun between two integers*/
int min(int a, int b)
{
	if(a<b)return a;
	else if(b<a)return b;
	/*if integers are equal by convention returns the first one*/
	else return a;
}

void deallocate_nodes(int num_of_nodes)
{
	int i;
	for(i = 0; i < num_of_nodes; i++)
	{
		free(g.nodes[i]);
	}
	free(g.nodes);
}

double return_time(void)
{
	gettimeofday(&tm, NULL);
	double time=tm.tv_sec+(tm.tv_usec/1000000.0);
	
	return time;
}



