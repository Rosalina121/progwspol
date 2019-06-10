#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pierwsza (int n)
{
	int i;
	if(n == 0)
		return 0;
	for(i = 2; i*i <= n; i++)
	{
		if(n%i == 0)
			return 0;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	int i;
	int poczatek, koniec, numer;
    char *plik;
    int suma = 0;

	poczatek = atoi(argv[1]);
	koniec = atoi(argv[2]);
	numer = atoi(argv[3]);
    plik = argv[4];

    for(i = poczatek; i <= koniec; i++)
    {
            if(pierwsza(i) == 1)
                suma++;
    }
    FILE *sciezka;
    sciezka = fopen("wynik.txt", "a");
    lockf(fileno(sciezka), F_LOCK, -1);
    fseek(sciezka, 0, SEEK_END);
    fprintf(sciezka, "%d %d %d\n", poczatek, koniec, suma);
    lockf(fileno(sciezka), F_ULOCK, -1);
    fclose(sciezka);
    printf("Koniec procesu potomnego.");
    _exit(numer);
}
