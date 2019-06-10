#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#define FIFO MyFIFO 

struct { 
   int pocz;// poczatek przedzialu
   int kon; // koniec przedzialu 
   int ile; // Ile liczb w przedziale 
} wynik; 

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
	int pid, status;
	int poczatek, koniec, procesy, przedzial;
	int start=1, stop=koniec;
	int i,j;
	int rd, fdes, res;
	int send;
	int suma = 0;
    char num[10];
    char num2[10];
    char num3[10];

    res = mkfifo("MyFIFO", 0666);
    if (res < 0) {
    	perror("mkfifo");
    }

	poczatek = atoi(argv[1]);
	koniec = atoi(argv[2]);
	procesy = atoi(argv[3]);

	przedzial = koniec/procesy;

	wynik.pocz = poczatek;
	wynik.kon = poczatek+przedzial;

    time_t tstart, tstop;
    time(&tstart);

	for(i = 0; i < procesy; i++) {
		if(i != 0) {
			wynik.pocz+=przedzial;
			wynik.kon+=przedzial;
		}
		if((pid = fork()) == 0) {
			int sum = 0;
			sprintf(num, "%d", wynik.pocz);
			sprintf(num2, "%d", wynik.kon);
			sprintf(num3, "%d", i);
			execl("./licz", "licz", num, num2, num3, NULL);
		}
	}


	printf("Proces macierzysty PID: %d \n", getpid());

	fdes = open("MyFIFO", O_RDWR);
	if (fdes < 0) {
		perror("Open");
		exit(0);
	}

	for(i=0; i < procesy; i++) {
		rd = read(fdes, &wynik, sizeof(wynik));
		suma += wynik.ile;
		printf("Proces %d skonczony. Znalazl %d liczb.\n", i, wynik.ile);
	}

	time(&tstop);
    int czas;
    czas = tstop-tstart;
    printf("Czas: %d sekund\n", czas);

	printf("Liczby pierwsze: %d\n", suma);
	return 0;
}
