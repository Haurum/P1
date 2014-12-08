#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define JORDENS_RADIUS 6373
#define PI 3.141592653589793
#define GRAD_TIL_RAD 0.017453292519943295769236907684886
#define ANTAL_ATTRAKTIONER 5
#define ANTAL_KANTER ANTAL_ATTRAKTIONER*(ANTAL_ATTRAKTIONER-1)
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

typedef struct {
  attraktion start;
  attraktion slut;
  double laengde;
} kanter;

void initialiserAttraktioner(attraktion *attraktioner);
void findKortesteNaboRute(attraktion *valgteAttraktioner, attraktion *ruteAttraktioner, double *samletLaengde);
void outputTilFil(attraktion *ruteAttraktioner);
void findNaboRute(attraktion *valgteAttraktioner, attraktion *startAttraktion, kant *kanter, attraktion **tempRute, double *ruteLaengde);
double beregn_dist(attraktion startAttraktion, attraktion slutAttraktion);
void output_liste(attraktion *attraktioner, int antalAttraktioner);

int main()
{
  attraktion attraktioner[ANTAL_ATTRAKTIONER];
  attraktion valgteAttraktioner[ANTAL_ATTRAKTIONER];
  attraktion ruteAttraktioner[ANTAL_ATTRAKTIONER+1];
  kant kanter[ANTAL_KANTER];
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

void findKortesteNaboRute(attraktion *valgteAttraktioner, attraktion *ruteAttraktioner, kant *kanter, double *samletLaengde){
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
    findNaboRute(valgteAttraktioner, &valgteAttraktioner[i], kanter tempRute, &ruteLaengde);
    if(ruteLaengde < *samletLaengde){
      *samletLaengde = ruteLaengde;
      for (j = 0; j < ANTAL_ATTRAKTIONER+1; ++j)
      {
        ruteAttraktioner[j] = *tempRute[j];
      }
    }
  }
}

void findNaboRute(attraktion *valgteAttraktioner, attraktion *startAttraktion, kant *kanter, attraktion **tempRute, double *ruteLaengde){
  int i = 0;
  double lavesteLaengde = 10000;
  *ruteLaengde = 0;

  tempRute[i] = startAttraktion;

  for(i = 0; i < ANTAL_ATTRAKTIONER-1; ++i){
    tempRute[i]->besoegt = 1;
    int j = 0;
    for (j = 0; j < ANTAL_ATTRAKTIONER; ++j)
    {
      if(valgteAttraktioner[j].besoegt != 1 && findDist(*tempRute[i], valgteAttraktioner[j]) < lavesteLaengde){
        lavesteLaengde = findDist(*tempRute[i], valgteAttraktioner[j]);
        tempRute[i+1] = &valgteAttraktioner[j];
      }
    }
    *ruteLaengde += lavesteLaengde;
    lavesteLaengde = 10000;
  }
  tempRute[ANTAL_ATTRAKTIONER] = startAttraktion;
  *ruteLaengde += findDist(*tempRute[ANTAL_ATTRAKTIONER-1], *tempRute[ANTAL_ATTRAKTIONER]);
}

double findDist(attraktion start, attraktion slut, kant *kanter){
  int i;
  for (i = 0; i < ANTAL_KANTER; ++i)
  {
    if(strcmp(kanter[i].start.navn, kanter[i].slut.navn) == 0 && strcmp(kanter[i].slut.navn, kanter[i].slut.navn) == 0){ // Kig på denne funktion Mandag, Mark siger den ikke virker
      return kanter[i].laengde;
    }
  }
  printf("kunne ikke finde passende kant\n"); exit(0);
}

void udregn_kanter(attraktion *attraktioner, kant *kanter)
{
  int i;
  int y;
  int indexTilKanter = 0;
  for (i = 0; i < ANTAL_ATTRAKTIONER; i++)
  {
    for (y = i; y < ANTAL_ATTRAKTIONER; y++)
    {

      if (strcmp(attraktioner[i].navn, attraktioner[y].navn) != 0)
      {
        kant k, j;
        k.start = attraktioner[i];
        j.slut = k.start;
        k.slut = attraktioner[y];
        j.start = k.slut;
        k.laengde = beregn_dist(k.start.brdg, k.start.lndg, k.slut.brdg, k.slut.lndg);
        j.laengde = k.laengde;
        kanter[indexTilKanter] = k;
        kanter[indexTilKanter+1] = j;
        indexTilKanter += 2;
      }
    }
  }
}

double beregn_dist(attraktion startAttraktion, attraktion slutAttraktion)
{

  double forsteDel = pow(sin((slutAttraktion.brdg - startAttraktion.brdg)/2 * GRAD_TIL_RAD),2);
  double andenDel = pow(sin((slutAttraktion.lndg - startAttraktion.lndg)/2 * GRAD_TIL_RAD),2);
  double a = sqrt(forsteDel + cos(startAttraktion.brdg * GRAD_TIL_RAD) * cos(slutAttraktion.brdg * GRAD_TIL_RAD) * andenDel);
  return 2 * JORDENS_RADIUS * asin(a);
}

void output_liste(attraktion *attraktioner, int antalAttraktioner)
{
  FILE *fp;
  fp = fopen("KortesteRute.txt", "w");
  
  if (fp == NULL)
  {
    printf("Kunne ikke finde filen 'KortesteRute.txt'");
    return;
  }

  int i;

  for (i = 0; i < antalAttraktioner; i++)
  {
    fprintf(fp, "%i: %s", i + 1, attraktioner[i].navn);
    if (i > 0)
    {
      double dist = beregn_dist(attraktioner[i].brdg, attraktioner[i].lndg, attraktioner[i-1].brdg, attraktioner[i-1].lndg);
      fprintf(fp, ", distancen mellem attraktionere: %4.2f km\n", dist);
    }
    else 
      fprintf(fp, "\n");
  }
  

}
