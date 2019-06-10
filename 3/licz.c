#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
	int i, res, fdes, rd;
	int poczatek, koniec, numer;
    int suma = 0;

    fdes = open("wejscie",O_RDWR);  
    if(fdes < 0) {
        perror("Open"); 
        exit(0); 
    } else {
    	printf("fifo %d ok\n", fdes); // do testowania
    }

    do {
        rd = read(fdes, &odcinek, sizeof(odcinek));	// tu coś nie działa
        for(i = odcinek.pocz; i <= odcinek.kon; i++){
        	suma+=pierwsza(i);
        }
        if(rd <= 0) printf("tu\n");
    } while(rd > 0);

    wynik.ile = suma;

    res = close(fdes);

    fdes = open("wyjscie",O_RDWR);
  	if(fdes < 0) {
  		  perror("Open"); 
    	  exit(0); 
  	} 

  	res = write(fdes, &wynik, sizeof(wynik));
    res = close(fdes);

    printf("Koniec procesu potomnego.");
    _exit(1);
}