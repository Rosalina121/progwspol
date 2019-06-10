#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#define FIFO MyFIFO 

struct { 
   int pocz;// poczatek przedzialu
   int kon; // koniec przedzialu 
   int ile; // Ile liczb w przedziale 
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
	int i, res, fdes;
	int poczatek, koniec, numer;
    int suma = 0;

	poczatek = atoi(argv[1]);
	koniec = atoi(argv[2]);
	numer = atoi(argv[3]);

    for(i = poczatek; i <= koniec; i++) {
            if(pierwsza(i) == 1)
                suma++;
    }
    wynik.ile = suma;

    fdes = open("MyFIFO",O_RDWR);  
  	if(fdes < 0) {
  		perror("Open"); 
    	exit(0); 
  	} 

  	res = write(fdes, &wynik, sizeof(wynik));
    printf("Koniec procesu potomnego.");
    _exit(numer);
}
