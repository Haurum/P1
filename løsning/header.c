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
  char rute[ANTAL_ATTRAKTIONER][MAX_STRING];
} naboRute;