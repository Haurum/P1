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
void findKortesteNaboRute(attraktion *valgteAttraktioner, attraktion *ruteAttraktioner, kant *kanter, double *samletLaengde);
void outputTilFil(attraktion *ruteAttraktioner);
void findNaboRute(kant *kanterne, naboRute *naboRuten, attraktion startAttraktion);
double beregn_dist(double lat1, double lon1, double lat2, double lon2);
void udregn_kanter(attraktion *attraktioner, kant *kanter);
int attraktionSoegning(attraktion *rute, attraktion rutePunkt);

int main()
{
  attraktion attraktioner[ANTAL_ATTRAKTIONER];
  /*attraktion valgteAttraktioner[ANTAL_ATTRAKTIONER];*/
  attraktion ruteAttraktioner[ANTAL_ATTRAKTIONER];
  kant kanter[ANTAL_KANTER];
  double samletLaengde;

  strcpy(attraktioner[0].navn, "nul");
  strcpy(attraktioner[1].navn, "et");
  strcpy(attraktioner[2].navn, "to");
  strcpy(attraktioner[3].navn, "tre");
  strcpy(attraktioner[4].navn, "fire");

  attraktioner[0].lndg = 9.875491000000011;
  attraktioner[0].brdg = 57.041256;
  attraktioner[1].lndg = 9.95243800000003;
  attraktioner[1].brdg = 57.036041;
  attraktioner[2].lndg = 9.675491000000011;
  attraktioner[2].brdg = 56.041256;
  attraktioner[3].lndg = 10.005491000000011;
  attraktioner[3].brdg = 56.541256;
  attraktioner[4].lndg = 11.875491000000011;
  attraktioner[4].brdg = 58.041256;


  udregn_kanter(attraktioner, kanter);
  /*initialiserAttraktioner(attraktioner);*/
  /*udskriv attraktionernes navne*/

  /*bé brugeren vælge mellem dem*/
  /*sæt de valgte attraktioner ind i valgteAttraktioner[]*/

  /*udregnKanter(kanter, valgteAttraktioner);*/

  findKortesteNaboRute(attraktioner, ruteAttraktioner, kanter, &samletLaengde);

  /*outputTilFil(ruteAttraktioner);*/

  /*udskriv rækkefølgen på den hurtigste rute på skærmen*/
  /*samt total længde*/

  int i = 0;

  for(i = 0; i < ANTAL_ATTRAKTIONER; ++i){
  printf("%s %s\n", attraktioner[i].navn, ruteAttraktioner[i].navn);
  }
  printf("%lf", samletLaengde);
  return 0;
}

/*void initialiserAttraktioner(attraktion *attraktioner){
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
} */

void findKortesteNaboRute(attraktion *valgteAttraktioner, attraktion *ruteAttraktioner, kant *kanter, double *samletLaengde){
  /* indput er valgteAttraktioner arrayet, og kanter arrayet*/
  /*output er ruteAttraktioner og samletLaengde*/

  *samletLaengde = 100000;

  naboRute nyNaboRute;

  int i;
  int j;
  for (i = 0; i < ANTAL_ATTRAKTIONER; ++i)
  {
    findNaboRute(kanter, &nyNaboRute, valgteAttraktioner[i]);
    if(nyNaboRute.ruteLaengde < *samletLaengde){
      *samletLaengde = nyNaboRute.ruteLaengde;
      for (j = 0; j < ANTAL_ATTRAKTIONER; ++j)
      {
        ruteAttraktioner[j] = nyNaboRute.rute[j];
      }
    }
  }
}

void findNaboRute(kant *kanterne, naboRute *naboRuten, attraktion startAttraktion){
  int i = 0;
  double lavesteLaengde = 10000;
  naboRuten->ruteLaengde = 0;

  naboRuten->rute[i] = startAttraktion;
  for(i = 0; i < ANTAL_ATTRAKTIONER; ++i){
    int j = 0;
    for(j = 0; j < ANTAL_KANTER; ++j){
      if(strcmp(kanterne[j].start.navn, naboRuten->rute[i].navn) == 0){
        if(attraktionSoegning(naboRuten->rute, kanterne[j].slut)){
          double temp = kanterne[j].laengde;
          if(temp < lavesteLaengde){
           lavesteLaengde = temp;
           naboRuten->rute[i+1] = kanterne[j].slut;
          }
        }
      }
      if(strcmp(kanterne[j].slut.navn, naboRuten->rute[i].navn) == 0){
        if(attraktionSoegning(naboRuten->rute, kanterne[j].start)){
          double temp = kanterne[j].laengde;
          if(temp < lavesteLaengde){
            lavesteLaengde = temp;
            naboRuten->rute[i+1] = kanterne[j].start;
          }
        }
      }
    }
    naboRuten->ruteLaengde += lavesteLaengde;
    lavesteLaengde = 10000;
  }
}

int attraktionSoegning(attraktion *rute, attraktion rutePunkt){
  int i;

  for(i = 0; i < ANTAL_ATTRAKTIONER; i++){
    if(strcmp(rute[i].navn, rutePunkt.navn) == 0)
      return 0;
  }
  return 1;
}

double beregn_dist(double lat1, double lon1, double lat2, double lon2)
{

  double forsteDel = pow(sin((lat2 - lat1)/2 * GRAD_TIL_RAD),2);
  double andenDel = pow(sin((lon2 - lon1)/2 * GRAD_TIL_RAD),2);
  double a = sqrt(forsteDel + cos(lat1 * GRAD_TIL_RAD) * cos(lat2 * GRAD_TIL_RAD) * andenDel);
  return 2 * JORDENS_RADIUS * asin(a);
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

      if (attraktioner[i].brdg != attraktioner[y].brdg)
      {
        kant k;
        k.start = attraktioner[i];
        k.slut = attraktioner[y];
        k.laengde = beregn_dist(k.start.brdg, k.start.lndg, k.slut.brdg, k.slut.lndg);
        kanter[indexTilKanter] = k;
        indexTilKanter++;
      }


    }
  }
}