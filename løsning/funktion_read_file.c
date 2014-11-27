#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "CuTest.h"

#define ANTAL_ATTRAKTIONER 10
#define ANTAL_KANTER ANTAL_ATTRAKTIONER*(ANTAL_ATTRAKTIONER-1)/2
#define MAX_STRING 30

typedef struct {
  char navn[MAX_STRING];
  char adresse[MAX_STRING]; 
  double lndg, brdg;
  int antalKanter;
} attraktion;

typedef struct {
  double laengde;
  attraktion start, slut;
} kant;

void initialiserAttraktioner(attraktion *attraktioner);

int main(void) {
	attraktion attraktioner[ANTAL_ATTRAKTIONER];
	
	initialiserAttraktioner(attraktioner);
	printf("%s %s %lf %lf\n", attraktioner[0].navn, attraktioner[0].adresse, attraktioner[0].lndg, attraktioner[0].brdg);

	return 0;
}

void initialiserAttraktioner(attraktion *attraktioner){
		FILE *input_file_pointer;
	int i = 0;
	char nvn[MAX_STRING];
	char adr[MAX_STRING];
	double lnd;
	double brd;

	input_file_pointer = fopen("attraktioner", "r");

	if(input_file_pointer != NULL){
		while(fscanf(input_file_pointer, " %s %s %lf %lf", nvn, adr, &lnd, &brd) == 4){
			strcpy(attraktioner[i].navn, nvn);
			strcpy(attraktioner[i].adresse, adr);
			attraktioner[i].lndg = lnd;
			attraktioner[i].brdg = brd;
			i++;
		}
	fclose(input_file_pointer);
	}
}