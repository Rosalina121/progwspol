#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include "wspolne.h"

/// Ilość procesów
int nproc;
/// Ilość przedziałów
int nprzedz;
/// Przedziały dla każdego z podprocesów
int* przedzial;
/// Nazwa pliku z wynikami
char nazwapliku[] = "wynik.txt";
/// Nazwa kolejki
char nazwakolejki[] = "/MOJAKOLEJKA";
/// Kolejka z danymi do procesów liczących
mqd_t mq;
/// Atrybuty do kolejki komunikatów
struct mq_attr attr;

int init(int argc, char* argv[]) {
    int i, res;
    wspolne_t wspol;
    mq_unlink(nazwakolejki);
    if (argc < 4) {
        // za mało parametrów!
        // ustawiamy domyślne
        nproc = nprzedz = 2;
        przedzial = malloc(3*sizeof(int));
        przedzial[0] = -1;
        przedzial[1] = 5;
        przedzial[2] = 10;
        return 0;
    }
    nproc = atoi(argv[3]);
    nprzedz = atoi(argv[4]);
    // alokujemy pamięć na przedziały
    przedzial = malloc((nprzedz+1)*sizeof(int));
    // zapamiętujemy wartość maksymalną i minimalną
    // o 1 mniejsze, bo zawsze wartość minimalną potem
    // zwiększamy o 1, aby się na siebie nie nachodziły
    // przedziały w podprocesach
    przedzial[0] = atoi(argv[1])-1;
    przedzial[nprzedz] = atoi(argv[2]);
    // liczymy przedziały
    for (i = 1; i < nprzedz ; i++) {
        przedzial[i] = i * przedzial[nprzedz]/nprzedz;
    }
    // teraz zajmujemy się kolejką komunikatów
    attr.mq_msgsize = sizeof(wspol); 
    attr.mq_maxmsg = nprzedz;
    attr.mq_flags = 0;
    mq = mq_open(nazwakolejki, O_RDWR | O_CREAT, 0660, &attr);
    for (i = 0; i < nprzedz ; i++) {
        wspol.poczatek  = przedzial[i];
        wspol.koniec    = przedzial[i+1];
        wspol.wynik     = -1;
        res = mq_send(mq, &wspol, sizeof(wspol), 10);
    }
    return 0;
}

/**
 * Uruchamia proces liczący
 * \param   qname   Nazwa kolejki komunikatów wejściowych.
 */
int runapp(char* qname, char* fname, int i) {
    char nprc[20];
    sprintf(nprc,"%d",i);
    if (fork() == 0) {
        execl("./licz", "licz", qname, fname, nprc, NULL);
    }
}


int main(int argc, char* argv[]){
    FILE* pliczek;
    int i;
    time_t start, finish;
    if (init(argc,argv) != 0)
        return 1;   // błąd w trakcie inicjalizacji
      time(&start);
    for (i = 0;i<nproc;i++) {
        runapp(nazwakolejki,nazwapliku,i);
    }
    for (i = 0;i<nproc;i++) {
        wait(NULL);
    }
    time(&finish);
    printf("%d %d\n",nprzedz,(int) (finish-start));
    mq_close(mq);
    mq_unlink(nazwakolejki);
    return 0;
}
