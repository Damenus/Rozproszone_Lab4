#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <cmath>
#include <math.h>
#include <ctime>
#include <vector>
#include <conio.h>

using namespace std;

struct dane {
	int seed;
	double wynik;
	bool skonczone;
};

double fun(int x) {
	return pow(x, 0.75) + 40 * sin(x);
}

HANDLE watki[10];

DWORD WINAPI funkcja_watku(void *arg);
DWORD WINAPI WaitingForKey(void *arg);

void zadanie1(DWORD id) {
	double wynik = 0;
	int ilosc_watkow = 10;
	dane tab[10];

	for (int i = 0; i < ilosc_watkow; i++) {
		tab[i].seed = rand();
		tab[i].skonczone = false;
		watki[i] = CreateThread(NULL, 0, funkcja_watku, (void*)&tab[i], 0, &id);
	}

	Sleep(100);

	for (int i = 0; i < ilosc_watkow; i++) {
		wynik += tab[i].wynik;
	}

	cout << "Wynik zadanie1: " << wynik / ilosc_watkow << endl;
}

void wypiszWynik(vector<double> wyniki){
	double wynik = 0;
	for (int i = 0; i < wyniki.size(); i++) {
		wynik += wyniki[i];
	}
	cout << "Wynik: " << wynik / wyniki.size() << endl;
}


int main()
{
	DWORD id;
	bool kontynuowac = true;

	HANDLE guard = CreateThread(NULL, 0, WaitingForKey, (void*)&kontynuowac, 0, &id);

	vector<double> wyniki;
	srand(time(NULL));
	double wynik = 0;
	int ilosc_watkow = 10;
	dane tab[10];

	for (int i = 0; i < ilosc_watkow; i++) {
		tab[i].seed = rand();
		tab[i].skonczone = false;
		watki[i] = CreateThread(NULL, 0, funkcja_watku, (void*)&tab[i], 0, &id);
	}

	while (kontynuowac) {
		for (int i = 0; i < ilosc_watkow; i++) {
			if (tab[i].skonczone == true) {
				wyniki.push_back(tab[i].wynik);
				wypiszWynik(wyniki);

				tab[i].seed = rand();
				tab[i].skonczone = false;
				watki[i] = CreateThread(NULL, 0, funkcja_watku, (void*)&tab[i], 0, &id);
			}
		}		

	}

	for (int i = 0; i < wyniki.size(); i++) {
		wynik += wyniki[i];
	}
	cout << "Wynik: " << wynik / wyniki.size() << endl; // wynik : 430
	_getch();

	return 0;
}

DWORD WINAPI funkcja_watku(void *arg) {

	struct dane *tab = (struct dane*)arg;
	srand(tab->seed);
	int licz = 30000;
	int x, y, j = 0;
	double wynik_funkcji;

	for (int i = 0; i < licz; i++) {
		x = rand() % 41; //zakres do 40
		y = rand() % 81; //zakres do 80
		
		wynik_funkcji = fun(x);

		if (y <= wynik_funkcji) {
			j++;
		}
		else if (wynik_funkcji < 0 && y <= abs(wynik_funkcji)) {
			j--;
		}

	}
	
	double cal = (double)j * 80 * 40 / licz;
	//cout << cal << endl;
	tab->wynik = cal;
	tab->skonczone = true;

	return 0;
}

DWORD WINAPI WaitingForKey(void *arg) {
	bool *kontynuowac = (bool*)arg;
	_getch();
	*kontynuowac = false;
	return 0;
}
