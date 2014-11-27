#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

typedef struct {
  double ruteLaengde;
  attraktion rute[ANTAL_ATTRAKTIONER];
} naboRute;

void initialiserAttraktioner(attraktion *attraktioner);
void udregnKanter(kant *kanter, attraktion *valgteAttraktioner);
void findKortesteNaboRute(attraktion *valgteAttraktioner, attraktion *ruteAttraktioner, kant *kanter, double *samletLaengde);
void outputTilFil(attraktion *ruteAttraktioner);
void findNaboRute(attraktion *valgteAttraktioner, kant *kanter, naboRute loesning);

int main()
{
  attraktion attraktioner[ANTAL_ATTRAKTIONER];
  attraktion valgteAttraktioner[ANTAL_ATTRAKTIONER];
  attraktion ruteAttraktioner[ANTAL_ATTRAKTIONER];
  kant kanter[ANTAL_KANTER];
  double samletLaengde;

  initialiserAttraktioner(attraktioner);
  /*udskriv attraktionernes navne*/

  /*bé brugeren vælge mellem dem*/
  /*sæt de valgte attraktioner ind i valgteAttraktioner[]*/

  /*udregnKanter(kanter, valgteAttraktioner);*/

  findKortesteNaboRute(valgteAttraktioner, ruteAttraktioner, kanter, &samletLaengde);

  /*outputTilFil(ruteAttraktioner);*/

  /*udskriv rækkefølgen på den hurtigste rute på skærmen*/
  /*samt total længde*/


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
    while(fscanf(input_file_pointer, " %s %s %lf %lf", attraktioner[i].navn, attraktioner[i].adresse, attraktioner[i].lndg, attraktioner[i].brdg) == 4){
      i++;
    }
  fclose(input_file_pointer);
  }
}

void findKortesteNaboRute(attraktion *valgteAttraktioner, attraktion *ruteAttraktioner, kant *kanter, double *samletLaengde){
  /* indput er valgteAttraktioner arrayet, og kanter arrayet*/
  /*output er ruteAttraktioner og samletLaengde*/

  *samletLaengde = 100000;

  naboRute loesninger[ANTAL_ATTRAKTIONER];

  int i;
  int j;
  for (i = 0; i < ANTAL_ATTRAKTIONER; ++i)
  {
    findNaboRute(valgteAttraktioner, kanter, loesninger[i]);
    if(loesninger[i].ruteLaengde < *samletLaengde){
      *samletLaengde = loesninger[i].ruteLaengde;
      for (j = 0; j < ANTAL_ATTRAKTIONER; ++j)
      {
        ruteAttraktioner[j] = loesninger[i].rute[j];
      }
    }
  }
}