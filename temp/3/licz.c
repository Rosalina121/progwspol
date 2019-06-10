#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    mq=mq_open("/wejscie", O_RDWR | O_CREAT , 0660, &attr);
    mq2=mq_open("/wyjscie", O_RDWR | O_CREAT , 0660, &attr);
    printf("kolejki w potomnych ok");
    res = mq_receive(mq, &msg, sizeof(msg), &prior);

    while(1) {
        res = mq_receive(mq, &odcinek, sizeof(odcinek), &prior);
        if (odcinek.pocz == 0 && odcinek.kon == 0) {
          break;
        }
        for(i = odcinek.pocz; i <= odcinek.kon; i++){
          suma+=pierwsza(i);
        }
        msg.ile = suma;
        res = mq_send(mq2, &msg, sizeof(msg), 10);
    } 

    res = mq_close(mq2);
    res = mq_close(mq);

    printf("Koniec procesu potomnego.");
    _exit(numer);
}


