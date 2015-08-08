#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <iostream>
#include <vector>
#include <stack>

#include "BrainFuck.h"

int main(int argc, char **argv)
{
	printf("\nQuickBrainFuck is released under GPL-v3 License\n");
	if (argc >= 2)
	{
		Format formato = Characters;
		if (argc >= 3)
		{
			if (strcmp(argv[2], "-n") == 0)
				formato = Numbers;
			else
				formato = Characters;
		}
		
		BrainFuck* programma = new BrainFuck(argv[1], formato);
		unsigned long istruzioniEseguite = programma->Esegui();
		struct Report statistiche = programma->GeneraReport();
		if (istruzioniEseguite != 0)
		{
			printf("\n\nStatistiche sull' esecuzione del programma:\n");
			printf("Totale Istruzioni caricate: %u\n", statistiche.istruzioniCaricate);
			printf("Totale Istruzioni eseguite: %u\n", statistiche.istruzioniEseguite);
			printf("Operazioni Aritmetiche eseguite: %u\n", statistiche.opAritmetiche);
			printf("Numero di byte utilizzati: %u", statistiche.byteUtilizzati);
		} else {
			switch (statistiche.ultimoErrore)
			{
				case FILE_UNAVAILABLE:
					printf("Impossibile aprire il file specificato");
					break;
				case FILE_EMPTY:
					printf("Il file e' vuoto");
					break;
				case NOT_CORRESPONDING:
					printf("Errore nella corrispondenza fra [ e ]");
					break;
				case NOINSTRUCTIONS:
					printf("Il file non e' vuoto ma non contiene istruzioni eseguibili");
					break;
				default:
					printf("Nessun errore classificato e' stato riscontrato!");
					break;
			}
		}
		delete programma;
	} else {
		printf("Devi specificare un programma da eseguire!");
	}
	printf("\n");
	return EXIT_SUCCESS;
} 
