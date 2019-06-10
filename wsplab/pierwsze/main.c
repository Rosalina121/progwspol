#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/mman.h> 
#include <semaphore.h> 
#define SIZE 10


typedef struct {
	int pocz;
	int kon;
	int suma;
} dane_t;

typedef struct {
	int wymiar;
	dane_t dane[SIZE];
} buf_t;

int main(int argc, char *argv[]) {
	int pid, status;
	int poczatek, koniec, procesy, przedzial;
	int start=1, stop=koniec;
	int i,j;
	int rd, fd, res, size;
	int send;
	int suma = 0;
    char arg1[10];
    char arg2[10];
    char arg3[10];
    char arg4[10];
    char arg5[10];
    buf_t *buf;
    int temp_pocz, temp_kon;
	poczatek = atoi(argv[1]);
	koniec = atoi(argv[2]);
	procesy = atoi(argv[3]);

	przedzial = koniec/procesy;

    time_t tstart, tstop;
    time(&tstart);

    // segment pamięci dzielonej 
    // Tworzenie segmentu:
	shm_unlink("bufor");
	if((fd=shm_open("bufor", O_RDWR|O_CREAT, 0774))==-1) {
		perror("open");
		_exit(-1);
	}

	// ftruncate
	size = ftruncate(fd, sizeof(buf_t));
	if (size < 0) {
		perror("trunc");
		_exit(-1);
	}

	// mmap
	buf = (buf_t *)mmap(0, sizeof(buf_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	// procesy
	buf->dane[0].pocz = poczatek; //0
	buf->dane[0].kon = poczatek+przedzial; //500
	temp_pocz = poczatek;
	temp_kon = poczatek+przedzial;
	for(i = 0; i < procesy; i++) {
		if(i != 0) {
			temp_pocz += przedzial;
			temp_kon += przedzial;
			buf->dane[i].pocz = temp_pocz;
			buf->dane[i].kon = temp_kon;
			buf->dane[i].suma = 0;
		}
		if((pid = fork()) == 0) {
			sprintf(arg1, "%d", i);
			execl("./licz", "licz", arg1, NULL);
		}
	}

	printf("Proces macierzysty PID: %d \n", getpid());
	for(i=0; i < procesy; i++) wait(NULL);
	for(i=0; i < procesy; i++) {
		suma += buf->dane[i].suma;
		printf("Proces %d skonczony. Znalazl %d liczb.\n", i, buf->dane[i].suma);
	}

	time(&tstop);
    int czas;
    czas = tstop-tstart;
    printf("Czas: %d sekund\n", czas);

	printf("Liczby pierwsze: %d\n", suma);
	return 0;
}