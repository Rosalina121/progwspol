#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <mqueue.h>

typedef  struct { 
    int nr;        // numer przedzialu i 
    int pocz;      // poczatek zakresu obliczen Zd(i) 
    int kon;       // koniec zakresu obliczen Zg(i) 
    int ile;     // ile liczb pierwszych w przedziale 
} msg_t; 

struct { 
   int pocz;  // Początek przedziału 
   int kon;   // Koniec przedziału 
   int numer; // Kolejny numer odcinka 
} odcinek; 

int main(int argc, char *argv[]) {
	int pid, status;
	int poczatek, koniec, procesy, przedzial, przedzialOdcinka, odcinki;
	int start=1, stop=koniec;
	int i,j;
	int rd, fdes, res, wr;
	int send;
	int suma = 0;

	unsigned int prior;
    msg_t msg;
	mqd_t mq, mq2;
	struct mq_attr attr;

	poczatek = atoi(argv[1]);
	koniec = atoi(argv[2]);
	procesy = atoi(argv[3]);
	przedzialOdcinka = atoi(argv[4]);

	przedzial = koniec/procesy;

	odcinki = (przedzial/przedzialOdcinka)*procesy;
	msg.pocz = poczatek;
	msg.kon = poczatek+przedzial;
	msg.nr = 0;

    for(i = 0; i < procesy; i++) {
 		if((pid = fork()) == 0) {
			execl("./licz", "licz", NULL);
		}
	}

	odcinek.pocz = poczatek;

	// POSIX queues
	res = mq_unlink("/wejscie");
	printf("usunieto stara klejke i tworze nowa\n");
    attr.mq_msgsize = sizeof(msg); 
    attr.mq_maxmsg = 8; 
    attr.mq_flags = 0; 
    mq=mq_open("/wejscie", O_RDWR | O_CREAT , 0660, &attr );

    res = mq_unlink("/wyjscie");
	printf("usunieto stara klejke i tworze nowa\n");

    attr.mq_msgsize = sizeof(msg); 
    attr.mq_maxmsg = 8; 
    attr.mq_flags = 0; 
    mq2=mq_open("/wyjscie", O_RDWR | O_CREAT , 0660, &attr );

	for(j = 0; j < odcinki; j++) {
		odcinek.kon = odcinek.pocz+=przedzialOdcinka;
		odcinek.numer = j;
		wr = mq_send(mq, &odcinek, sizeof(odcinek), 10);
		//printf("%d\n", wr);
		odcinek.pocz += odcinek.kon;
	}
	for(i = 0; i<procesy; i++){
		odcinek.pocz = 0;
		odcinek.kon = 0;
		wr = mq_send(mq, &odcinek, sizeof(odcinek), 10);

	}

    time_t tstart, tstop;
    time(&tstart);

	
	printf("Proces macierzysty PID: %d \n", getpid());

	for(i=0; i < procesy; i++) {
		rd = mq_receive(mq2, &msg, sizeof(msg), &prior);
		suma += msg.ile;
		printf("Proces %d skonczony. Znalazl %d liczb.\n", i, msg.ile);
	}

	res = close(fdes);

	time(&tstop);
    int czas;
    czas = tstop-tstart;
    printf("Czas: %d sekund\n", czas);

	printf("Liczby pierwsze: %d\n", suma);
	return 0;
}
