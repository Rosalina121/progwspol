#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

int pierwsza (int n) {
	int i;
	if(n == 0)
		return 0;
	for(i = 2; i*i <= n; i++) {
		if(n%i == 0)
			return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {
	  int i, res, fd, id;
	  int poczatek, koniec, numer;
    int suma = 0;
    buf_t *buf;

    id = atoi(argv[1]);

    // Otwarcie pamieci wspoldzielonej
    if((fd=shm_open("bufor", O_RDWR, 0774))==-1) {
      perror("open");
      _exit(-1);
    }

    // mmap
    buf = (buf_t *)mmap(0, sizeof(buf_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(buf == NULL) {perror("map");  _exit(-1); } 

    //printf("Proces %d: %d - %d - %d\n",id, buf->dane[id].pocz, buf->dane[id].kon, buf->dane[id].suma);
    for(i = buf->dane[id].pocz; i <= buf->dane[id].kon; i++) {
        if(pierwsza(i) == 1)
            buf->dane[id].suma++;
    }
    _exit(id);
}
