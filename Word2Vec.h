#pragma once
#include "Slovnik.h"
#include <vector>

using namespace std;

class Word2Vec {
private:
	Slovnik* slovnik;
	string trenovaciSoubor;
	int kontextoveOkno;
	float miraUceni;
	float pocatecniMiraUceni;
	float vzorek;
	int pocetEpoch;
	int pocetNegatVzorku;
	//long long aktualniPocetSlov;
	long long trenovanaSlova;
	int pocetVlaken;

	long long pocetUnikatnichSlov;
	long long pocetNeuronuSkryteVrstvy;
	//unordered_map<string, vector<float>> W;
	//unordered_map<string, vector<float>> W1;
	//vector<float>* W;
	//vector<float>* W1;
	float* W;
	float* W1;

	vector<float> expTable;

	int maxExp;
	int expTableSize;

public:
	Word2Vec(string _trenovaciSoubor, Slovnik* _slovnik);
	void alokujNeuronovouSit();
	void natrenujSit(int);
	void ulozVysledek();
	void prectiVetu(ifstream &soubor, vector<string> &veta);
	~Word2Vec();
};