#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "CuTest.h"

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