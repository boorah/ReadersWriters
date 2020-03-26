#include<unistd.h>
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

#define R 5
#define W 5
#define X 5

// Global variables
int shared_var = 0;
int waiting_readers = 0;
int available_readers = 0;

// Mutex
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

// Condition variables
pthread_cond_t c_rd = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_wr = PTHREAD_COND_INITIALIZER;

void *reader(void* args) {
	int val = *((int*) args);
	int j;
	int current_readers;

	for (j = 0; j < X; j++) {
		// Generate wait time between 1s to 3s
		int wait_time = (rand() % 3) + 1;
		
		// Sleep
		usleep(wait_time * 1000000);

		pthread_mutex_lock(&m);
			waiting_readers++;
			while (available_readers == -1) {
				pthread_cond_wait(&c_rd, &m);
			}
			waiting_readers--;
			available_readers++;
			current_readers	 = available_readers;
		pthread_mutex_unlock(&m);
		
		// Print read details
		printf("Reader %d[%d], Value read: %d, Readers present: %d --> R\n", val+1, j, shared_var, current_readers);

		pthread_mutex_lock(&m);
			available_readers--;
			if (available_readers == 0) {
				pthread_cond_signal(&c_wr);
			}
		pthread_mutex_unlock(&m);
	}
}

void *writer(void* args) {
	int val = *((int*) args);
	int j;
	int current_readers;

	for (j = 0; j < X; j++) {
		// Generate wait time between 1s to 3s
		int wait_time = (rand() % 3) + 1;

		// Sleep
		usleep(wait_time * 1000000);

		pthread_mutex_lock(&m);
			while (available_readers != 0) {
				pthread_cond_wait(&c_wr, &m);
			}
			available_readers = -1;
			current_readers = available_readers;
		pthread_mutex_unlock(&m);
		
		// Update value
		shared_var++;
		
		// Print writing details
		printf("Writer %d[%d], Value written: %d, Readers present: %d --> W\n", val+1, j, shared_var, current_readers + 1);

		pthread_mutex_lock(&m);
			available_readers = 0;
			if (waiting_readers > 0) {
				pthread_cond_broadcast(&c_rd);
			}
			else {
				pthread_cond_signal(&c_wr);
			}
		pthread_mutex_unlock(&m);
	}
}

int main() {
        srand(time(NULL));
	pthread_t rid[R];
	pthread_t wid[W];
	int rnum[R];
	int wnum[W];

	int i;

	// Create readers
	for (i = 0; i < R; i++) {
		rnum[i] = i;
		pthread_create(&rid[i], NULL, reader, &rnum[i]);
	}

	// Create writers
	for (i = 0; i < W; i++) {
		wnum[i] = i;
		pthread_create(&wid[i], NULL, writer, &wnum[i]);
	}

	// Join readers
	for (i = 0; i < R; i++) {
		pthread_join(rid[i], NULL);
	}

	// Join writers
	for (i = 0; i < W; i++) {
		pthread_join(wid[i], NULL);
	}

	return 0;
}
