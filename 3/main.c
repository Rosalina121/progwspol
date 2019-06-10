#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h> 
#include <sys/stat.h> 

struct { 
   int pocz;// poczatek przedzialu
   int kon; // koniec przedzialu 
   int ile; // Ile liczb w przedziale 
} wynik; 

struct { 
   int pocz;  // Początek przedziału 
   int kon;   // Koniec przedziału 
   int numer; // Kolejny numer odcinka 
} odcinek; 

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
	int poczatek, koniec, procesy, przedzial, przedzialOdcinka, odcinki;
	int start=1, stop=koniec;
	int i,j;
	int rd, fdes, res, wr;
	int send;
	int suma = 0;

    unlink("wyjscie");
    unlink("wejscie");
    res = mkfifo("wyjscie", 0666);
    if (res < 0) {
    	perror("mkfifo");
    }

    res = mkfifo("wejscie", 0666);
    if (res < 0) {
    	perror("mkfifo");
    }

	poczatek = atoi(argv[1]);
	koniec = atoi(argv[2]);
	procesy = atoi(argv[3]);
	przedzialOdcinka = atoi(argv[4]);

	przedzial = koniec/procesy;

	odcinki = (przedzial/przedzialOdcinka)*procesy;
	wynik.pocz = poczatek;
	wynik.kon = poczatek+przedzial;

	fdes = open("wejscie", O_RDWR);
	if (fdes < 0) {
		perror("Open");
		exit(0);
	}


    for(i = 0; i < procesy; i++) {
 		if((pid = fork()) == 0) {
			printf("process %d ok\n", i); // do testowania
			execl("./licz", "licz", NULL);
		}
	}

	odcinek.pocz = poczatek;

	for(j = 0; j < odcinki; j++) {
		odcinek.kon = odcinek.pocz+=przedzialOdcinka;
		odcinek.numer = j;
		wr = write(fdes, &odcinek, sizeof(odcinek));
		//printf("%d\n", wr);
		odcinek.pocz += odcinek.kon;
	}

	res = close(fdes);

    time_t tstart, tstop;
    time(&tstart);

	
	printf("Proces macierzysty PID: %d \n", getpid());

	fdes = open("wyjscie", O_RDWR);
	if (fdes < 0) {
		perror("Open");
		exit(0);
	}

	for(i=0; i < procesy; i++) {
		rd = read(fdes, &wynik, sizeof(wynik));
		suma += wynik.ile;
		printf("Proces %d skonczony. Znalazl %d liczb.\n", i, wynik.ile);
	}

	res = close(fdes);

	time(&tstop);
    int czas;
    czas = tstop-tstart;
    printf("Czas: %d sekund\n", czas);

	printf("Liczby pierwsze: %d\n", suma);
	return 0;
}
