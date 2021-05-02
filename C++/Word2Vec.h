#pragma once
#include "Slovnik.h"
#include <vector>

using namespace std;

/** Tøída Word2Vec natrénuje neuronovou sí */
class Word2Vec {
private:
	/* Vytvoøení slovníku, pro pøístup k jeho metodám. */
	Slovnik* slovnik;
	/* Cesta k trénovacímu souboru. */
	string trenovaciSoubor;
	/* Velikost kontextového okna. */
	int kontextoveOkno;
	/* Parametr modelu (alpha). */
	float miraUceni;
	/* Vıchozí alpha (míra uèení). */
	float pocatecniMiraUceni;
	/* Poèet epoch (poèet prùchodù souborem). */
	int pocetEpoch;
	/* Poèet negativních vzorkù pro Negative Sampling. */
	int pocetNegatVzorku;
	/* Poèet trénovanıch slov. */
	long long trenovanaSlova;
	/* Poèet vláken programu. */
	int pocetVlaken;
	/* Poèet unikátních slov. */
	long long pocetUnikatnichSlov;
	/* Velikost skryté vrstvy. */
	long long pocetNeuronuSkryteVrstvy;
	/* Matice vah vstupní vrstvy. */
	float* W;
	/* Matice vah vıstupní vrstvy. */
	float* W1;
	/* Vektor reprezentující tabulku. */
	vector<float> expTable;
	/* Maximální velikost tabulky. */
	int maxExp;
	/* Velikost tabulky. */
	int expTableSize;
	/* Celkovı poèet znakù sdílenı všemi vlákny */
	int celkPocetZnaku;
public:
	/* Konstruktor nastaví parametry pro cestu k souborùm a další parametry. */
	Word2Vec(string _trenovaciSoubor, Slovnik* _slovnik);
	/* Metoda alokuje neuronovou sí a pøedpoèítá sigmoid funkci.  */
	void alokujNeuronovouSit();
	/* Hlavní metoda pro natrénování neuronové sítì. */
	void natrenujSit(int, char*, size_t length);
	/* Metoda uloí natrénované vektory do souboru. */
	void ulozVysledek();
	/* Metoda pøeète další vìtu ze souboru. */
	int prectiVetu(char* &buffer, int* index, string* &veta, int* pocetZnaku, int* znakyVSegmentu);
	/* Destruktor tøídy. */
	~Word2Vec();
	/* Metoda uloí natrénované vektory do binárního souboru */
	void ulozDoBinaru();

	/* Metoda pro testování naèítání ze souboru. */
	void test(int, char* buffer, size_t index);
};