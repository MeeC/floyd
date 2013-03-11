#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/*function definitions*/

void allocate_nodes(int num_of_nodes);

void create_random_graph(int num_of_nodes, int max_distance);

int create_file_based_graph(char * filename);

void print_matrix(int num_of_nodes);

int randomness(int range);

int min(int a, int b);

void deallocate_nodes(int num_of_nodes);

double return_time(void);


#endif
