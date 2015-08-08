#include "BrainFuck.h"

BrainFuck::BrainFuck(char *program, Format output)
{
	//inizializzo l' oggetto:
	this->formato = output;
	this->valido = true;
	this->numeroOperazioniAritmetiche = 0L;
	this->esecuzioneTerminata = false;
	this->totaleIstruzioniEseguite = 0L;
	this->numeroIstruzioneAttuale = 0L;
	this->numeroIstruzioniCaricate = 0L;
	this->bytesUsati = 1L;
	this->fileError = NO_ERRORS;
	
	//creo il primo byte nel nastro "infinito":
	Nastro = new struct Byte;
	Nastro->Next = (struct Byte*)NULL;
	Nastro->Previous = (struct Byte*)NULL;
	Nastro->Data = (unsigned char)0x00;
	
	//apro ed esamino il file
	FILE *pFile = fopen(program, "r");
	if (pFile != (FILE*)NULL)
	{
		fseek (pFile, 0, SEEK_END);
		unsigned long programSize = ftell(pFile); //numero di caratteri nel file (1 byte = 1 char)
		fseek (pFile, 0, SEEK_SET); 
		if (programSize != 0)
		{
			while (!feof(pFile)) //per ogni carattere nel file:
			{
				char carattere = fgetc(pFile); //leggi il carattere
				if ((carattere == '>') || (carattere == '<') || (carattere == '+') || (carattere == '-') || (carattere == '.') || (carattere == ',') || (carattere == '[') || (carattere == ']'))
				{ //e se il carattere è una istruzione caricalo nella memoria istruzioni....
					//trasformazione carattere ---> codice istruzione
					unsigned short codiceIstruzione;
					switch (carattere)
					{
						case '>':
							codiceIstruzione = 0;
							break;
						case '<':
							codiceIstruzione = 1;
							break;
						case '+':
							codiceIstruzione = 2;
							break;
						case '-':
							codiceIstruzione = 3;
							break;
						case '.':
							codiceIstruzione = 4;
							break;
						case ',':
							codiceIstruzione = 5;
							break;
						case '[':
							codiceIstruzione = 6;
							break;
						case ']':
							codiceIstruzione = 7;
							break;
						default:
							break;
					}
					//carica la nuova istruzione
					this->istruzioni.push_back(codiceIstruzione);
					//aggiorna il contatore di istruzioni caricate
					this->numeroIstruzioniCaricate++;
				}
			}
			
			if (this->numeroIstruzioniCaricate != 0) //se il file contiene istruzioni
			{ //controlla la corrispondenza fra [ e ]
				long par = 0L, j;
				bool errori = false;
				for (j = 0L; j < this->numeroIstruzioniCaricate; j++)
				{
					if (this->istruzioni[j] == 6)
						par++;
					else if (this->istruzioni[j] == 7)
						par--;
					//se una parentesi è stata chiusa senza essere aperta....
					if (par < 0)
					{
						errori = true; //memorizzo l' errore
						break; //ed esco dal ciclo
					}
				}
				if (par != 0)
					errori = true;

				if (errori) //se c' è una mancata corrispondenza fra [ e ]
				{ //memorizza il tipo di errore e l' invalidità del programma
					this->fileError = NOT_CORRESPONDING;
					this->valido = false;
				}
			} else { //il file non contiene istruzioni, memorizza il tipo di "errore" e l' invalidità del programma
				this->fileError = NOINSTRUCTIONS;
				this->valido = false;
			}
			
		} else {
			//il file non contiene caratteri
			this->fileError = FILE_EMPTY;
			this->valido = false;
		}
		fclose(pFile);
	} else {
		//impossibile aprire il file
		this->fileError = FILE_UNAVAILABLE;
		this->valido = false;
	}
	
}


BrainFuck::~BrainFuck()
{
	if (this->Nastro != (struct Byte*)NULL)
	{
		//mi porto al primo byte
		while (this->Nastro->Previous != (struct Byte*)NULL)
			this->Nastro = this->Nastro->Previous;
		//elimino ad uno ad uno gli elementi della lista
		while (this->Nastro->Next != (struct Byte*)NULL)
		{
			if (this->Nastro->Next != (struct Byte*)NULL) //se c' è un byte dopo quello attuale
			{
				//elimino quello attuale dopo essermi spostato sul successivo
				this->Nastro = this->Nastro->Next;
				delete this->Nastro->Previous;
			} else { //altrimenti sono arrivato alla fine della sequenza di byte e posso eliminare quello attuale
				delete this->Nastro;
			}
		}
	}
	//se ci sono istruzioni caricate in memoria allora libera quella memoria
	if (!this->istruzioni.empty())
		this->istruzioni.clear();
	//se ci sono ancora dati nello stack utilizzato per i cicli libera la memoria dello stack (ovviamente se ciò accade, lè esecuzione del programma non è terminata)
	while (!this->Cicli.empty())
		this->Cicli.pop();
}

unsigned long BrainFuck::Esegui()
{
	//in caso di validità del programma
	if (this->valido)
	{
		//mi preparo ad eseguire l' intero programma
		this->totaleIstruzioniEseguite = 0L;
		this->numeroIstruzioneAttuale = 0L;
		this->numeroOperazioniAritmetiche = 0L;
		this->esecuzioneTerminata = false;
		//eseguo il ciclo che provvederà ad eseguire ogni istruzione in ordine fino alla fine del programma
		while (this->numeroIstruzioneAttuale < this->numeroIstruzioniCaricate)
		{
			if (this->EseguiIstruzione()) //se l' istruzione è stata correttamente eseguita
				this->totaleIstruzioniEseguite++; //aggiorno il numero totale di istruzioni eseguite
		}
		//memorizzo che l' esecuzione è stata portata a termine
		this->esecuzioneTerminata = true;
		/* QUI GENERO IL REPORT */

	}
	return this->totaleIstruzioniEseguite;
}

bool BrainFuck::EseguiIstruzione()
{
	if ((this->valido) && (!this->esecuzioneTerminata))
	{
		if (this->istruzioni[this->numeroIstruzioneAttuale] == 0)
		{ //istruzione >
			if (this->Nastro->Next == (struct Byte*)NULL) //se non c' è un byte successivo a quello attuale
			{
				//crealo.....
				struct Byte* nuovoByte = new struct Byte;
				//collega il byte attuale a quello nuovo e vice versa
				nuovoByte->Previous = this->Nastro;
				nuovoByte->Next = (struct Byte*)NULL;
				this->Nastro->Next = nuovoByte;
				//inizializzane il valore a 0
				nuovoByte->Data = (unsigned char)0x00;
				//ed aggiorna il numero di byte richiesti dal programma
				this->bytesUsati = this->bytesUsati + 1;
			}
			this->Nastro = this->Nastro->Next;
		}
		else if (this->istruzioni[this->numeroIstruzioneAttuale] == 1)
		{ //istruzione <
			if (this->Nastro->Previous == (struct Byte*)NULL) //se non c' è un byte precendente a quello attuale
			{
				//crealo.....
				struct Byte* nuovoByte = new struct Byte;
				//collega il byte attuale a quello nuovo e vice versa
				nuovoByte->Next = this->Nastro;
				nuovoByte->Previous = (struct Byte*)NULL;
				this->Nastro->Previous = nuovoByte;
				//inizializzane il valore a 0
				nuovoByte->Data = (unsigned char)0x00;
				//ed aggiorna il numero di byte richiesti dal programma
				this->bytesUsati = this->bytesUsati + 1;
			}
			this->Nastro = this->Nastro->Previous;
		}
		else if (this->istruzioni[this->numeroIstruzioneAttuale] == 2)
		{ //istruzione +
			this->Nastro->Data = this->Nastro->Data + 1;
			//aggiorna il numero di istruzioni aritmetiche eseguite
			this->numeroOperazioniAritmetiche++;
		}
		else if (this->istruzioni[this->numeroIstruzioneAttuale] == 3)
		{ //istruzione -
			this->Nastro->Data = this->Nastro->Data - 1;
			//aggiorna il numero di istruzioni aritmetiche eseguite
			this->numeroOperazioniAritmetiche++;
		}
		else if (this->istruzioni[this->numeroIstruzioneAttuale] == 4)
		{ //istruzione .
			if (this->formato == Numbers)
				printf("%u", this->Nastro->Data);
			else
				printf("%c", this->Nastro->Data);
		} //istruzione ,
		else if (this->istruzioni[this->numeroIstruzioneAttuale] == 5)
		{
			char lettura;
			fflush(stdin);
			//leggi il valore inserito da tastiera a seconda del formato specificato
			if (this->formato == Numbers)
				scanf("%u", &lettura);
			else
				scanf("%c", &lettura);
			//aggiorna il byte attuale
			this->Nastro->Data = lettura;
		}
		else if (this->istruzioni[this->numeroIstruzioneAttuale] == 6)
		{ //istruzione [
			if (this->Nastro->Data == (unsigned char)0x00)
			{ //salta alla corrispondente ]
				unsigned long par = 1;
				while (par >= 1L)
				{
					this->numeroIstruzioneAttuale++;
					if (this->istruzioni[this->numeroIstruzioneAttuale] == 7)
						par--;
					else if (this->istruzioni[this->numeroIstruzioneAttuale] == 6)
						par++;
				}
			} else { //memorizza nello stack la posizione attuale
				this->Cicli.push(this->numeroIstruzioneAttuale);
			}
		}
		else if (this->istruzioni[this->numeroIstruzioneAttuale] == 7)
		{ //istruzione ]
			if (this->Nastro->Data == (unsigned char)0x00) //se il byte attuale è zero
			{ //procedi con l' istruzione successiva alla ] attuale
				this->Cicli.pop();	
			} else { //altrimenti torna alla corrispondente [
				this->numeroIstruzioneAttuale = this->Cicli.top();
			}
		}
		//rendo la prossima istruzione da eseguire, quella successiva di quella appena eseguita
		this->numeroIstruzioneAttuale++;
		if (this->numeroIstruzioneAttuale == this->numeroIstruzioniCaricate)
			this->esecuzioneTerminata = true;
		return true;
	}
	return false;
}

struct Report BrainFuck::GeneraReport()
{
	//genero il report a partire dalle informazioni memorizzate nell' oggetto
	struct Report generato;
	generato.byteUtilizzati = this->bytesUsati;
	generato.istruzioniCaricate = this->numeroIstruzioniCaricate;
	generato.istruzioniEseguite = this->totaleIstruzioniEseguite;
	generato.opAritmetiche = this->numeroOperazioniAritmetiche;
	generato.ultimoErrore = this->fileError;
	generato.terminato = this->esecuzioneTerminata;
	return generato;
}