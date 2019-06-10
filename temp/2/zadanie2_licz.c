#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#define QUEUE_NAME "/pierwsze"

typedef  struct { 
    int nr;        // numer przedzialu i 
    int pocz;      // poczatek zakresu obliczen Zd(i) 
    int kon;       // koniec zakresu obliczen Zg(i) 
    int ile;     // ile liczb pierwszych w przedziale 
} msg_t; 

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
	  int i, res;
	  int poczatek, koniec, numer;
    int suma = 0;
    unsigned int prior;
  msg_t msg;
  mqd_t mq, mq2;
  struct mq_attr attr;

    attr.mq_msgsize = sizeof(msg); 
    attr.mq_maxmsg = 8; 
    attr.mq_flags = 0; 
    mq=mq_open(QUEUE_NAME, O_RDWR | O_CREAT , 0660, &attr);
    mq2=mq_open("/wyjscie", O_RDWR | O_CREAT , 0660, &attr);
    res = mq_receive(mq, &msg, sizeof(msg), &prior);
    
	poczatek = msg.pocz;
	koniec = msg.kon;
	numer = msg.nr;

    for(i = poczatek; i <= koniec; i++) {
            if(pierwsza(i) == 1)
                suma++;
    }
    msg.ile = suma;

  	res = mq_send(mq2, &msg, sizeof(msg), 10);
    res = mq_close(mq2);
    res = mq_close(mq);

    printf("Koniec procesu potomnego.");
    _exit(numer);
}
