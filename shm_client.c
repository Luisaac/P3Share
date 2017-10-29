#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
// ADD NECESSARY HEADERS
#define SHM_NAME "zhenda_renjie"
#define PAGESIZE 4096


// stats struct
typedef struct {
    int pid;
    char birth[25];
    char clientString[10];
    int elapsed_sec;
    double elapsed_msec;
    int inuse;
    int index;
} stats_t;


// Mutex variables
pthread_mutex_t* mutex;

// Global variables
stats_t* curr;
void* shm_ptr;

void exit_handler(int sig) {
    // ADD

    // critical section begins
	pthread_mutex_lock(mutex);

    // Client leaving; needs to reset its segment   
	memset(shm_ptr+(curr->index*64), 0, 64);
	munmap(shm_ptr, PAGESIZE);
    shm_unlink(SHM_NAME);

	pthread_mutex_unlock(mutex);
	// critical section ends

    exit(0);
}

int main(int argc, char *argv[]) {

	//TODO handle argc != 1 and string too long
	if(argc != 2) {
		fprintf(stderr, "%s\n", "Need one argument.");
		exit(1);
	}

	// timer
	struct timeval t1,t2;
	gettimeofday(&t1, NULL);

	//Signals
	struct sigaction act;
	act.sa_handler = exit_handler;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);

	// ADD    
	int shm_fd = shm_open(SHM_NAME, O_RDWR, 0660);
	if (shm_fd == -1) {
		fprintf(stderr, "%s\n", "Open shared memory error.");
		exit(1);
	}
	shm_ptr = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);		
	if (shm_ptr == MAP_FAILED) {
		fprintf(stderr, "%s\n", "Open shared memory error.");
		exit(1);
	}
	mutex = (pthread_mutex_t*) shm_ptr;
    // critical section begins
    pthread_mutex_lock(mutex);
        
	// client updates available segment
   
   	int isMax = 1;
	for (int i = 1; i < 64; i++) {
		curr = (stats_t*)(shm_ptr+(i*64));
		if (curr->inuse == 0) {
			gettimeofday(&t2, NULL);
			curr->elapsed_sec = t2.tv_sec - t1.tv_sec;
			curr->elapsed_msec = (t2.tv_usec - t1.tv_usec)/1000;
			curr->index = i;
			curr->inuse = 1;
			curr->pid = getpid();
			time_t t = time(NULL);
			strncpy(curr->birth, ctime(&t), strlen(ctime(&t))-1);
			strcpy(curr->clientString, argv[1]);
			isMax = 0;
			break;
		}
	}
	pthread_mutex_unlock(mutex);
	if (isMax) {
		fprintf(stderr, "%s\n", "maxClient limitation.");
		exit(1);
	}
    // critical section ends



        
	while (1) {
        
		// ADD
        //TODO update elapsed time
        gettimeofday(&t2, NULL);
		curr->elapsed_sec = t2.tv_sec - t1.tv_sec;
		curr->elapsed_msec = (t2.tv_usec - t1.tv_usec)/1000;

  

		// Print active clients
		printf("Active clients :");
       	for (int i = 1; i < 64; i++) {
			stats_t* itr = (stats_t*)(shm_ptr+(i*64));
			if (itr->inuse == 1) {
				printf(" %d", itr->pid);
			}
		}
		printf("\n");
        sleep(1);
    }

    return 0;
}
