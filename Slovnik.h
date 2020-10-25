#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

/** Struktura slouží pro uchování slova a poèet jeho výskytù */
struct slovoVeSlovniku {
	long long pocetVyskytu = 0;
	string text;
};

/** Tøída Slovník je urèena pro uchování slov */
class Slovnik {
private:
	int minPocet;
	string trenovaciSoubor;
	string slovnikSoubor;
	long long velikostSlovniku;
	int velikostHashSlovniku;
	slovoVeSlovniku slovo;
	long long pocetSlovVSouboru=0;
public:
	unordered_map<string, slovoVeSlovniku> mapaSlov;
	//vector<string>* vectorSlovnik;
	string* vectorSlovnik;
	Slovnik(string trenovaciSoubor, string slovnikSoubor);
	//int hashSlova(string&);
	void pridejSlovoDoSlovniku(string&, int, int); //todo - vyøešit kolize hashe
	void zmensiSlovnik();
	void vytvorSlovnik();
	void ulozSlovnik();
	void nactiSlovnik(); //todo - upravit poøadí 
	bool bExistujeSlovo(string);
	int pocetSlov();
	long long pocetVsechSlov();
	long long pocetSlovVeSlovniku();
};