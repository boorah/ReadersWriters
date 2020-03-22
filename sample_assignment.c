#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>

#define RNUM 5
#define WNUM 5
#define TIME 5

int shared_var = 1;
int available_readers = 0;
int available_writers = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t curr_read = PTHREAD_COND_INITIALIZER;
pthread_cond_t curr_write = PTHREAD_COND_INITIALIZER;

void *reader_func(void *arg) {
	int i;
	int wait_time;
	int val = *((int*) arg);
//	pthread_mutex_lock(&m);
//	shared_var += 1;
	for (i = 0; i < TIME; i++) {
	    pthread_mutex_lock(&m);
		 wait_time = (rand() % 3) + 1;
		 usleep(wait_time * 1000000);

		

	   	 available_readers += 1;
 		 printf("\nReader %d[%d] entered! Value read: %d, Wait time: %ds, Available readers: %d, Available writers: %d\n",val, i+1, shared_var, wait_time, available_readers, available_writers);
	    pthread_mutex_unlock(&m);
	   
            pthread_mutex_lock(&m);
	    	available_readers -= 1;
		printf("\nReader %d[%d] quitting!\n", val, i+1);

		if (available_readers == 0) {
			pthread_cond_signal(&curr_write);
		}
	    pthread_mutex_unlock(&m);
	}
//	pthread_mutex_unlock(&m);
}

void *writer_func(void *arg) {
	int i;
	int wait_time;
	int val = *((int*) arg);
//	pthread_mutex_lock(&m);
	for (i = 0; i < TIME; i++) {
	    pthread_mutex_lock(&m);
		wait_time = (rand() % 3) + 1;
		usleep(wait_time * 1000000);

		while (available_readers != 0) {
			pthread_cond_wait(&curr_write, &m);
		}

	    	shared_var += 1;
	    	available_writers += 1;
		printf("\nWriter %d[%d] entered! Value written: %d, Wait time: %ds, Available readers: %d, Available writers: %d\n",val, i+1, shared_var, wait_time, available_readers, available_writers);
	    pthread_mutex_unlock(&m);
	    
	    pthread_mutex_lock(&m);
	    	available_writers -= 1;
		printf("\nWriter %d[%d] quitting!\n", val, i+1);
	    pthread_mutex_unlock(&m);

	}
//	pthread_mutex_unlock(&m);
}

int main() {
	srand(time(NULL));
	int i;
	pthread_t rid[RNUM];
	pthread_t wid[WNUM];

	int rno[RNUM];
	int wno[WNUM];

	// Create reader threads
	for (i = 0; i < RNUM; i++) {
		rno[i] = i;
		pthread_create(&rid[i], NULL, reader_func, &rno[i]);
	}

	// Create writer threads
	for (i = 0; i < WNUM; i++) {
		wno[i] = i;
		pthread_create(&wid[i], NULL, writer_func, &wno[i]);
	}

	// Join reader threads
	for (i = 0; i < RNUM; i++) {
		pthread_join(rid[i], NULL);
	}
	
	// Join writer threads
	for (i = 0; i < WNUM; i++) {
		pthread_join(wid[i], NULL);
	}

	printf("\nValue: %d\nAvailable readers: %d\nAvailable writers: %d\n", shared_var, available_readers, available_writers);
	return 0;
}
