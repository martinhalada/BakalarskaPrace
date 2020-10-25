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


using namespace std;
using namespace chrono;

struct membuf : std::streambuf
{
	membuf(char* begin, char* end) {
		this->setg(begin, begin, end);
	}
};

int main() {
	SetConsoleOutputCP(CP_UTF8);
	
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
	
	for (int i = 0; i < pocetVlaken; i++) {
		vlakna.emplace_back(&Word2Vec::natrenujSit, w2v, i);
	}
	for (int i = 0; i < pocetVlaken; i++) {
		vlakna[i].join();
	}
	
	w2v->ulozVysledek();
	

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<minutes>(stop - start);
	cout << "Celkovy cas: "
		<< duration.count() << " minut" << endl;
	


	/*
	cout << "Start" << endl;
	ifstream soubor("Data/trenovaciSoubor.txt", ifstream::binary);
	string slovo;
	int pocitadlo = 0;
	if (soubor) {
		soubor.seekg(0, soubor.end);
		size_t length = soubor.tellg();
		soubor.seekg(0, soubor.beg);
		char* buffer = new char[length + 1];
		soubor.read(buffer, length);
		buffer[length] = '\0';
		
		
		membuf sbuf(buffer, buffer + length);
		std::istream in(&sbuf);
		std::string line;
		while (std::getline(in, line, ' ')) {
			if (line != " " && line != "0") {
				pocitadlo++;
			}
		}
		cout << pocitadlo << endl;
		cout << endl;


		delete[] buffer;
	}
	cout << "Konec" << endl;
	*/

	/*
	time_t start, end;
	start = clock();

	vector<int>* test = new vector<int>(20000000);
	for (int i = 0; i < 20000000; i++) {
		(*test)[i] = i;
	}
	cout << (*test)[100] << endl;

	end = clock();
	double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
	cout << "Time taken by program is : " << fixed
		<< time_taken;
	cout << " sec " << endl;

	start = clock();
	int* test2 = new int[20000000];
	for (int i = 0; i < 20000000; i++) {
		*(test2+i) = i;
	}
	cout << test2[5] << endl;

	end = clock();
	time_taken = double(end - start) / double(CLOCKS_PER_SEC);
	cout << "Time taken by program is : " << fixed
		<< time_taken;
	cout << " sec " << endl;
	*/

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