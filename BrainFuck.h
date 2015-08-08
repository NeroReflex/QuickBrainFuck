#ifndef __BRAINFUCK__
#define __BRAINFUCK__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

#define NO_ERRORS -1
#define FILE_UNAVAILABLE 0
#define FILE_EMPTY 1
#define NOT_CORRESPONDING 2
#define NOINSTRUCTIONS 3

enum Format
{
	Numbers = 0x00, Characters = 0x01
};

struct Byte
{
	struct Byte *Next;
	struct Byte *Previous;
	unsigned char Data;
};

struct Report
{
	unsigned long opAritmetiche; //numero di istruzioni + e - eseguite
	unsigned long istruzioniCaricate; //numero di istruzioni caricate
	unsigned long byteUtilizzati; //numero di byte utilizzati
	unsigned long istruzioniEseguite; //numero di istruzioni già eseguite
	int ultimoErrore; //ultimo errore riscontrato
	bool terminato; //il programma è gia terminato?
};

class BrainFuck
{
	public:
		BrainFuck(char*, Format); //costruttore di classe. Richiere il percorso di un programma e il formato (unico) per le operazioni di input ed output
		~BrainFuck(); //pulisci la memoria allocata per l' esecuzione del programma (elimina dalla memoria i byte utilizzati e le istruzioni caricate)
		unsigned long Esegui(); //esegue l' intero programma e stampa a video un report
		bool EseguiIstruzione(); //esegue l' istruzione attuale (si parte dalla prima) e prepara l' esecuzione dell' istruzione successiva
		struct Report GeneraReport(); //genera un report sullo stato attuale del programma
		
	private:
		unsigned long numeroIstruzioneAttuale; //istruzione da eseguire
		unsigned long totaleIstruzioniEseguite; //istruzioni eseguite
		unsigned long numeroOperazioniAritmetiche; //istruzioni aritmetiche eseguite (incrementi e decrementi)
		unsigned long numeroIstruzioniCaricate; //numero di istruzioni pervenute all interno del file
		vector<unsigned short> istruzioni; //istruzioni del programma
		unsigned long bytesUsati; //numero di byte utilizzati dal programma
		stack <unsigned long> Cicli; //stack per la gestione degli '[' e ']'
		struct Byte *Nastro; //il nastro "infinito"
		bool valido; //validità programma
		int fileError; //errori relativi al file
		bool esecuzioneTerminata; //memorizza se l' esecuzione del programma è già stata terminata
		Format formato; //formato dell' output/input, scpecifica se gli output devono essere mostrati come numero o come carattere
};

#endif
