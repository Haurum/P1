void findNaboRute(attraktion *valgteAttraktion, kant *kanterne, naboRute loesning){
	int i = 0;
	int j = 0;
	double lavesteLaengde = 10000;
	naboRute.ruteLaengde = 0;

	naboRute.rute[i] = valgteAttraktion[i];
	for(i = 0; i < ANTAL_ATTRAKTIONER; ++i){
		for(j = 0; j < ANTAL_KANTER; ++j){
			if(kanterne[j].start == naboRute[i]){
				double temp = kanterne.laengde;
				if(temp < lavesteLaengde){
					lavesteLaengde = temp;
					naboRute.rute[i+1] = kanterne[j].slut;
				}
			}
			if(kanterne[j].slut == naboRute[i]){
				double temp = kanterne.laengde;
				if(temp < lavesteLaengde){
					lavesteLaengde = temp;
					naboRute.rute[i+1] = kanterne[j].start;
				}
			}
		}
		naboRute.ruteLaengde += lavesteLaengde;
		lavesteLaengde = 10000;
	}
}