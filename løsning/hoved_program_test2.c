#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define JORDENS_RADIUS 6373
#define PI 3.141592653589793
#define GRAD_TIL_RAD 0.017453292519943295769236907684886
#define ANTAL_ATTRAKTIONER 5
#define ANTAL_KANTER ANTAL_ATTRAKTIONER*(ANTAL_ATTRAKTIONER-1)/2
#define MAX_STRING 30

typedef struct {
  char navn[MAX_STRING];
  char adresse[MAX_STRING]; 
  double lndg, brdg;
  int besoegt;
} attraktion;

typedef struct {
  double ruteLaengde;
  attraktion rute[ANTAL_ATTRAKTIONER];
} naboRute;

void initialiserAttraktioner(attraktion *attraktioner);
void findKortesteNaboRute(attraktion *valgteAttraktioner, attraktion *ruteAttraktioner, double *samletLaengde);
void outputTilFil(attraktion *ruteAttraktioner);
void findNaboRute(attraktion *valgteAttraktioner, attraktion *startAttraktion,  attraktion **tempRute, double *ruteLaengde);
double beregn_dist(attraktion startAttraktion, attraktion slutAttraktion);

int main()
{
  attraktion attraktioner[ANTAL_ATTRAKTIONER];
  attraktion valgteAttraktioner[ANTAL_ATTRAKTIONER];
  attraktion ruteAttraktioner[ANTAL_ATTRAKTIONER+1];
  double samletLaengde;


  initialiserAttraktioner(attraktioner);
  /*udskriv attraktionernes navne*/

  /*bé brugeren vælge mellem dem*/
  /*sæt de valgte attraktioner ind i valgteAttraktioner[]*/


  findKortesteNaboRute(attraktioner, ruteAttraktioner, &samletLaengde);

  /*outputTilFil(ruteAttraktioner);*/

  /*udskriv rækkefølgen på den hurtigste rute på skærmen*/
  /*samt total længde*/

  int i = 0;

  for(i = 0; i < ANTAL_ATTRAKTIONER+1; ++i){
  printf("%s %s\n", attraktioner[i].navn, ruteAttraktioner[i].navn);
  }
  printf("%lf", samletLaengde);
  return 0;
}

void initialiserAttraktioner(attraktion *attraktioner){
    FILE *input_file_pointer;
  int i = 0;
  char nvn[MAX_STRING];
  char adrs[MAX_STRING];
  double lndgrad;
  double brdgrad;

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

void findKortesteNaboRute(attraktion *valgteAttraktioner, attraktion *ruteAttraktioner, double *samletLaengde){
  /* indput er valgteAttraktioner arrayet, og kanter arrayet*/
  /*output er ruteAttraktioner og samletLaengde*/
  double ruteLaengde;
  attraktion *tempRute[ANTAL_ATTRAKTIONER+1];
  *samletLaengde = 100000;

  int i;
  int h;
  int j;
  for (i = 0; i < ANTAL_ATTRAKTIONER; ++i)
  {
      for (h = 0; h < ANTAL_ATTRAKTIONER; ++h)
    {
      valgteAttraktioner[h].besoegt = 0;
    }
    findNaboRute(valgteAttraktioner, &valgteAttraktioner[i], tempRute, &ruteLaengde);
    if(ruteLaengde < *samletLaengde){
      *samletLaengde = ruteLaengde;
      for (j = 0; j < ANTAL_ATTRAKTIONER+1; ++j)
      {
        ruteAttraktioner[j] = *tempRute[j];
      }
    }
  }
}

void findNaboRute(attraktion *valgteAttraktioner, attraktion *startAttraktion, attraktion **tempRute, double *ruteLaengde){
  int i = 0;
  double lavesteLaengde = 10000;
  *ruteLaengde = 0;

  tempRute[i] = startAttraktion;

  for(i = 0; i < ANTAL_ATTRAKTIONER-1; ++i){
    tempRute[i]->besoegt = 1;
    int j = 0;
    for (j = 0; j < ANTAL_ATTRAKTIONER; ++j)
    {
      if(valgteAttraktioner[j].besoegt != 1 && beregn_dist(*tempRute[i], valgteAttraktioner[j]) < lavesteLaengde){
        lavesteLaengde = beregn_dist(*tempRute[i], valgteAttraktioner[j]);
        tempRute[i+1] = &valgteAttraktioner[j];
      }
    }
    *ruteLaengde += lavesteLaengde;
    lavesteLaengde = 10000;
  }
  tempRute[ANTAL_ATTRAKTIONER] = startAttraktion;
  *ruteLaengde += beregn_dist(*tempRute[ANTAL_ATTRAKTIONER-1], *tempRute[ANTAL_ATTRAKTIONER]);
}

int attraktionSoegning(attraktion *rute, attraktion rutePunkt){
  int i;

  for(i = 0; i < ANTAL_ATTRAKTIONER; i++){
    if(strcmp(rute[i].navn, rutePunkt.navn) == 0)
      return 0;
  }
  return 1;
}

double beregn_dist(attraktion startAttraktion, attraktion slutAttraktion)
{

  double forsteDel = pow(sin((slutAttraktion.brdg - startAttraktion.brdg)/2 * GRAD_TIL_RAD),2);
  double andenDel = pow(sin((slutAttraktion.lndg - startAttraktion.lndg)/2 * GRAD_TIL_RAD),2);
  double a = sqrt(forsteDel + cos(startAttraktion.brdg * GRAD_TIL_RAD) * cos(slutAttraktion.brdg * GRAD_TIL_RAD) * andenDel);
  return 2 * JORDENS_RADIUS * asin(a);
}