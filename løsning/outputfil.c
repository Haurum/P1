#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define JORDENS_RADIUS 6373
#define PI 3.141592653589793
#define GRAD_TIL_RAD 0.017453292519943295769236907684886
#define ANTAL_ATTRAKTIONER 12
#define ANTAL_KANTER ANTAL_ATTRAKTIONER*(ANTAL_ATTRAKTIONER-1)/2
#define MAX_STRING 50

typedef struct {
  char navn[MAX_STRING];
  char adresse[MAX_STRING]; 
  double lndg, brdg;
  int besoegt;
} attraktion;

void initialiserAttraktioner(attraktion *attraktioner);
void valgafAttraktioner(attraktion *attraktioner, attraktion *ruteAttraktioner);

int main(void){
	attraktion attraktioner[MAX_STRING];
	attraktion ruteAttraktioner[MAX_STRING];
	initialiserAttraktioner(attraktioner);
	valgafAttraktioner(attraktioner, ruteAttraktioner);
	return 0;
}

void initialiserAttraktioner(attraktion *attraktioner){
    FILE *input_file_pointer;
  int i = 0;

  input_file_pointer = fopen("attraktioner.txt", "r");

  if(input_file_pointer != NULL){
    while(fscanf(input_file_pointer, " %s %s %lf %lf", attraktioner[i].navn, attraktioner[i].adresse, &attraktioner[i].lndg, &attraktioner[i].brdg) == 4){
      attraktioner[i].besoegt = 0;
      i++;
    }
  }else{
    printf("tom fil\n"); exit(1);
  }
  fclose(input_file_pointer);
} 

void valgafAttraktioner(attraktion *attraktioner, attraktion *ruteAttraktioner){
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int valg[ANTAL_ATTRAKTIONER];

	for(i = 0; i < ANTAL_ATTRAKTIONER; i++){
		printf("%d: %s\n", i+1, attraktioner[i].navn);
		}


	do{
		if(scanf("%d", &k) != 1){
			printf("Fejl i indlaesning. Farvel.\n"); exit(0);
		}
		if(k != 0){
			valg[j] = k;
			j++;
		}
	} while(j < ANTAL_ATTRAKTIONER && k != 0);

	for(l = 0; l < j; l++){
		ruteAttraktioner[l] = attraktioner[valg[l]-1];
		printf("%s\n", ruteAttraktioner[l].navn); 
	}
}