#define _CRT_SECURE_NO_WARNINGS
#include "Word2Vec.h"
#include "Slovnik.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include <thread>
#include <mutex>

using namespace std;
std::mutex mu;

/**
* Konstruktor tøídy nastaví parametry a cestu k trénovacímu souboru.
*
* @param _trenovaciSoubor je cesta k trénovacímu souboru
* @param _slovnik vytvoøený slovník
*/
Word2Vec::Word2Vec(string _trenovaciSoubor, Slovnik* _slovnik) {
	this->trenovaciSoubor = _trenovaciSoubor;
	this->slovnik = _slovnik;
	this->kontextoveOkno = 15;
	this->miraUceni = 0.025f;
	this->pocatecniMiraUceni = miraUceni;
	this->pocetNeuronuSkryteVrstvy = 100;
	this->pocetEpoch = 5;
	this->pocetNegatVzorku = 5;
	this->trenovanaSlova = 0;
	this->pocetVlaken = 12;
	this->maxExp = 6;
	this->expTableSize = 1000;
	this->celkPocetZnaku = 0;
}


/**
* Metoda naplní matice vah náhodnými hodnotami a pøedpoèítá sigmoid funkci.
*/
void Word2Vec::alokujNeuronovouSit()
{
	cout << "Alokuji neuronovou sit" << endl;
	int size = slovnik->mapaSlov.size();

	W = new float[size*pocetNeuronuSkryteVrstvy];
	W1 = new float[size *pocetNeuronuSkryteVrstvy];


	float randCislo;
	srand((unsigned)time(0));

	for (int i = 0; i < size * pocetNeuronuSkryteVrstvy; i++) {
		randCislo = (2 * ((float)rand() / RAND_MAX) - 1)/2;
		*(W+i) = randCislo;
		//randCislo = 2 * ((float)rand() / RAND_MAX) - 1;
		*(W1+i) = 0;
	}

	//pøedpoèítání sigmoid funkce
	for (int i = 0; i < expTableSize; i++) {
		expTable.push_back(exp((i / (float)expTableSize * 2 - 1) * maxExp));
		expTable[i] = expTable[i] / (expTable[i] + 1);
	}

	trenovanaSlova = slovnik->pocetSlovVeSlovniku();
	cout << "Sit je alokovana" << endl;
}


/**
* Metoda pøeète další vìtu ze souboru.
*
* @param soubor je trénovací soubor
* @param veta je vektor reprezentující vìtu
*/
int Word2Vec::prectiVetu(char* &buffer, int* index, string* &veta, int* aktualniPocetZnaku, int* znakyVSegmentu)
{
	int minPozice = *index;
	int pocetZnaku = *aktualniPocetZnaku;
	int pom = *znakyVSegmentu;
	int vetaIndex = 0;
	string slovo ="";
	while (buffer[minPozice] != '0') {
		
		slovo += buffer[minPozice];
		if (buffer[minPozice] == ' ' || buffer[minPozice] == '\n' || buffer[minPozice] == '\t') {
			if (slovo.size() > 0) {
				*(veta + vetaIndex) = slovo;
				vetaIndex++;
				slovo = "";
			}
		}
		minPozice++;
		pocetZnaku++;
		pom++;
		if (buffer[minPozice] == '\0') break;
	}
	pocetZnaku++;
	pom++;
	minPozice++;
	*index = minPozice;
	*aktualniPocetZnaku = pocetZnaku;
	*znakyVSegmentu = pom;
	vetaIndex--;

	return vetaIndex;
}


/**
* Destruktor odstraní slovník z pamìti.
*/
Word2Vec::~Word2Vec()
{
	delete slovnik;
}


/**
* Metoda slouží k natrénování vektorù.
*
* @param vlakna je poèet vláken programu
*/
void Word2Vec::natrenujSit(int vlakna, char* buffer, size_t length)
{
	string* veta = new string[1000];

	//auto pocetVsechZnaku = soubor.tellg(); //1 159 923 046    pocet vsech slov=170433965  //bez 0 je tam 157261208 slov
	auto pocetVsechZnaku = length;

	int hodnota = vlakna + 1;

	int indexBufferStart = pocetVsechZnaku / (long long)pocetVlaken * (long long)vlakna;
	int indexBufferEnd = pocetVsechZnaku / (long long)pocetVlaken * (long long)(hodnota);
	int pocetZnakuVSegmentu = indexBufferEnd - indexBufferStart;
	int aktualniPocetZnaku = 0;
	int znakyVSegmentu = 0;

	long long pocetSlov = 0, posledniPocetSlov = 0;
	long long delkaVety = 0, pozice = 0;
	long long c, b, a;
	int d;
	unsigned long long dalsiRandom=(long long) vlakna;
	string posledniSlovo;
	string ciloveSlovo;
	long long hodnotaNeg;
	float f, g;
	string l1, l2;
	long long aktualniPocetSlov = 0;
	
	float* vrstvaSChybou = new float[pocetNeuronuSkryteVrstvy];

	int pocetEpoch = this->pocetEpoch;

	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(0, 200000); // guaranteed unbiased

	//cout << "Celk pocet slov: " << slovnik->pocetSlovVeSlovniku() << endl;

	int velikost = slovnik->vectorSlovnik->size();
	int pom = 0;
	bool empty = true;

	int epochy = pocetEpoch;
	string slovo;
	while (true) {

		//vypiš progress a uprav trenovaci alpha parametr - mira uèeni	
		if (aktualniPocetZnaku - posledniPocetSlov > 1000000) {
			posledniPocetSlov = aktualniPocetZnaku;
			//celkPocetZnaku += aktualniPocetZnaku - posledniPocetSlov;
			mu.lock();
			cout << "Mira uceni: " << miraUceni << " Dokonceno: " << aktualniPocetZnaku / (float)(epochy * pocetZnakuVSegmentu) * 100 << "% \r";
			mu.unlock();
		}
		miraUceni = pocatecniMiraUceni * (1 - aktualniPocetZnaku / (float)(epochy * pocetZnakuVSegmentu + 1));
		if (miraUceni < pocatecniMiraUceni * 0.0001) miraUceni = pocatecniMiraUceni * 0.0001;

		//pøeèti další vìtu ze souboru
		if (empty) {
			delkaVety = prectiVetu(buffer, &indexBufferStart, veta, &aktualniPocetZnaku, &znakyVSegmentu);
			pocetSlov += delkaVety;
			empty = false;
			pozice = 0;
		}
	
		if (buffer[indexBufferStart] == '\0' || (znakyVSegmentu >= pocetZnakuVSegmentu)) {
			pocetEpoch--;
			//cout << "\nVlaknu: " <<vlakna<<" zbyva: " << pocetEpoch << " epoch." <<endl;
			if (pocetEpoch == 0) { break; }
			pocetSlov = 0;
			posledniPocetSlov = 0;
			znakyVSegmentu = 0;
			indexBufferStart = pocetVsechZnaku / (long long)pocetVlaken * (long long)vlakna;
			empty = true;
			continue;
		}
		
		
		//skip-gram - w=syn0, w1=syn1neg
		//if (pozice >= veta.size())continue;
		if (pozice >= delkaVety) {
		//	pozice = 0;
		//	delkaVety = 0;
			empty = true;
			continue;
		}
		slovo = veta[pozice];
		if (!slovnik->bExistujeSlovo(slovo)) {
			pozice++;
			continue;
		}

		//nastavení vrstvy s chybou = neu1e
		for (int i = 0; i < pocetNeuronuSkryteVrstvy; i++) {
			*(vrstvaSChybou + i) = 0;
		}

		dalsiRandom = dalsiRandom * (unsigned long long)25214903917 + 11;
		b = dalsiRandom % kontextoveOkno;
		
		int oknoOffset = kontextoveOkno * 2 + 1 - b;
		for (a = b; a < oknoOffset; a++) {
			if (a != kontextoveOkno) {
				c = pozice - kontextoveOkno + a;
				if (c < 0 || c >= delkaVety) continue;
				posledniSlovo = veta[c];
				if (!slovnik->bExistujeSlovo(posledniSlovo)) continue;
				
				for (int i = 0; i < pocetNeuronuSkryteVrstvy; i++) {
					*(vrstvaSChybou+i) = 0;
				}
				l1 = posledniSlovo;
				int l1Index=0;
				
				for (int i = 0; i < velikost; i++) {
					if (l1 == *(slovnik->vectorSlovnik + i)) {
						l1Index = i*pocetNeuronuSkryteVrstvy;
						break;
					}
				}
				
				
				//Negative sampling			
				for (d = 0; d < pocetNegatVzorku+1; d++) {
					if (d == 0) {
						ciloveSlovo = slovo;
						hodnotaNeg = 1;
					}
					else {
						auto random_integer = uni(rng);
						ciloveSlovo = *(slovnik->vectorSlovnik + random_integer);
						if (ciloveSlovo == slovo) continue;
						hodnotaNeg = 0;
					}

					l2 = ciloveSlovo;
					int l2Index = 0;
					for (int i = 0; i < velikost; i++) {
						if (l2 == *(slovnik->vectorSlovnik + i)) {
							l2Index = i*pocetNeuronuSkryteVrstvy;
							break;
						}
					}
					
					f = 0;
					
					for (c = 0; c < pocetNeuronuSkryteVrstvy; c++) {
						f += *(W + l1Index + c) * *(W1 + l2Index + c);
					}
					
					if (f > maxExp) {
						g = (hodnotaNeg - 1) * miraUceni;
					}
					else if (f < -maxExp) {
						g = (hodnotaNeg - 0) * miraUceni;
					}
					else {
						g = (hodnotaNeg - expTable[(int)((f + 6) * (1000 / 6 / 2))]) * miraUceni;
					}

					for (c = 0; c < pocetNeuronuSkryteVrstvy; c++) {
						*(vrstvaSChybou+c) += g * *(W1 + l2Index +c);
					}					
					for (c = 0; c < pocetNeuronuSkryteVrstvy; c++) {
						*(W1 + l2Index +c) += g * *(W + l1Index +c);
					}
					
				}
				
				for (c = 0; c < pocetNeuronuSkryteVrstvy; c++) {
					*(W + l1Index + c) += *(vrstvaSChybou+c);
				}	
				
			}
		}
		
		
		//konec vìty
		pozice++;
		if (pozice >= delkaVety) {
			pozice = 0;
			delkaVety = 0;
			//for (int i = 0; i < 1000; i++) {
			//	*(veta + i) = "";
			//}
			empty = true;
			continue;
		}
		
	}
}


/**
* Metoda uloží natrénované vektory do souboru.
*/
void Word2Vec::ulozVysledek()
{
	cout << "Ukladam vysledek" << endl;
	ofstream soubor("Data/Vektory13122020Vahy.txt", ofstream::out);

	int size = slovnik->mapaSlov.size();
	int pozice = 0;
	for (int i = 0; i < size; i++) {
		soubor << *(slovnik->vectorSlovnik +i) << " ";
		pozice = i*pocetNeuronuSkryteVrstvy;
		for (int j = 0; j < pocetNeuronuSkryteVrstvy; j++) {
			soubor << *(W+pozice + j) << " ";
		}
		soubor << "KONEC" << endl;
	}
	soubor.close();
	cout << "Vysledek je ulozen" << endl;
}

/**
* Metoda uloží vektory do binárního souboru
*/
void Word2Vec::ulozDoBinaru() {
	ofstream souborOut("Data/vektory.dat", ios::out | ios::binary);
	ifstream souborIn("Data/VektoryFinal.txt", ifstream::in);

	string slovo;
	float cislo;

	//asi 202 000 slov
	while (souborIn >> slovo) {
		souborOut.write((char*) &slovo, sizeof(slovo));

		//100 dimenzí
		for (int j = 0; j < 100; j++) {
			souborIn >> cislo;
			souborOut.write((char*) &cislo, sizeof(cislo));
		}
		souborIn >> slovo; //,,KONEC'' slovo
	}

	souborIn.close();
	souborOut.close();
	cout << "\nVysledek je ulozen" << endl;
}




/**
* Metoda slouží jen pro otestování naèítání slov ze souboru
*
* @param vlakna je poèet vláken
* @param buffer je pole všech znakù testovacího souboru
* @param length je délka pole buffer
*/
void Word2Vec::test(int vlakna, char* buffer, size_t length) {
	string *veta = new string[1000];
	string slovo;
	int index = length / (long long)pocetVlaken * (long long)vlakna;
	int hodnota = vlakna + 1;
	int max = length / (long long)pocetVlaken * (long long)hodnota;
	char pom;
	int invec = 0;
	while (index<max) {
		pom = buffer[index];
		if (pom == ' ' || pom == '\n') {
			veta[invec] = slovo;
			if (invec >= 999) {
				invec = 0;
			}
			slovo = "";
			invec++;
		}
		else {
			slovo += pom;
		}
		index++;

		if (index % 30000 == 0) {
			mu.lock();
			cout << "max-min" << max - index << endl;
			mu.unlock();
		}
	}
}
