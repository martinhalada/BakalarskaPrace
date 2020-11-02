#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

/** Struktura slouží pro uchování slova a poèet jeho výskytù. */
struct slovoVeSlovniku {
	long long pocetVyskytu = 0;
	string text;
};

/** Tøída Slovník je urèena pro uchování slov. */
class Slovnik {
private:
	/* Minimální možný výskyt slova s trénovacím souboru, aby slovo mohlo být pøidáno. */
	int minPocet;

	/* Cesta k trénovacímu souboru. */
	string trenovaciSoubor;

	/* Cesta k souboru slovníku. */
	string slovnikSoubor;

	/* Velikost slovníku. */
	long long velikostSlovniku;

	/* Promìnná pro uložení slova. */
	slovoVeSlovniku slovo;

	/* Poèet slov v souboru. */
	long long pocetSlovVSouboru=0;
public:
	/* Mapa ukládá jednotlivá slova */
	unordered_map<string, slovoVeSlovniku> mapaSlov;

	/* Pole slov tvoøí slovník */
	string* vectorSlovnik;

	/* Konstruktor tøídy slovník nastaví parametry slovníku a cestu k souborùm. */
	Slovnik(string trenovaciSoubor, string slovnikSoubor);

	/* Metoda pøidá slovo do slovníku. */
	void pridejSlovoDoSlovniku(string&, int, int);

	/* Metoda odstraní slova, jejichž frekvence výskytu je menší než 14. */
	void zmensiSlovnik();

	/* Metoda vytvoøí slovník z trénovacího souboru. */
	void vytvorSlovnik();

	/* Metoda uloží slovník do souboru. */
	void ulozSlovnik();

	/* Metoda naète slovník ze souboru. */
	void nactiSlovnik();

	/* Metoda zjistí, jestli se slovo nachází ve slovníku. */
	bool bExistujeSlovo(string);

	/* Metoda zjistí velikost slovníku (poèet rùzných slov). */
	int pocetSlov();

	/* Metoda zjistí poèet slov v trénovacím souboru (vèetnì znaku 0). */
	long long pocetVsechSlov();

	/* Metoda zjistí poèet všech slov ve slovníku (sèítá jejich frekvence). */
	long long pocetSlovVeSlovniku();

	/* Metoda zjistí poèet všech slov v trénovacím souboru (bez znaku 0). */
	long long vypisPocetSlovVTrenovacimSouboru();
};