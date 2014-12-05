#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define JORDENS_RADIUS 6373
#define PI 3.141592653589793
#define GRAD_TIL_RAD 0.017453292519943295769236907684886
#define ANTAL_ATTRAKTIONER 5
#define ANTAL_KANTER (ANTAL_ATTRAKTIONER*(ANTAL_ATTRAKTIONER-1)/2)
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

double beregn_dist(double lat1, double lon1, double lat2, double lon2);
void udregn_kanter(attraktion *attraktioner, kant *kanter);
void output_liste(attraktion *attraktioner, int antalAttraktioner);


int main()
{
	kant kanter[ANTAL_KANTER];
	attraktion attraktioner[ANTAL_ATTRAKTIONER];
	attraktioner[0].lndg = 9.875491000000011;
	attraktioner[0].brdg = 57.041256;
  strcpy(attraktioner[0].navn, "Nummer et er her");
	attraktioner[1].lndg = 9.95243800000003;
	attraktioner[1].brdg = 57.036041;
  strcpy(attraktioner[1].navn, "Nummer to er her");
	attraktioner[2].lndg = 9.675491000000011;
	attraktioner[2].brdg = 56.041256;
  strcpy(attraktioner[2].navn, "Nummer tre er her");
	attraktioner[3].lndg = 10.005491000000011;
	attraktioner[3].brdg = 56.541256;
  strcpy(attraktioner[3].navn, "Nummer fire er her");
	attraktioner[4].lndg = 11.875491000000011;
	attraktioner[4].brdg = 58.041256;
  strcpy(attraktioner[4].navn, "Nummer fem er her");

	udregn_kanter(attraktioner, kanter);
	printf("antal kanter %i\n", ANTAL_KANTER);
	int i;
	for (i = 0; i < ANTAL_KANTER; i++)
	{
		printf("%f\n", kanter[i].laengde);
	}

	double lat1 = (57.041256), lon1 = (9.875491000000011),  lat2 = (57.036041), lon2 = (9.95243800000003);
	double dist = beregn_dist(lat1, lon1, lat2, lon2);
	printf("%f\n", dist);

  output_liste(attraktioner, 5);
	return 0;
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
    fprintf(fp, "%i: %s\n", i + 1, attraktioner[i].navn);

}

