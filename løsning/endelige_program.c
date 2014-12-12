#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define KM_PR_LNDGRAD 60.7722
#define KM_PR_BRDGRAD 111.36
#define ANTAL_ATTRAKTIONER 22
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
void findNaboRute(attraktion *valgteAttraktioner, int antalValgteAttraktioner, attraktion *startAttraktion, kant *kanter, attraktion **tempRute, double *ruteLaengde);
double findDist(attraktion start, attraktion slut, kant *kanter);
void findEkstraAttraktioner(attraktion *ruteAttraktioner, int *antalValgteAttraktioner, 
                            kant *kanter, attraktion *ikkeValgteAttraktioner, double maxDist, attraktion *ekstraAttraktioner, int *antalEkstraAttraktioner);
void findEkstraAttraktionerFirkant(attraktion startAttraktion, attraktion slutAttraktion, kant *kanter, attraktion attraktionAtTilfoeje, double maxDist,
                                  attraktion *ekstraAttraktioner, int *antalEkstraAttraktioner);
double prikProdukt(vektor vektor1, vektor vektor2);
void valgAfEkstraAttraktioner(attraktion *valgteAttraktioner, int *antalValgteAttraktioner, attraktion *ekstraAttraktioner, int antalEkstraAttraktioner);
void aendre_startsted(attraktion *ruten, attraktion nytStartSted, int antalAttraktioner, attraktion *outputRute);
void outputTilFil(attraktion *ruteAttraktioner, int antalValgteAttraktioner);
void kopierFil(FILE *filSkabelon, FILE *filOutput);
int attraktionErTilfoejet(attraktion *ekstraAttraktioner, int antalEkstraAttraktioner, attraktion attraktionAtTilfoeje);

int main()
{
  attraktion attraktioner[ANTAL_ATTRAKTIONER];
  attraktion valgteAttraktioner[ANTAL_ATTRAKTIONER];
  attraktion ikkeValgteAttraktioner[ANTAL_ATTRAKTIONER];
  attraktion ruteAttraktioner[ANTAL_ATTRAKTIONER+1];
  attraktion endeligRute[ANTAL_ATTRAKTIONER+1];
  attraktion ekstraAttraktioner[ANTAL_ATTRAKTIONER];
  kant kanter[ANTAL_KANTER];
  double samletLaengde = 0;
  int antalValgteAttraktioner = 0, antalEkstraAttraktioner = 0;


  initialiserAttraktioner(attraktioner);

  udregn_kanter(attraktioner, kanter);

  valgafAttraktioner(attraktioner, valgteAttraktioner, &antalValgteAttraktioner, ikkeValgteAttraktioner);

  findKortesteNaboRute(valgteAttraktioner, antalValgteAttraktioner, ruteAttraktioner, kanter, &samletLaengde);

  findEkstraAttraktioner(ruteAttraktioner, &antalValgteAttraktioner, kanter, 
                        ikkeValgteAttraktioner, 0.1, ekstraAttraktioner, &antalEkstraAttraktioner);

  if(antalEkstraAttraktioner > 0){
    valgAfEkstraAttraktioner(valgteAttraktioner, &antalValgteAttraktioner, ekstraAttraktioner, antalEkstraAttraktioner);

    findKortesteNaboRute(valgteAttraktioner, antalValgteAttraktioner, ruteAttraktioner, kanter, &samletLaengde);
  }

  aendre_startsted(ruteAttraktioner, valgteAttraktioner[0], antalValgteAttraktioner+1, endeligRute);

  outputTilFil(endeligRute, antalValgteAttraktioner);

  int i;
  printf("\nDin rute:\n");
  for(i = 0; i < antalValgteAttraktioner+1; ++i){
  printf("%s\n", endeligRute[i].navn);
  }
  printf("\nRutens saamlede laengde: %.2lfkm\n", samletLaengde);
  return 0;
}

void initialiserAttraktioner(attraktion *attraktioner){
  FILE *input_file_pointer;
  int i = 0;
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
    printf("kunne ikke aabne fil\n"); exit(1);
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
  int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, valgt = 0, y = 0;
  int opretNy = 1;

  printf("Rutevejledning - A401 - P1\n");
  printf("Dette program vil, ud fra dine oenskede attraktioner, bestemme den korteste rute.\n");
  printf("Du kan se de attraktioner du kan vaelge her:\n");

  int halvdelen = ANTAL_ATTRAKTIONER/2;

  for(i = 0; i < halvdelen; i++){
    printf("%d: %-40s\t \t \t%d: %s\n", i+1, attraktioner[i].navn, i+halvdelen+1, attraktioner[i+halvdelen].navn);
  }

  printf("Vaelg de attraktioner du oensker at se ved at skrive det tilhoerende tal.\n");
  printf("Vaelg et tal (svarende til en attraktion) af gangen og tryk enter efter hver indtastet tal.\n");
  printf("Tast 0, efterfulgt af enter, for at gaa videre. Indtast ikke samme tal 2 gange.\n");
  do{
    opretNy = 1;
    if (scanf("%d", &k) ==  1){
      for (y = 0; y < j; y++)
      {
        if (strcmp(valgteAttraktioner[y].navn, attraktioner[k-1].navn) == 0)
        {
          printf("Du har allerede indtastet denne attraktion. Proev igen.\n");
          opretNy = 0;
        }
      }
      if (k == 0)
        break;
      else if (k > ANTAL_ATTRAKTIONER || k < 0)
        printf("Tallet svarer ikke til en attraktion\n");
      else if (opretNy && (k <= ANTAL_ATTRAKTIONER))
      {
        valgteAttraktioner[j] = attraktioner[k-1];
        printf("Tilfoejet attraktion: %s\n", attraktioner[k-1].navn);
        j++;
      }
    }
    else
    {
      printf("Fejlindtastning - proev igen\n");
      char e[MAX_STRING];
      scanf("%s", e);
      k = 1;
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
  printf("Kunne ikke finde passende kant\n"); exit(0);
}

void findEkstraAttraktioner(attraktion *ruteAttraktioner, int *antalValgteAttraktioner, 
                            kant *kanter, attraktion *ikkeValgteAttraktioner, double maxDist, attraktion *ekstraAttraktioner, int *antalEkstraAttraktioner){

  int i, j, antalIkkeValgteAttraktioner = ANTAL_ATTRAKTIONER - *antalValgteAttraktioner;

  for (i = 0; i < *antalValgteAttraktioner; ++i)
  {
    for (j = 0; j < antalIkkeValgteAttraktioner; ++j)
    {

      if(attraktionErTilfoejet(ekstraAttraktioner, *antalEkstraAttraktioner, ikkeValgteAttraktioner[j])){
      }else if(findDist(ruteAttraktioner[i], ikkeValgteAttraktioner[j], kanter) < maxDist || 
         findDist(ruteAttraktioner[i+1], ikkeValgteAttraktioner[j], kanter) < maxDist){
        ekstraAttraktioner[*antalEkstraAttraktioner] = ikkeValgteAttraktioner[j];
        *antalEkstraAttraktioner += 1;
      }else{
        findEkstraAttraktionerFirkant(ruteAttraktioner[i], ruteAttraktioner[i+1], 
                                      kanter, ikkeValgteAttraktioner[j], maxDist, ekstraAttraktioner, antalEkstraAttraktioner);
      }
    }
  }
}

int attraktionErTilfoejet(attraktion *ekstraAttraktioner, int antalEkstraAttraktioner, attraktion attraktionAtTilfoeje){
  int i;
  for (i = 0; i < antalEkstraAttraktioner; ++i)
  {
    if(strcmp(ekstraAttraktioner[i].navn, attraktionAtTilfoeje.navn) == 0){
      return 1;
    }
  }
  return 0;
}

void findEkstraAttraktionerFirkant(attraktion startAttraktion, attraktion slutAttraktion, kant *kanter, attraktion attraktionAtTilfoeje, double maxDist, 
                                  attraktion *ekstraAttraktioner, int *antalEkstraAttraktioner){
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
    ekstraAttraktioner[*antalEkstraAttraktioner] = attraktionAtTilfoeje;
    *antalEkstraAttraktioner += 1;
  }
}

double prikProdukt(vektor vektor1, vektor vektor2){
  return (vektor1.x * vektor2.x) + (vektor1.y * vektor2.y);
}

void valgAfEkstraAttraktioner(attraktion *valgteAttraktioner, int *antalValgteAttraktioner, attraktion *ekstraAttraktioner, int antalEkstraAttraktioner){
  int i, j = *antalValgteAttraktioner, k, y = 0;
  int opretNy = 1;

  if (antalEkstraAttraktioner == 1)
  {
    printf("\nRuten er blevet fundet. Attraktionen nedenfor, der ligger taet paa din nuvaerende rute, kunne give dig en mere interessant tur.\n");
    printf("Tast 1 og tryk enter for at tage attraktionen med. Tast 0, efterfulgt af enter, for at gaa videre..\n");
  }
  else if(antalEkstraAttraktioner > 1)
  {
    printf("\nRuten er blevet fundet. Attraktionerne nedenfor, der ligger taet paa din nuvaerende rute, kunne give dig en mere interessant tur.\n");
    printf("Vaelg et tal (svarende til en attraktion) af gangen, efterfulgt af enter efter hver indtastet tal.\n");
    printf("Tast 0, efterfulgt af enter, for at gaa videre. Indtast ikke samme tal 2 gange.\n");
  }
  for (i = 0; i < antalEkstraAttraktioner; ++i)
  {
    printf("%d: %s\n", i+1, ekstraAttraktioner[i].navn);
  }

  do{
    opretNy = 1;

    if(scanf("%d", &k) == 1){
      for (y = 0; y < j; y++)
      {
        if (strcmp(valgteAttraktioner[y].navn, ekstraAttraktioner[k-1].navn) == 0)
        {
          printf("Du har allerede indtastet denne attraktion. Proev igen.\n");
          opretNy = 0;
        }
      }
      if (k == 0)
        break;
      else if (k > antalEkstraAttraktioner || k < 0)
        printf("Tallet svarer ikke til en attraktion\n");
      else if (opretNy)
      {
        valgteAttraktioner[j] = ekstraAttraktioner[k-1];
        printf("Tilfoejet attraktion: %s\n", ekstraAttraktioner[k-1].navn);
        j++;
      }
    }
    else
    {
      printf("Fejl i indlaesning  - proev igen\n");
      char e;
      scanf("%c", &e);
      k = 1;
    }
  } while(j < ANTAL_ATTRAKTIONER && k != 0);
  *antalValgteAttraktioner = j;
}

void aendre_startsted(attraktion *ruten, attraktion nytStartSted, int antalAttraktioner, attraktion *outputRute)
{
  int i = 0, startStedIndex = 0;

  for (i = 0; i < antalAttraktioner; i++)
  {
    if (strcmp(ruten[i].navn, nytStartSted.navn) == 0)
      startStedIndex = i;
  }

  for (i = 0; i < antalAttraktioner; i++)
  {

    if (startStedIndex == antalAttraktioner-1)
    {
      startStedIndex = 1;
      outputRute[i] = ruten[0];
    }
    else
    {
      outputRute[i] = ruten[startStedIndex];
      startStedIndex++;
    }
  }
}

void outputTilFil(attraktion *ruteAttraktioner, int antalValgteAttraktioner){
  FILE *filSkabelon, *filOutput;

  kopierFil(filSkabelon, filOutput);

  int i;
  char buf[MAX_STRING];

  filOutput = fopen("output.kml", "r+");

  if(filOutput != NULL){
    while(fscanf(filOutput, " %s", buf) == 1){
      if(strcmp(buf, "<coordinates>") == 0){
        fseek(filOutput, 0, SEEK_CUR);
        fprintf(filOutput, "\n");
        for (i = 0; i < antalValgteAttraktioner+1; ++i)
        {
          fprintf(filOutput, "%f,%f \n", ruteAttraktioner[i].kmFraGreenwich/KM_PR_LNDGRAD, ruteAttraktioner[i].kmFraAekvator/KM_PR_BRDGRAD);
        }
        fprintf(filOutput, "</coordinates>\n");
        fprintf(filOutput, "</LineString>\n");
        fprintf(filOutput, "</Placemark>\n");
        fprintf(filOutput, "</Document>\n");
        fprintf(filOutput, "</kml>\n");
        fseek(filOutput, 0, SEEK_CUR);
      }
    }
  }else{
    printf("kunne ikke aabne fil\n"); exit(1);
  }
}

void kopierFil(FILE *filSkabelon, FILE *filOutput){
  char ch;
  filSkabelon = fopen("outputSkabelon.kml", "r");
  filOutput = fopen("output.kml", "w");

  if(filSkabelon == NULL || filOutput == NULL){
    printf("kunne ikke aabne fil\n"); exit(1);
  }

  while( ( ch = fgetc(filSkabelon) ) != EOF ){
      fputc(ch, filOutput);
  } 
  fclose(filSkabelon);
  fclose(filOutput);
}