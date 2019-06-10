#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h> 
#include <mqueue.h>

#include <sys/stat.h> 
#define QUEUE_NAME "/pierwsze"

typedef  struct { 
    int nr;        // numer przedzialu i 
    int pocz;      // poczatek zakresu obliczen Zd(i) 
    int kon;       // koniec zakresu obliczen Zg(i) 
    int ile;     // ile liczb pierwszych w przedziale 
} msg_t; 

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
    unsigned int prior;
    msg_t msg;
	mqd_t mq, mq2;
	struct mq_attr attr;

	poczatek = atoi(argv[1]);
	koniec = atoi(argv[2]);
	procesy = atoi(argv[3]);

	przedzial = koniec/procesy;

	msg.pocz = poczatek;
	msg.kon = poczatek+przedzial;
	msg.nr = 0;
    time_t tstart, tstop;
    time(&tstart);

	for(i = 0; i < procesy; i++) {
		if((pid = fork()) == 0) {
			execl("./licz", "licz", NULL);
		}
	}

	// tworzenie kolejki posix i jej wypelnianie
	res = mq_unlink(QUEUE_NAME);

    attr.mq_msgsize = sizeof(msg); 
    attr.mq_maxmsg = 8; 
    attr.mq_flags = 0; 
    mq=mq_open(QUEUE_NAME, O_RDWR | O_CREAT , 0660, &attr );

    res = mq_unlink("/wyjscie");

    attr.mq_msgsize = sizeof(msg); 
    attr.mq_maxmsg = 8; 
    attr.mq_flags = 0; 
    mq2=mq_open("/wyjscie", O_RDWR | O_CREAT , 0660, &attr );


    for(i = 0; i < procesy; i++) {
    	if(i != 0) {
			msg.pocz += przedzial;
			msg.kon += przedzial;
			msg.nr += 1;
		}
    	res = mq_send(mq, &msg, sizeof(msg), 10);
    }

	printf("Proces macierzysty PID: %d \n", getpid());

	for(i=0; i < procesy; i++) {
		rd = mq_receive(mq2, &msg, sizeof(msg), &prior);
		suma += msg.ile;
		printf("Proces %d skonczony. Znalazl %d liczb.\n", i, msg.ile);
	}

	time(&tstop);
    int czas;
    czas = tstop-tstart;
    printf("Czas: %d sekund\n", czas);

	printf("Liczby pierwsze: %d\n", suma);
	return 0;
}
