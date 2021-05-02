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
	
	Slovnik* slovnik = new Slovnik("Data/trenovaciSoubor.txt", "Data/slovnik.txt");
	Word2Vec* w2v = new Word2Vec("Data/trenovaciSoubor.txt", slovnik);
	
	//slovnik->vytvorSlovnik();
	//slovnik->zmensiSlovnik();
	//slovnik->ulozSlovnik();
	
	
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
	





	//Pøesunout do slovnik.cpp
	/*
	ifstream soubor("Data/trenovaciSoubor.txt", ifstream::in);
	string slovo;
	long long pocitadlo = 0;
	while (soubor >> slovo) {
		if (slovo != "0") {
			pocitadlo++;
		}
	}
	soubor.close();
	cout << "V souboru je: " << pocitadlo << " slov." << endl;
	*/

	cin.get();
	return 0;
}