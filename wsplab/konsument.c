#include <sys/mman.h> 
#include <fcntl.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 

#define BSIZE     4   // Rozmiar bufora 
#define LSIZE    80   //  Dlugosc linii 

typedef struct { 
    char buf[BSIZE][LSIZE]; 
    int head; 
    int tail; 
    int cnt; 
    sem_t mutex; 
    sem_t empty; 
    sem_t full; 
} bufor_t; 

int main (int argc, char *argv[]) {
	int i, stat, k, pid, size, fd, res;
	int steps = atoi(argv[2]);

	bufor_t *wbuf;
	if (argc != 3) {
		printf("Use: nr_kons no_steps\n");
		_exit(0);
	}

	// Otwarcie pamieci wspoldzielonej
	if((fd=shm_open("bufor", O_RDWR, 0774))==-1) {
		perror("open");
		_exit(-1);
	}

 	// mmap
 	wbuf = (bufor_t *)mmap(0, sizeof(bufor_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
 	if(wbuf == NULL) {perror("map");  _exit(-1); } 

 	for(int i = 0; i < steps; i++) {
 		sem_wait(&(wbuf->full)); 
     	sem_wait(&(wbuf->mutex)); 
     	printf("Konsument - cnt: %d odebrano %s\n",wbuf->cnt, wbuf->buf[wbuf->tail]);  
     	wbuf->cnt--; 
     	wbuf->tail = (wbuf->tail +1) % BSIZE; 
     	sem_post(&(wbuf->mutex)); 
     	sem_post(&(wbuf->empty)); 
     	sleep(1);
 	}

 	pid = wait(&stat); 
 	sem_close(&(wbuf->mutex)); 
 	sem_close(&(wbuf->empty)); 
 	sem_close(&(wbuf->full)); 
 	return 0; 
}