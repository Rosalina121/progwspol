#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

struct {
	int pocz;
	int kon;
	int ile;
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
	int pid, status, fd[2];
	int poczatek, koniec, procesy, przedzial;
	//int start=1, stop=koniec;
	int i,j;
	int rd;
	int send;
	int suma = 0;
    // char num[10];
    // char num2[10];
    // char num3[10];

	// char nazwa[] = "wynik.txt";
	// char *wnazwa = nazwa;

	// FILE *plik;
	// plik = fopen(wnazwa, "w");
	// fclose(plik);

	pipe(fd);

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
			close(fd[0]);
			//sprintf(num, "%d", start);
			//sprintf(num2, "%d", stop);
			//sprintf(num3, "%d", i);
			//execl("./licz", "licz", num, num2, num3, wnazwa, NULL);
			for(int j = wynik.pocz; j <= wynik.kon; j++) {
            	if(pierwsza(j) == 1)
            	    sum++;
    		}
    		wynik.ile = sum;
    		write(fd[1], &wynik, sizeof(wynik));
    		close(fd[1]);
    		exit(0);
		}
	}


	printf("Proces macierzysty PID: %d \n", getpid());

	// for(j = 0; j < procesy; j++) {
 //            pid = wait(&status);
 //            printf("Proces %d zakonczony. Status: %d\n", pid, WEXITSTATUS(status));
	// }

	close(fd[1]);
	for(i=0; i < procesy; i++) {
		rd = read(fd[0], &wynik, sizeof(wynik));
		suma += wynik.ile;
		printf("Proces %d skonczony. Znalazl %d liczb.\n", i, wynik.ile);
	}
	close(fd[0]);

	time(&tstop);
    int czas;
    czas = tstop-tstart;
    printf("Czas: %d sekund\n", czas);

 //    int temp, temp2, temp3;
 //    plik = fopen(wnazwa, "r");
 //    for(i = 0; i < procesy; i++) {
 //        fscanf(plik, "%d %d %d\n", &temp, &temp2, &temp3);
 //        suma+=temp3;
 //    }
	// fclose(plik);

	printf("Liczby pierwsze: %d\n", suma);
	return 0;
}
