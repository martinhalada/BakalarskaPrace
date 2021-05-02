#include "Word2Vec.h"
#include "Slovnik.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <ctime>
#include <random>
#include <functional>
#include <thread>
#include <Windows.h>
#include <sstream>
#include <chrono>
#include <locale>
#include <clocale>
#include <codecvt>

//#include <boost/iostreams/device/mapped_file.hpp> // for mmap
#include <algorithm>  // for std::find
#include <iostream>   // for std::cout
#include <cstring>


using namespace std;
using namespace chrono;
//using namespace boost::iostreams;


/* Hlavní metoda programu. */
int main() {
	//SetConsoleOutputCP(CP_UTF8);
	
	Slovnik* slovnik = new Slovnik("Data/trenovaciSoubor.txt", "Data/slovnikVsechnaSlova.txt");
	Word2Vec* w2v = new Word2Vec("Data/trenovaciSoubor.txt", slovnik);

	//slovnik->nactiSlovnik();



	slovnik->vytvorSlovnik();
	//slovnik->ulozSlovnik();
	//slovnik->nactiSlovnik();
	slovnik->zmensiSlovnik();
	slovnik->ulozSlovnik();

	//slovnik->zmensiSlovnik();
	//slovnik->ulozSlovnik();
	
	/* OK
	slovnik->nactiSlovnik();
	w2v->alokujNeuronovouSit();

	auto start = high_resolution_clock::now();

	int MaxPocetVlaken = thread::hardware_concurrency();
	int pocetVlaken = 12;
	vector<thread> vlakna;

	ifstream is("Data/trenovaciSoubor.txt", ifstream::binary);
	if (is) {
		is.seekg(0, is.end);
		size_t length = is.tellg();
		is.seekg(0, is.beg);
		char* buffer = new char[length + 1];
		is.read(buffer, length);
		buffer[length] = '\0';
		is.close();
		
		for (int i = 0; i < pocetVlaken; i++) {
			vlakna.emplace_back(&Word2Vec::natrenujSit, w2v, i, buffer, length);
		}
		for (int i = 0; i < pocetVlaken; i++) {
			vlakna[i].join();
		}

		delete[] buffer;
	}

	w2v->ulozVysledek();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<minutes>(stop - start);
	cout << "Celkovy cas: "
		<< duration.count() << " minut" << endl;
	*/

	//w2v->ulozDoBinaru();



	/*
	//naètení binárního souboru - pøesunout do metody nactiVektory()
	ifstream soubor("Data/vektory.dat", ios::in | ios::binary);
	string slovo;
	float cislo;
	soubor.read((char*)& slovo, sizeof(slovo));
	cout << slovo << endl;
	for (int i = 0; i < 100; i++) {
		soubor.read((char*)& cislo, sizeof(cislo));
		cout << cislo << endl;
	}
	soubor.read((char*)& slovo, sizeof(slovo));
	cout << slovo << endl;
	soubor.read((char*)& cislo, sizeof(cislo));
	cout << cislo << endl;
	soubor.close();
	*/

	//Pøesunout do slovnik.cpp
	
	ifstream soubor("Data/trenovaciSoubor.txt", ifstream::in);
	string slovo;
	long long pocitadlo = 0;
	long long oov_slova = 0;
	while (soubor >> slovo) {
		if (slovo != "0") {
			pocitadlo++;
			if (!slovnik->bExistujeSlovo(slovo)) {
				oov_slova++;
			}
		}
	}
	soubor.close();
	double oov_rate = ((double)oov_slova / (double)pocitadlo) * 100;
	cout << "V souboru je: " << pocitadlo << " slov." << endl;
	cout << "Ve slovniku nebylo nalezeno: " << oov_slova << " slov." << endl;
	cout << "OOV rate je: " << oov_rate << " %." << endl;
	
	cin.get();
	return 0;
}