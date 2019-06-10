#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char *argv[])
{
	int pid, status;
	int poczatek, koniec, procesy, przedzial;
	int start, stop, i,j;
    char num[10];
    char num2[10];
    char num3[10];

	char nazwa[] = "wynik.txt";
	char *wnazwa = nazwa;

	FILE *plik;
	plik = fopen(wnazwa, "w");
	fclose(plik);


	poczatek = atoi(argv[1]);
	koniec = atoi(argv[2]);
	procesy = atoi(argv[3]);


	przedzial = koniec/procesy;

    time_t tstart, tstop;
    time(&tstart);
	for(i = 0; i < procesy; i++)
	{
		if(i == 0)
		{
			start = 1;
			stop = przedzial;
		}
		else
		{
			start+=przedzial;
			stop+=przedzial;
		}

		if((pid = fork()) == 0)
		{
			sprintf(num, "%d", start);
			sprintf(num2, "%d", stop);
			sprintf(num3, "%d", i);
			execl("./licz", "licz", num, num2, num3, wnazwa, NULL);
		}

	}


	printf("Proces macierzysty PID: %d \n", getpid());

	for(j = 0; j < procesy; j++)
	{
            pid = wait(&status);
            printf("Proces %d zakonczony. Status: %d\n", pid, WEXITSTATUS(status));
	}
	time(&tstop);
    int czas;
    czas = tstop-tstart;
    printf("Czas: %d sekund\n", czas);

    int sumka=0;
    int temp, temp2, temp3;
    plik = fopen(wnazwa, "r");
    for(i = 0; i < procesy; i++)
    {
        fscanf(plik, "%d %d %d\n", &temp, &temp2, &temp3);
        sumka+=temp3;
    }
	fclose(plik);

	printf("Liczby pierwsze: %d\n", sumka);
	return 0;
}
