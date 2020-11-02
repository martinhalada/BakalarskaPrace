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

#include <boost/iostreams/device/mapped_file.hpp> // for mmap
#include <algorithm>  // for std::find
#include <iostream>   // for std::cout
#include <cstring>


using namespace std;
using namespace chrono;
using namespace boost::iostreams;


void precti(char* &buffer, int* index, vector<string> *veta)
{
	vector<string> v = *veta;
	int minPozice = *index;
	string slovo;
	while (buffer[minPozice] != '0') {
		slovo += buffer[minPozice];
		if (buffer[minPozice] == ' ' || buffer[minPozice] == '\n') {
			//cout << slovo << endl;
			v.push_back(slovo);
			slovo = "";
		}
		minPozice++;
	}
	*veta = v;
	minPozice++;
	*index = minPozice;
}

/* Hlavní metoda programu. */
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

	ifstream is("Data/trenovaciSoubor.txt", ifstream::binary);
	if (is) {
		is.seekg(0, is.end);
		size_t length = is.tellg();
		is.seekg(0, is.beg);
		char* buffer = new char[length + 1];
		is.read(buffer, length);
		buffer[length] = '\0';
		is.close();
		//vector<string> veta;
		//int indexBufferStart = 0;
		//int aktualniPocetZnaku = 0;
		//w2v->prectiVetu(buffer, &indexBufferStart, &veta, &aktualniPocetZnaku);
		//veta.clear();
		//w2v->prectiVetu(buffer, &indexBufferStart, &veta, &aktualniPocetZnaku);
		//veta.clear();
		//w2v->prectiVetu(buffer, &indexBufferStart, &veta, &aktualniPocetZnaku);
		//veta.clear();
		//w2v->prectiVetu(buffer, &indexBufferStart, &veta, &aktualniPocetZnaku);
		//veta.clear();
		//w2v->prectiVetu(buffer, &indexBufferStart, &veta, &aktualniPocetZnaku);
		//veta.clear();

		
		for (int i = 0; i < pocetVlaken; i++) {
			vlakna.emplace_back(&Word2Vec::test, w2v, i, buffer, length);
		}
		for (int i = 0; i < pocetVlaken; i++) {
			vlakna[i].join();
		}
		
		/*
		for (int i = 0; i < pocetVlaken; i++) {
			vlakna.emplace_back(&Word2Vec::natrenujSit, w2v, i, buffer, length);
		}
		for (int i = 0; i < pocetVlaken; i++) {
			vlakna[i].join();
		}
		*/
		
		//w2v->natrenujSit(0, buffer, length);

		delete[] buffer;
	}

	//w2v->ulozVysledek();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<minutes>(stop - start);
	cout << "Celkovy cas: "
		<< duration.count() << " minut" << endl;
	


	/*
	boost::iostreams::mapped_file_params params;
	params.path = "Data/trenovaciSoubor.txt";
	//params.length = 512; // default: complete file
	params.new_file_size = pow(1024, 2); // 1 MB
	params.flags =
		boost::iostreams::mapped_file::mapmode::readonly;
	boost::iostreams::mapped_file mf;
	mf.open(params);
	char* bytes = (char*)mf.const_data();
	for(int i=0;i<2000;i++)
		cout << bytes[i];
	mf.close();
	*/


	/*
	boost::iostreams::mapped_file mmap("Data/test.txt", boost::iostreams::mapped_file::readonly);
	auto f = mmap.const_data();
	auto l = f + mmap.size();

	long long pocet_slov = 0;
	while (f && f != l)
		if ((f = static_cast<const char*>(memchr(f, ' ', l - f)))) {
			f++, pocet_slov++;
		}

	std::cout << "pocetslov = " << pocet_slov << "\n";
	*/



	/*
	cout << "Start" << endl;
	ifstream soubor("Data/trenovaciSoubor.txt", ifstream::binary);
	string slovo;
	int pocitadlo = 0;
	int index = 0;
	if (soubor) {
		soubor.seekg(0, soubor.end);
		size_t length = soubor.tellg();
		soubor.seekg(0, soubor.beg);
		char* buffer = new char[length + 1];
		soubor.read(buffer, length);
		buffer[length] = '\0';
		
		while (index < length) {
			if (buffer[index] == ' ' || buffer[index] == '\t' || buffer[index] == '\n') {
				pocitadlo++;
			}
			index++;
		}
		cout << pocitadlo << endl;
		cout << index << endl;
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

	//Pøesunout do slovnik.cpp - OK
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