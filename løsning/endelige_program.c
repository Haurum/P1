#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define KM_PR_LNDGRAD 60.7722
#define KM_PR_BRDGRAD 111.36
#define ANTAL_ATTRAKTIONER 12
#define ANTAL_KANTER ANTAL_ATTRAKTIONER*(ANTAL_ATTRAKTIONER-1)
#define MAX_STRING 50

typedef struct {
  char navn[MAX_STRING];
  char adresse[MAX_STRING]; 
  double kmFraGreenwich, kmFraAekvator;
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
} kant;

typedef struct {
  double x, y;
} vektor;

void initialiserAttraktioner(attraktion *attraktioner);
void udregn_kanter(attraktion *attraktioner, kant *kanter);
double beregn_dist(attraktion startAttraktion, attraktion slutAttraktion);
void valgafAttraktioner(attraktion *attraktioner, attraktion *valgteAttraktioner, int *antalValgteAttraktioner, attraktion *ikkeValgteAttraktioner);
void findKortesteNaboRute(attraktion *valgteAttraktioner, int antalValgteAttraktioner, attraktion *ruteAttraktioner, kant *kanter, double *samletLaengde);
void outputTilFil(attraktion *ruteAttraktioner);
void findNaboRute(attraktion *valgteAttraktioner, int antalValgteAttraktioner, attraktion *startAttraktion, kant *kanter, attraktion **tempRute, double *ruteLaengde);
double findDist(attraktion start, attraktion slut, kant *kanter);
void findEkstraAttraktioner(attraktion *ruteAttraktioner, attraktion *valgteAttraktioner, int *antalValgteAttraktioner, 
                            kant *kanter, attraktion *ikkeValgteAttraktioner, double maxDist, attraktion *ekstraAttraktioner, int *antalEsktraAttraktioner);
void findEkstraAttraktionerFirkant(attraktion startAttraktion, attraktion slutAttraktion, attraktion *valgteAttraktioner, 
                                  int *antalValgteAttraktioner, kant *kanter, attraktion attraktionAtTilfoeje, double maxDist,
                                  attraktion *ekstraAttraktioner, int *antalEsktraAttraktioner);
double prikProdukt(vektor vektor1, vektor vektor2);
void valgAfEkstraAttraktioner(attraktion *valgteAttraktioner, int *antalValgteAttraktioner, attraktion *ekstraAttraktioner, int antalEsktraAttraktioner);




int main()
{
  attraktion attraktioner[ANTAL_ATTRAKTIONER];
  attraktion valgteAttraktioner[ANTAL_ATTRAKTIONER];
  attraktion ikkeValgteAttraktioner[ANTAL_ATTRAKTIONER];
  attraktion ruteAttraktioner[ANTAL_ATTRAKTIONER+1];
  attraktion ekstraAttraktioner[ANTAL_ATTRAKTIONER];
  kant kanter[ANTAL_KANTER];
  double samletLaengde = 0;
  int antalValgteAttraktioner = 0, antalEsktraAttraktioner = 0;


  initialiserAttraktioner(attraktioner);

  udregn_kanter(attraktioner, kanter);

  valgafAttraktioner(attraktioner, valgteAttraktioner, &antalValgteAttraktioner, ikkeValgteAttraktioner);

  findKortesteNaboRute(valgteAttraktioner, antalValgteAttraktioner, ruteAttraktioner, kanter, &samletLaengde);

  findEkstraAttraktioner(ruteAttraktioner, valgteAttraktioner, &antalValgteAttraktioner, kanter, 
                        ikkeValgteAttraktioner, 0.5, ekstraAttraktioner, &antalEsktraAttraktioner);

  if(antalEsktraAttraktioner > 0){
    valgAfEkstraAttraktioner(valgteAttraktioner, &antalValgteAttraktioner, ekstraAttraktioner, antalEsktraAttraktioner);

    findKortesteNaboRute(valgteAttraktioner, antalValgteAttraktioner, ruteAttraktioner, kanter, &samletLaengde);
  }

  /*outputTilFil(ruteAttraktioner);*/

  /*udskriv rækkefølgen på den hurtigste rute på skærmen*/
  /*samt total længde*/

  int i, j;
  printf("din rute:\n");
  for(i = 0; i < antalValgteAttraktioner+1; ++i){
  printf("%s\n", ruteAttraktioner[i].navn);
  }
  printf("%lf\n", samletLaengde);
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
    while(fscanf(input_file_pointer, " %s %s %lf %lf", attraktioner[i].navn, attraktioner[i].adresse, &brdgrad, &lndgrad) == 4){
      attraktioner[i].kmFraGreenwich = lndgrad * KM_PR_LNDGRAD;
      attraktioner[i].kmFraAekvator = brdgrad * KM_PR_BRDGRAD;
      attraktioner[i].besoegt = 0;
      i++;
    }
  }else{
    printf("tom fil\n"); exit(1);
  }
  fclose(input_file_pointer);
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
        k.laengde = beregn_dist(k.start, k.slut);
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
  return sqrt(pow(startAttraktion.kmFraGreenwich - slutAttraktion.kmFraGreenwich, 2) + 
          pow(startAttraktion.kmFraAekvator - slutAttraktion.kmFraAekvator, 2));
}

void valgafAttraktioner(attraktion *attraktioner, attraktion *valgteAttraktioner, int *antalValgteAttraktioner, attraktion *ikkeValgteAttraktioner){
  int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, o = 0, valgt = 0;

  for(i = 0; i < ANTAL_ATTRAKTIONER; i++){
    printf("%d: %s\n", i+1, attraktioner[i].navn);
  }

  printf("vaelg de attraktioner du oensker at se ved at skrive det tilhoerende tal.\n");
  printf("vaelg et tal (svarende til en attraktion) af gangen og tryk enter efter hver indtastet tal\n");
  printf("indtast ikke samme tal 2 gange\n");
  do{
    if(scanf("%d", &k) != 1){
      printf("Fejl i indlaesning. Farvel.\n"); exit(0);
    }
    if(k != 0){
      valgteAttraktioner[j] = attraktioner[k-1];
      j++;
    }
  } while(j < ANTAL_ATTRAKTIONER && k != 0);

  for (l = 0; l < ANTAL_ATTRAKTIONER; ++l)
  {
    valgt = 0;
    for (m = 0; m < j; ++m)
    {
      if(strcmp(attraktioner[l].navn, valgteAttraktioner[m].navn) == 0){
        m = j;
        valgt = 1;
      }
    }
    if(valgt != 1){
      ikkeValgteAttraktioner[n] = attraktioner[l];
      n++;
    }
  }
  *antalValgteAttraktioner = j;
}

void findKortesteNaboRute(attraktion *valgteAttraktioner, int antalValgteAttraktioner, attraktion *ruteAttraktioner, kant *kanter, double *samletLaengde){
  /* indput er valgteAttraktioner arrayet, og kanter arrayet*/
  /*output er ruteAttraktioner og samletLaengde*/
  double ruteLaengde;
  attraktion *tempRute[antalValgteAttraktioner+1];
  *samletLaengde = 100000;

  int i;
  int h;
  int j;
  for (i = 0; i < antalValgteAttraktioner; ++i)
  {
    for (h = 0; h < antalValgteAttraktioner; ++h)
    {
      valgteAttraktioner[h].besoegt = 0;
    }
    findNaboRute(valgteAttraktioner, antalValgteAttraktioner, &valgteAttraktioner[i], kanter, tempRute, &ruteLaengde);
    if(ruteLaengde < *samletLaengde){
      *samletLaengde = ruteLaengde;
      for (j = 0; j < antalValgteAttraktioner+1; ++j)
      {
        ruteAttraktioner[j] = *tempRute[j];
      }
    }
  }
}

void findNaboRute(attraktion *valgteAttraktioner, int antalValgteAttraktioner, attraktion *startAttraktion, kant *kanter, attraktion **tempRute, double *ruteLaengde){
  int i = 0;
  double lavesteLaengde = 10000;
  *ruteLaengde = 0;

  tempRute[i] = startAttraktion;

  for(i = 0; i < antalValgteAttraktioner-1; ++i){ /*da der f.eks. kun er 4 kanter imellem 5 punkter*/
    tempRute[i]->besoegt = 1;
    int j = 0;
    for (j = 0; j < antalValgteAttraktioner; ++j)
    {
      if(valgteAttraktioner[j].besoegt != 1 && findDist(*tempRute[i], valgteAttraktioner[j], kanter) < lavesteLaengde){
        lavesteLaengde = findDist(*tempRute[i], valgteAttraktioner[j], kanter);
        tempRute[i+1] = &valgteAttraktioner[j];
      }
    }
    *ruteLaengde += lavesteLaengde;
    lavesteLaengde = 10000;
  }
  tempRute[antalValgteAttraktioner] = startAttraktion;
  *ruteLaengde += findDist(*tempRute[antalValgteAttraktioner-1], *tempRute[antalValgteAttraktioner], kanter);
}

double findDist(attraktion start, attraktion slut, kant *kanter){
  int i;
  for (i = 0; i < ANTAL_KANTER; ++i)
  {
    if(strcmp(kanter[i].start.navn, start.navn) == 0 && strcmp(kanter[i].slut.navn, slut.navn) == 0){
      return kanter[i].laengde;
    }
  }
  printf("kunne ikke finde passende kant\n"); exit(0);
}

void findEkstraAttraktioner(attraktion *ruteAttraktioner, attraktion *valgteAttraktioner, int *antalValgteAttraktioner, 
                            kant *kanter, attraktion *ikkeValgteAttraktioner, double maxDist, attraktion *ekstraAttraktioner, int *antalEsktraAttraktioner){

  int i, j, antalIkkeValgteAttraktioner = ANTAL_ATTRAKTIONER - *antalValgteAttraktioner;

  for (i = 0; i < *antalValgteAttraktioner; ++i)
  {
    for (j = 0; j < antalIkkeValgteAttraktioner; ++j)
    {

      if(attraktionErTilfoejet(ekstraAttraktioner, *antalEsktraAttraktioner, ikkeValgteAttraktioner[j])){
      }else if(findDist(ruteAttraktioner[i], ikkeValgteAttraktioner[j], kanter) < maxDist || 
         findDist(ruteAttraktioner[i+1], ikkeValgteAttraktioner[j], kanter) < maxDist){
        ekstraAttraktioner[*antalEsktraAttraktioner] = ikkeValgteAttraktioner[j];
        *antalEsktraAttraktioner += 1;
      }else{
        findEkstraAttraktionerFirkant(ruteAttraktioner[i], ruteAttraktioner[i+1], valgteAttraktioner, antalValgteAttraktioner,
                                      kanter, ikkeValgteAttraktioner[j], maxDist, ekstraAttraktioner, antalEsktraAttraktioner);
      }
    }
  }
}

int attraktionErTilfoejet(attraktion *ekstraAttraktioner, int antalEsktraAttraktioner, attraktion attraktionAtTilfoeje){
  int i;
  for (i = 0; i < antalEsktraAttraktioner; ++i)
  {
    if(strcmp(ekstraAttraktioner[i].navn, attraktionAtTilfoeje.navn) == 0){
      return 1;
    }
  }
  return 0;
}

void findEkstraAttraktionerFirkant(attraktion startAttraktion, attraktion slutAttraktion, attraktion *valgteAttraktioner, 
                                  int *antalValgteAttraktioner, kant *kanter, attraktion attraktionAtTilfoeje, double maxDist, 
                                  attraktion *ekstraAttraktioner, int *antalEsktraAttraktioner){
  int i, j;
  double vektorLaengde;
  vektor ruteVektor, ruteVinkelretVektor, ruteEnhedsVinkelretVektor, mainHjoerne, side1Vektor, side2Vektor, punktVektor;

  vektorLaengde = findDist(startAttraktion, slutAttraktion, kanter);
  ruteVektor.x = slutAttraktion.kmFraGreenwich - startAttraktion.kmFraGreenwich;
  ruteVektor.y = slutAttraktion.kmFraAekvator - startAttraktion.kmFraAekvator;
  ruteVinkelretVektor.x = -ruteVektor.y;
  ruteVinkelretVektor.y = ruteVektor.x;
  ruteEnhedsVinkelretVektor.x = ruteVinkelretVektor.x / vektorLaengde;
  ruteEnhedsVinkelretVektor.y = ruteVinkelretVektor.y / vektorLaengde;
  mainHjoerne.x = startAttraktion.kmFraGreenwich + ruteEnhedsVinkelretVektor.x * maxDist;
  mainHjoerne.y = startAttraktion.kmFraAekvator + ruteEnhedsVinkelretVektor.y * maxDist;
  side1Vektor.x = ruteVektor.x;
  side1Vektor.y = ruteVektor.y;
  side2Vektor.x = -2 * ruteEnhedsVinkelretVektor.x * maxDist;
  side2Vektor.y = -2 * ruteEnhedsVinkelretVektor.y * maxDist;

  punktVektor.x = attraktionAtTilfoeje.kmFraGreenwich - mainHjoerne.x;
  punktVektor.y = attraktionAtTilfoeje.kmFraAekvator - mainHjoerne.y;
  if(0 < prikProdukt(punktVektor, side1Vektor) && prikProdukt(punktVektor, side1Vektor) < prikProdukt(side1Vektor, side1Vektor) &&
      0 < prikProdukt(punktVektor, side2Vektor) && prikProdukt(punktVektor, side2Vektor) < prikProdukt(side2Vektor, side2Vektor)){
    printf("%s, %s, %s\n", startAttraktion.navn, slutAttraktion.navn, attraktionAtTilfoeje.navn);
    ekstraAttraktioner[*antalEsktraAttraktioner] = attraktionAtTilfoeje;
    *antalEsktraAttraktioner += 1;
  }
}

double prikProdukt(vektor vektor1, vektor vektor2){
  return (vektor1.x * vektor2.x) + (vektor1.y * vektor2.y);
}

void valgAfEkstraAttraktioner(attraktion *valgteAttraktioner, int *antalValgteAttraktioner, attraktion *ekstraAttraktioner, int antalEsktraAttraktioner){
  int i, j = *antalValgteAttraktioner, k;
  printf("\nvaelg de attraktioner du oensker at tilfoeje til ruten ved at skrive det tilhoerende tal.\n");
  printf("vaelg et tal (svarende til en attraktion) af gangen og tryk enter efter hver indtastet tal\n");
  printf("indtast ikke samme tal 2 gange\n");
  for (i = 0; i < antalEsktraAttraktioner; ++i)
  {
    printf("%d: %s\n", i+1, ekstraAttraktioner[i].navn);
  }

  do{
    if(scanf("%d", &k) != 1){
      printf("Fejl i indlaesning. Farvel.\n"); exit(0);
    }
    if(k != 0){
      valgteAttraktioner[j] = ekstraAttraktioner[k-1];
      j++;
    }
  } while(j < ANTAL_ATTRAKTIONER && k != 0);
  *antalValgteAttraktioner = j;
}