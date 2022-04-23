#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <ctime>
#include <algorithm>
#include <random>
using namespace std;

struct odpowiedz {
	string desc;
	bool isGood;
};

class CPytanie {
public:
	string poprawna;
	string pytanie;
	vector<odpowiedz> odpowiedzi;
	int liczba_wyswietlen;

	CPytanie(string pyt) : pytanie(pyt), liczba_wyswietlen(0) {}
	void DodajOdp(string& odp) {
		odpowiedz temp;
		temp.desc = odp;
		temp.isGood = false;
		odpowiedzi.push_back(temp);
	}
	void Wyswietlono() { liczba_wyswietlen++; }
};

int pyt_sort(const CPytanie& pyt1, const CPytanie& pyt2) {
	return pyt1.liczba_wyswietlen < pyt2.liczba_wyswietlen;
}

int bye(int status = 0) {
	if (status != 0) cout << "BLAD WCZYTYWANIA PLIKU\n";
	cout << "Naura\n";

	string stop;
	getline(cin, stop);

	return status;
}

string& napraw_ogonki(string& napis) {
#if defined(WINDOWS) || defined(WIN32) || defined(_WIN32)
	int l = napis.size();
	for (int i = 0; i < l; ++i) {
		if (napis[i] < 0) {
			switch (napis[i]) {
			case -54: napis[i] = 168; break;
			case -81: napis[i] = 189; break;
			case -113: napis[i] = 141; break;
			case -58: napis[i] = 143; break;
			case -116: napis[i] = 151; break;
			case -47: napis[i] = 227; break;
			case -45: napis[i] = 224; break;
			case -93: napis[i] = 157; break;
			case -91: napis[i] = 164; break;
			case -71: napis[i] = 165; break;
			case -22: napis[i] = 169; break;
			case -97: napis[i] = 171; break;
			case -26: napis[i] = 134; break;
			case -100: napis[i] = 152; break;
			case -77: napis[i] = 136; break;
			case -65: napis[i] = 190; break;
			case -15: napis[i] = 228; break;
			case -13: napis[i] = 162; break;
			}
		}
	}
#endif
	return napis;
}

int main(int argc, char* argv[]) {
	cout << "ZEBY TEN TESTER DZIALAL NALEZY BAZA.TXT ZAPISYWAC Z ENKODOWANIEM ANSII\n";
	cout << "Tester ver 1.1" << endl;
	cout << "Lemm @ 2012 in association with DS @ 2022" << endl;
	vector<CPytanie> pytania;
	ifstream plik("baza.txt", ifstream::in);
	string temp, odp, ver;
	int lodpowiedzi;

	if (!plik.good()) return bye(1);
	while (plik.good()) {
		getline(plik, temp);
		if (!temp.empty()) {
			// Stworzenie nowego pytania o danej tresci
			CPytanie nowe(napraw_ogonki(temp));
			plik >> lodpowiedzi;

			// wyciecie entera
			getline(plik, temp);
			int i = 0;
			// Dodawanie nowych odpowiedzi
			for (i = 0; i < lodpowiedzi && plik.good(); i++) {
				getline(plik, temp);
				nowe.DodajOdp(napraw_ogonki(temp));
			}
			if (i == lodpowiedzi) {
				// wziecie calej odpowiedzi
				getline(plik, temp);
				for (int i = 0; i < temp.size(); i++) {
					if (temp[i] > 96 && temp[i] < 105){
						int index = (int) temp[i] - 97;
						nowe.odpowiedzi[index].isGood = true;
					}
				}
				pytania.push_back(nowe);
			}
		}
	}

	cout << "Nacisnij enter jezeli jestes gotow na 1sze pytanie\n";
	string ans;
	getline(cin, ans);
	int nrpyt = 0, poprawne = 0, indeks = 0;
	srand(time(NULL));
	auto rng = std::default_random_engine{};
	while (true) {
		cout << "\nTwoj aktualny wynik to: " << (double)((double)poprawne / (double)(nrpyt == 0 ? 1 : nrpyt)) * 100.0f << "% (" << poprawne << "/" << nrpyt << ")\n\n";
		//niech wybiera pytania najmniej razy pokazane, lub ze zlymi odp.

		sort(pytania.begin(), pytania.end(), pyt_sort);
		int los = rand() % (pytania.size()) / 4;

		cout << pytania[los].pytanie << "\n";
		int lpyt = pytania[los].odpowiedzi.size();

		std::shuffle(std::begin(pytania[los].odpowiedzi), std::end(pytania[los].odpowiedzi), rng);

		for (int i = 0; i < lpyt; i++) cout << (char)(i + 'a') << ". " << pytania[los].odpowiedzi[i].desc << "\n";

		getline(cin, ans);

		int sum = 0;
		for (int i = 0; i < pytania[los].odpowiedzi.size(); i++) {
			if (pytania[los].odpowiedzi[i].isGood) sum++;
		}

		bool check = false;
		for (int i = 0; i < ans.size(); i++) {
			if(ans[i] > 96 && ans[i] < 105){
				int index = (int)ans[i] - 97;
				if (pytania[los].odpowiedzi[index].isGood) {
					sum--;
					continue;
				}
				else {
					cout << "To niestety zla odpowiedz. Poprawne to: \n";
					for (int j = 0; j < pytania[los].odpowiedzi.size(); j++) {
						if (pytania[los].odpowiedzi[j].isGood)
							cout << "      " << pytania[los].odpowiedzi[j].desc << "\n";
					}
					check = true;
					break;
				}
			}
		}

		if (sum > 0 && !check) {
			cout << "To niestety zla odpowiedz. Poprawne to: \n";
			for (int j = 0; j < pytania[los].odpowiedzi.size(); j++) {
				if (pytania[los].odpowiedzi[j].isGood) {
					cout << "      " << pytania[los].odpowiedzi[j].desc << "\n";
				}
			}
			check = true;
		}
		else if(!check) {
			cout << "Brawo! To byla poprawna odpowiedz.\n";
			poprawne++;
		}

		pytania[los].Wyswietlono();
		nrpyt++;
	}
	bye(0);
	return 0;
}
