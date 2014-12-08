#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ANTAL_ATTRAKTIONER 3
#define ANTAL_KANTER ANTAL_ATTRAKTIONER*(ANTAL_ATTRAKTIONER-1)/2
#define MAX_STRING 30



typedef struct {
  char *navn;
  char *adresse; 
  double lndg, brdg;
  int antalKanter;
} attraktion;

typedef struct {
  double laengde;
  attraktion start, slut;
} kant;

void print_attraktioner(attraktion *attraktionsListe);

int main(){
	attraktion al[3];
	al[0].navn = "soeren";
	al[1].navn = "mark";
	al[2].navn = "mikael";

	print_attraktioner(al);	

}

void print_attraktioner(attraktion *attraktionsListe){
	int i;
	for(i = 0; i < ANTAL_ATTRAKTIONER; i++)
		printf("%i: %s\n", i, attraktionsListe[i].navn);
}