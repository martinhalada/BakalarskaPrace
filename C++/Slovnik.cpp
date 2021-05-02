#define _CRT_SECURE_NO_WARNINGS
#include "Slovnik.h"
#include <algorithm>
#include <iostream>

using namespace std;

/**
* Konstruktor tøídy slovník nastaví parametry slovníku a cestu k souborùm.
*
* @param trenovaciSoubor je cesta k souboru s trénovacím souborem
* @param slovnikSoubor je cesta k souboru se slovníkem
*/
Slovnik::Slovnik(string trenovaciSoubor, string slovnikSoubor)
{
	this->trenovaciSoubor = trenovaciSoubor;
	this->slovnikSoubor = slovnikSoubor;
	this->minPocet = 5;
	this->velikostSlovniku = 0;
	this->pocetSlovVSouboru = 170433965; //bez 0 je tam 157261208 slov
	//vectorSlovnik = new vector<string>(201902);
	vectorSlovnik = new string[201902];
}

/**
* Metoda pøidá slovo do slovníku.
*
* @param slovo je slovo, které chceme pøidat
* @param pocet je poèet výskytù slova
* @param index je index slova v poli ve slovy
*/
void Slovnik::pridejSlovoDoSlovniku(string& slovo, int pocet = 1, int index = 0)
{
	if (slovo == "0") return;
	unordered_map<string, slovoVeSlovniku>::iterator it = mapaSlov.find(slovo);
	if (it == mapaSlov.end()) {
		slovoVeSlovniku s;
		s.text = slovo;
		s.pocetVyskytu = pocet;
		mapaSlov.insert(make_pair(slovo, s));
		//(*vectorSlovnik).push_back(slovo);
		*(vectorSlovnik + index) = slovo;
	}
	else {
		it->second.pocetVyskytu += pocet;
	}
	//pocetSlovVSouboru++;
}

/**
* Metoda zjistí poèet všech slov ve slovníku (sèítá jejich frekvence).
*/
long long Slovnik::pocetSlovVeSlovniku()
{
	long long pocet = 0;
	for (auto it = mapaSlov.cbegin(), nextIt = it; it != mapaSlov.cend(); it = nextIt) {
		++nextIt;
		pocet += it->second.pocetVyskytu;
	}
	return pocet;
}

/**
* Metoda odstraní ze slovníku ta slova, jejichž výskyt je menší než je hodnota v promìnné minPocet.
*/
void Slovnik::zmensiSlovnik()
{
	cout << "Zmensuji slovnik" << endl;
	for (auto it = mapaSlov.cbegin(), nextIt = it; it != mapaSlov.cend(); it = nextIt) {
		++nextIt;
		if (it->second.pocetVyskytu < minPocet) {
			mapaSlov.erase(it);
		}
	}
	cout << "Slovnik zmensen, je zde: " << mapaSlov.size() << " slov" << endl;
}

/**
* Metoda ète ze souboru slova a pøidá je do slovníku.
*/
void Slovnik::vytvorSlovnik()
{
	cout << "Vytvarim slovnik" << endl;
	ifstream soubor(trenovaciSoubor, ifstream::in);
	string slovo;
	while (soubor >> slovo) {
		pridejSlovoDoSlovniku(slovo);
	}
	soubor.close();
	cout << "Slovnik vytvoren" << endl;
}

/**
* Metoda uloží slovník do souboru.
*/
void Slovnik::ulozSlovnik()
{
	cout << "Ukladam slovnik" << endl;
	ofstream soubor("Data/slovnikZmenseny5.txt", std::ios::out | std::ios::app);
	for (auto& it : mapaSlov) {
		soubor << it.first << " " << it.second.text << " " << it.second.pocetVyskytu << " " << endl;
	}
	soubor.close();
	cout << "Slovnik ulozen" << endl;
}

/**
* Metoda naète již vytvoøený slovník ze souboru.
*/
void Slovnik::nactiSlovnik() {
	cout << "Nacitam slovnik" << endl;
	ifstream soubor(slovnikSoubor, ifstream::in);
	string slovo, pocet;
	long long pocetVyskytu = 0;
	int index = 0;
	while (soubor>>slovo && soubor>>slovo && soubor>>pocet){//getline(soubor, slovo, ' ') && getline(soubor, slovo, ' ') && getline(soubor, pocet)) {
		pocetVyskytu = stoll(pocet);
		pridejSlovoDoSlovniku(slovo, pocetVyskytu, index);
		index++;
	}
	soubor.close();
	cout << "Slovnik je nacteny" << endl;
}

/**
* Metoda zjistí, jestli se slovo nachází ve slovníku.
*
* @param slovo je slovo, u kterého zjišujeme výskyt ve slovníku
*/
bool Slovnik::bExistujeSlovo(string slovo)
{
	if(mapaSlov.find(slovo)==mapaSlov.end())
		return false;
	return true;
}

/**
* Metoda zjistí poèet rùzných slov ve slovníku.
*/
int Slovnik::pocetSlov()
{
	return mapaSlov.size();
}

/**
* Metoda zjistí poèet slov v trénovacím souboru (vèetnì znaku 0). 
*/
long long Slovnik::pocetVsechSlov()
{
	return pocetSlovVSouboru;
}

/**
* Metoda zjistí poèet všech slov v trénovacím souboru (bez znaku 0).
*/
long long Slovnik::vypisPocetSlovVTrenovacimSouboru() {
	ifstream soubor(trenovaciSoubor, ifstream::in);
	string slovo;
	long long pocitadlo = 0;
	while (soubor >> slovo) {
		if (slovo != "0") {
			pocitadlo++;
		}
	}
	soubor.close();
	return pocitadlo;
}




/*
int Slovnik::hashSlova(string& slovo)
{
	long long hash = 0;
	int n = slovo.length();
	char* charPole = new char[n + 1];
	strcpy(charPole, slovo.c_str());

	for (int i = 0; i < n; i++)
		hash = charPole[i] + 31 * hash;
	return hash % velikostHashSlovniku;
}
*/