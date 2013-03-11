#ifndef PFLOYD_H
#define PFLOYD_H

/*floyd functions definitions*/

void floyd_algorithm(int num_of_nodes);

void thread_info_setup(int number_of_threads, int matrix_size, int type_of_proccessing);

void p_floyd_func(void * ptr);

void p_floyd_algorithm(int num_of_nodes, int num_of_threads, int type_of_proc);


#endif
