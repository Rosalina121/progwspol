#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include "wspolne.h"

wspolne_t argument;
struct mq_attr attr;
mqd_t mq;

int czypierwsza(int liczba) {
    int i;
    for (i = 2; i*i <= liczba; i++) {
        if (liczba % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char * argv[], char *envp[]) {
	int poczatek;
	int koniec;
    int i;
    int res, prior;
    char* nrproc;
    char* nazwakolejki;
    int wynik = 0;
	if (argc < 3) {
		printf("Za mało argumentów\n");
		return 1;
	}
    nazwakolejki = argv[1];
    nrproc = argv[3];
    // tworzymy kolejkę
    mq = mq_open(nazwakolejki, O_RDWR | O_NONBLOCK);
    // koniec inicjalizacji
    while (1) {
        // pętla, w której liczymy liczby
        res = mq_receive(mq,&argument,sizeof(argument),&prior);
        if (res == -1)
            break;  // jeśli błąd, to kończymy program
        poczatek = argument.poczatek;
        koniec = argument.koniec;
        //printf("pocz: %d kon: %d\n",poczatek,koniec);
        for (i = poczatek; i <= koniec; i++) {
            wynik += czypierwsza(i);
        }
        // zapisujemy wynik do pliku
    }
       mq_close(mq);
    return 0;
}
