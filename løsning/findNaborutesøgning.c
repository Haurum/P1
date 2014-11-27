void findNaboRute(attraktion *valgteAttraktion, kant *kanterne, naboRute naboRuten, attration startAttraktion){
  int i = 0;
  double lavesteLaengde = 10000;
  naboRuten.ruteLaengde = 0;

  naboRuten.rute[i] = startAttraktion;
  for(i = 0; i < ANTAL_ATTRAKTIONER; ++i){
    int j = 0;
    for(j = 0; j < ANTAL_KANTER; ++j){
      if(kanterne[j].start.navn == naboRuten.rute[i].navn){
        if(attraktionSøgning(naboRuten.rute, kanterne[j].slut)){
          double temp = kanterne[j].laengde;
          if(temp < lavesteLaengde){
           lavesteLaengde = temp;
           naboRuten.rute[i+1] = kanterne[j].slut;
          }
        }
      }
      if(kanterne[j].slut.navn == naboRuten.rute[i].navn){
        if(attraktionSøgning(naboRuten.rute, kanterne[j].start)){
          double temp = kanterne[j].laengde;
          if(temp < lavesteLaengde){
            lavesteLaengde = temp;
            naboRuten.rute[i+1] = kanterne[j].start;
          }
        }
      }
    }
    naboRuten.ruteLaengde += lavesteLaengde;
    lavesteLaengde = 10000;
  }
}

int attraktionSøgning(attraktion *rute, attraktion rutePunkt){
  int i;

  for(i = 0; i < ANTAL_ATTRAKTIONER; i++){
    if(rute[i].navn == rutePunkt.navn)
      return 0;
  }
  return 1;
}