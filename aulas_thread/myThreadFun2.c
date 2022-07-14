#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 30

// Let us create a global variable to change it in threads
int g = 0;

// The function to be executed by all threads
void * myThreadFun2(void *vargp)
{
	// Store the value argument passed to this thread
	int *myid = (int *)vargp;

	// Let us create a local variable to observe its changes
	int s = 0;

	// Change static and global variables
	s++; 
	
	// entrar //
	g++;
	// sair //

	// Print the argument, static and global variables
	printf("Thread ID: %d, Local: %d, Global: %d\n", *myid, s, g);
}

int main()
{
	int i;
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];
	
	// Let us create three threads
    for (i = 0; i < NUM_THREADS; i++) {
      printf("IN MAIN: Creating thread %d.\n", i);
      thread_args[i] = i;
      pthread_create(&threads[i], NULL, myThreadFun2, &thread_args[i]);
    }
	pthread_exit(NULL);
	return 0;
}

