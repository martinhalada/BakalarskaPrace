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

Word2Vec::Word2Vec(string _trenovaciSoubor, Slovnik* _slovnik) {
	this->trenovaciSoubor = _trenovaciSoubor;
	this->slovnik = _slovnik;
	this->kontextoveOkno = 15;
	this->miraUceni = 0.025f;
	this->pocatecniMiraUceni = miraUceni;
	this->vzorek = 1e-3f;
	//this->pocetUnikatnichSlov = 10;
	this->pocetNeuronuSkryteVrstvy = 100;
	this->pocetEpoch = 5;
	this->pocetNegatVzorku = 2;
	//this->aktualniPocetSlov = 0;
	this->trenovanaSlova = 0;
	this->pocetVlaken = 12;
	this->maxExp = 6;
	this->expTableSize = 1000;
}


void Word2Vec::alokujNeuronovouSit()
{
	cout << "Alokuji neuronovou sit" << endl;
	int size = slovnik->mapaSlov.size();
	//W = new vector<float>(size*pocetNeuronuSkryteVrstvy);
	//W1 = new vector<float>(size *pocetNeuronuSkryteVrstvy);

	W = new float[size*pocetNeuronuSkryteVrstvy];
	W1 = new float[size *pocetNeuronuSkryteVrstvy];


	float randCislo;
	srand((unsigned)time(0));
	/*
	for (auto it = slovnik->mapaSlov.begin(); it != slovnik->mapaSlov.end(); it++) {
		vector<float> radek;
		for (int j = 0; j < pocetNeuronuSkryteVrstvy; j++) {
			randCislo = 2 * ((float)rand() / RAND_MAX) - 1;
			radek.push_back(randCislo);
		}
		string s;
		W.insert(make_pair(it->first, radek));
	}

	for (auto it = slovnik->mapaSlov.begin(); it != slovnik->mapaSlov.end(); it++) {
		vector<float> radek;
		for (int j = 0; j < pocetNeuronuSkryteVrstvy; j++) {
			randCislo = 2 * ((float)rand() / RAND_MAX) - 1;
			radek.push_back(randCislo);
		}
		string s;
		W1.insert(make_pair(it->first, radek));
	}
	*/

	for (int i = 0; i < size * pocetNeuronuSkryteVrstvy; i++) {
		randCislo = 2 * ((float)rand() / RAND_MAX) - 1;
		*(W+i) = randCislo;
		randCislo = 2 * ((float)rand() / RAND_MAX) - 1;
		*(W1+i) = randCislo;
	}

	//pøedpoèítání sigmoid funkce
	for (int i = 0; i < expTableSize; i++) {
		expTable.push_back(exp((i / (float)expTableSize * 2 - 1) * maxExp));
		expTable[i] = expTable[i] / (expTable[i] + 1);
	}

	trenovanaSlova = slovnik->pocetSlovVeSlovniku();
	cout << "Sit je alokovana" << endl;
}


void Word2Vec::prectiVetu(ifstream &soubor, vector<string> &veta)
{	
	string slovo;
	while (soubor >> slovo) {
		if(slovo == "0"){
			return;
		}
		else {
			veta.push_back(slovo);
		}
	}	
}

Word2Vec::~Word2Vec()
{
	delete slovnik;
}


void Word2Vec::natrenujSit(int vlakna)
{
	ifstream soubor(trenovaciSoubor, ifstream::in);

	vector<string> veta;
	mu.lock();
	soubor.seekg(0, ios::end);
	auto pocetVsechZnaku = soubor.tellg(); //1 159 923 046    pocet vsech slov=170433965  //bez 0 je tam 157261208 slov
	mu.unlock();

	soubor.seekg(pocetVsechZnaku / (long long)pocetVlaken * (long long)vlakna);

	long long pocetSlov = 0, posledniPocetSlov = 0;
	long long delkaVety = 0, pozice = 0;
	long long c, b, a, d;
	long long dalsiRandom=0;
	string posledniSlovo;
	string ciloveSlovo;
	long long hodnotaNeg;
	float f, g;
	string l1, l2;
	long long aktualniPocetSlov = 0;
	
	//vector<float>* vrstvaSChybou = new vector<float>(pocetNeuronuSkryteVrstvy);
	//vector<float>* l1Radek = new vector<float>(pocetNeuronuSkryteVrstvy);
	//vector<float>* l2Radek = new vector<float>(pocetNeuronuSkryteVrstvy);
	float* vrstvaSChybou = new float[pocetNeuronuSkryteVrstvy];
	//float* l1Radek = new float[pocetNeuronuSkryteVrstvy];
	//float* l2Radek = new float[pocetNeuronuSkryteVrstvy];

	int pocetEpoch = this->pocetEpoch;

	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(0, 200000); // guaranteed unbiased


	//cout << "Celk pocet slov: " << slovnik->pocetSlovVeSlovniku() << endl;

	int velikost = slovnik->vectorSlovnik->size();
	
	while (true) {
		//vypiš progress a uprav trenovaci alpha parametr - mira uèeni

		if (pocetSlov - posledniPocetSlov > 10000) {
			aktualniPocetSlov += pocetSlov - posledniPocetSlov;
			posledniPocetSlov = pocetSlov;
			//mu.lock();
			//cout << "Mira uceni: " << miraUceni << " Dokonceno: " << aktualniPocetSlov / (float)(pocetEpoch * trenovanaSlova + 1) * 100 << "% \r";
			cout << "Mira uceni: " << miraUceni << " Dokonceno: " << aktualniPocetSlov / (float)(pocetEpoch * 157261208 / pocetVlaken) * 100 << "% \r";
			//mu.unlock();
		}
		//miraUceni = pocatecniMiraUceni * (1 - aktualniPocetSlov / (float)(pocetEpoch * trenovanaSlova + 1));
		miraUceni = pocatecniMiraUceni * (1 - aktualniPocetSlov / (float)(pocetEpoch * 157261208 + 1));
		if (miraUceni < pocatecniMiraUceni * 0.0001) miraUceni = pocatecniMiraUceni * 0.0001;

		//pøeèti další vìtu ze souboru
		if (veta.empty()) {
			prectiVetu(soubor, veta);
			pocetSlov += veta.size();
			pozice = 0;
		}
		
		//if (soubor.eof() || (pocetSlov >=  slovnik->pocetVsechSlov() / pocetVlaken)) {
		if (soubor.eof() || (pocetSlov >= 157261208 / pocetVlaken)) {
			aktualniPocetSlov += pocetSlov - posledniPocetSlov;
			pocetEpoch--;
			//mu.lock();
			cout << "\nPocet epoch: " << pocetEpoch << endl;
			//mu.unlock();
			if (pocetEpoch == 0) { break; }
			pocetSlov = 0;
			posledniPocetSlov = 0;
			soubor.seekg(pocetVsechZnaku / (long long)pocetVlaken * (long long)vlakna);
		}
		
		//nastavení skryté vrstvy a vrstvy s chybou = neu1e
		//std::fill(vrstvaSChybou.begin(), vrstvaSChybou.end(), 0);
		for (int i = 0; i < pocetNeuronuSkryteVrstvy; i++) {
			*(vrstvaSChybou+i) = 0;
		}
		
		dalsiRandom *= 25214903917 + 11;
		b = dalsiRandom % kontextoveOkno;
		
		//skip-gram - w=syn0, w1=syn1neg
		if (pozice >= veta.size())continue;
		string slovo = veta[pozice];
		
		
		for (a = b; a < kontextoveOkno * 2 + 1 - b; a++) {
			if (a != kontextoveOkno) {
				c = pozice - kontextoveOkno + a;
				if (c < 0 || c >= veta.size()) continue;
				posledniSlovo = veta[c];
				if (!slovnik->bExistujeSlovo(posledniSlovo)) continue;
				//std::fill(vrstvaSChybou.begin(), vrstvaSChybou.end(), 0);
				
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
				ciloveSlovo = slovo;
				hodnotaNeg = 1;
				
				//mu.lock();
				//for (int i = 0; i < pocetNeuronuSkryteVrstvy; i++) {
				//	*(l1Radek+i) = *(W + l1Index + i);
				//}
				//mu.unlock();
				
				
				
				for (d = 0; d < pocetNegatVzorku; d++) {
					auto random_integer = uni(rng);
					ciloveSlovo = *(slovnik->vectorSlovnik + random_integer);

					if (ciloveSlovo == slovo) continue;
					hodnotaNeg = 0;

					l2 = ciloveSlovo;
					int l2Index = 0;
					for (int i = 0; i < velikost; i++) {
						if (l2 == *(slovnik->vectorSlovnik + i)) {
							l2Index = i*pocetNeuronuSkryteVrstvy;
							break;
						}
					}
					
					f = 0;
					
					//l2Radek = W1[l2];
					//mu.lock();
					//for (int i = 0; i < pocetNeuronuSkryteVrstvy; i++) {
					//	*(l2Radek+i) = *(W1 +l2Index + i);
					//}
					//mu.unlock();
					//mu.lock();
					for (c = 0; c < pocetNeuronuSkryteVrstvy; c++) {
						f += *(W + l1Index + c) * *(W1 + l2Index + c);
					}
					//mu.unlock();
					
					if (f > maxExp) {
						g = (hodnotaNeg - 1) * miraUceni;
					}
					else if (f < -maxExp) {
						g = (hodnotaNeg - 0) * miraUceni;
					}
					else {
						g = (hodnotaNeg - expTable[(int)((f + maxExp) * (expTableSize / maxExp / 2))] * miraUceni);
					}
					//mu.lock();
					for (c = 0; c < pocetNeuronuSkryteVrstvy; c++) {
						*(vrstvaSChybou+c) += g * *(W1 + l2Index +c);
					}					
					for (c = 0; c < pocetNeuronuSkryteVrstvy; c++) {
						*(W1 + l2Index +c) += g * *(W + l1Index +c);
					}
					//mu.unlock();
					/*
					mu.lock();
					for (int i = 0; i < pocetNeuronuSkryteVrstvy; i++) {
						*(W1+l2Index + i) = *(l2Radek+i);
					}
					//W1[l2] = l2Radek;
					mu.unlock();
					*/
				}
				
				//mu.lock();
				for (c = 0; c < pocetNeuronuSkryteVrstvy; c++) {
					*(W + l1Index + c) += *(vrstvaSChybou+c);
				}	
				//mu.unlock();

				/*
				mu.lock();
				for (int i = 0; i < pocetNeuronuSkryteVrstvy; i++) {
					*(W+l1Index + i) = *(l1Radek+i);
				}
				//W[l1] = l1Radek;
				mu.unlock();
				*/
			}
		}
		
		
		//konec vìty
		pozice++;
		if (pozice >= veta.size()) {
			pozice = 0;
			veta.erase(veta.begin(), veta.end());
			continue;
		}	
	}
		
	soubor.close();
}


void Word2Vec::ulozVysledek()
{
	cout << "Ukladam vysledek" << endl;
	ofstream soubor("Data/NatrenovaneVektory.txt", ofstream::out);
	/*
	for (auto& it : W) {
		soubor << it.first << " ";
		for (int i = 0; i < pocetNeuronuSkryteVrstvy; i++) {
			soubor << it.second.at(i) << " ";
		}
		soubor << "KONEC" << endl;
	}
	*/
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
