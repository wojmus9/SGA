#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <bitset>
using namespace std;
//Zmienne
int a = -1; // współczynniki równania kwadratowego
int b = 0;
int c = 1;
int ile_wyn = 40; // ilość uruchomień programu
int lb_pop = 14;  // liczba populacji 
int ile_os = 10; // liczba osobników w danej populacji 
double pr_krzyz = 0.9; // prawdopodobieństwo krzyżowania 
double pr_mut = 0.1; // prawdopodobieństwo mutacji 

void generator(int randomtab[])
{
	for (int i = 0; i < ile_os; i++)
	{
		randomtab[i] = (rand() % 256);
	}
}


//Zmiana na binarne 
void zm_binarny(int liczba[], string tab[]) //liczba - zmienna
{
	for (int i = 0; i < ile_os; i++)
	{
		tab[i] = bitset<8>(liczba[i]).to_string(); 
	}
}


// Krzyżowanie rodziców i tworzenie potomków 
void krzyzowanie(int ilosc, string potomek[], string tab[])
{
	int rodzic1;
	int rodzic2;
	int pkt_przec;
	float w_lb_psl;

	for (int i = 0; i < ilosc; i += 2)
	{
		w_lb_psl = (float)rand() / (float)RAND_MAX;
		rodzic1 = (int)(rand() % ile_os);
		rodzic2 = (int)(rand() % ile_os);
		if (w_lb_psl <= pr_krzyz)
		{
			// losowanie punktów przecięcia
			pkt_przec = (int)(rand() % 7+1);
			potomek[i] = tab[rodzic1].substr(0, pkt_przec);
			potomek[i] += tab[rodzic2].substr(pkt_przec, 8);
			potomek[i + 1] = tab[rodzic2].substr(0, pkt_przec);
			potomek[i + 1] += tab[rodzic1].substr(pkt_przec, 8);
		}
		else
		{
			potomek[i] = tab[rodzic1];
			potomek[i + 1] = tab[rodzic2];
		}
	}
	//cout << w_lb_psl << endl;
}
// Mutacja 
void mutacja(string potomek[])
{
	float w_lb_psl;
	string tab_pot;
	for (int i = 0; i < ile_os; i++)
	{
		tab_pot = potomek[i];
		for (int y = 0; y < 8; y++)
		{
			w_lb_psl = (float)rand() /(float) RAND_MAX;
			if (w_lb_psl <= pr_mut) {
				if (tab_pot[y] == char(49))
				{
					tab_pot[y] = char(48);
				}
				else
				{
					tab_pot[y] = char(49);
				}
			}
		}
		potomek[i] = tab_pot;
	}
}
// Selekcja metoda koła ruletki 
void selekcja(int randomtab[], double fx[])
{
	double* px;
	px = new double[ile_os];
	int* tmp;
	tmp = new int[ile_os];
	double f_suma = 0;
	double los;
	double abc = 0;
	for (int i = 0; i < ile_os; i++)
	{
		fx[i] = (a * pow(randomtab[i], 2) + b * randomtab[i] + c);
	}
	for (int i = 0; i < ile_os; i++)
	{
		f_suma = f_suma + fx[i]; // sumaryczna wartość 

	}
	for (int i = 0; i < ile_os; i++)
	{
		px[i] = fx[i] / f_suma; // liczenie prawdopodobieństwa wybrania osobnika
	}

	for (int i = 0; i < ile_os; i++)
	{
		abc = 0;
		los = (float)rand() /(float)RAND_MAX; // generowanie liczby pseudolosowej
		for (int x = 0; x < ile_os; x++)
		{
			if (los >= abc && px[x] + abc > los)
			{
				tmp[i] = randomtab[x];
				break;
			}
			abc += px[x];
		}
	}
	for (int i = 0; i < ile_os; i++)
	{
		randomtab[i] = tmp[i];
	}
}


int main()
{
	int liczba = 0; 
	int osob = 0;
	int* randomtab;
	randomtab = new int[ile_os]; 
	double* fx;	
	fx = new double[ile_os]; 	
	double max;
	string* potomek;
	potomek = new string[ile_os];
	string* tab;	
	tab = new string[ile_os];
	
	ofstream zapis("Najlepsi.txt");

	srand(time(NULL));
	


	for (int i = 0; i < ile_wyn; i++)
	{
		generator(randomtab); //generowanie
		 
		
		for (int y = 0; y < lb_pop; y++) //lb_pop
		{
			zm_binarny(randomtab, tab); //wywołanie funkcji do konwersji na binar
			krzyzowanie(ile_os, potomek, tab); //krzyżowanie
			mutacja(potomek); //mutacja

		    for (int x = 0; x < ile_os; x++)
			{
				randomtab[x] = stoi(potomek[x], nullptr, 2); // do randomtab zamieniam konwersja na dziesiętny
			}
			selekcja(randomtab, fx); //selekcja
		}
		
		
		

		  max = 0;
		for (int i = 0; i < ile_os; i++)
		{
			fx[i] = (a * pow(randomtab[i], 2) + b * randomtab[i] + c);
			if (fx[i] > max)
			{
				max = fx[i];
				osob = i;
			}
		}
	    // Wyświetlenie wfunkcji kwadratowej oraz osobnika 
		std::cout << max << " " << randomtab[osob] << endl;
		//zapis do pliku dane.txt
		zapis << max << " " << randomtab[osob] << endl;
		
		 
		
		
	}
	zapis.close();
	
}