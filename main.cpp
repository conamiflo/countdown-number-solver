#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include "Broj.h"
#include <stack>
#include <iterator>
#include <set>
#include <limits>
#include "Calculator.h"
#include "Runda.h"
#include "Trazeni.h"

using namespace std;


//Funkcija ucitava brojeve za svaku rundu i smesta ih u vektor igara

vector<Broj> ucitaj_fajl(string ime_fajla) {
	ifstream in(ime_fajla);

	vector<Broj> vector_igre;
	int i = 1;
	while (!in.eof()) {

		double br1, br2, br3, br4, br5, br6, rezultat;
		in >> br1 >> br2 >> br3 >> br4 >> br5 >> br6 >> rezultat;

		vector<double> brojevi;
		brojevi.push_back(round(br1)); brojevi.push_back(round(br2)); brojevi.push_back(round(br3));
		brojevi.push_back(round(br4)); brojevi.push_back(round(br5)); brojevi.push_back(round(br6));

		if (i % 2 != 0) {
			Broj igra(brojevi, rezultat, "A");
			vector_igre.push_back(igra);
		}
		else {
			Broj igra(brojevi, rezultat, "B");
			vector_igre.push_back(igra);
		}

		i++;
	}
	return vector_igre;

}


//Funkcija proverava da li je korisnik uneo postojeci fajl koji sadrzi
//brojeve za svaku rundu

string unos_fajla() {

	string fajl;
	while (true) {

		cout << "Unesite ime fajla iz kog zelite da ucitate brojeve: ";
		cin >> fajl;
		ifstream in(fajl);

		if (!in) {
			cout << "Uneli ste pogresan fajl, pokusajte ponovo! " << endl;
		}
		else {
			break;
		}
	}
	return fajl;

}

//Funkcija racuna izraz u zavisnosti od operacije i vraca ga
double racunaj(double i, double j, char op) {
	double rez;

	switch (op)
	{
	case '+':
		rez = i + j;
		break;
	case '-':
		rez = i - j;
		break;
	case '*':
		rez = i * j;
		break;
	case '/':
		if (j == 0) {
			throw "GRESKA";
		}
		rez = i / j;
		break;

	default:
		break;
	}

	return rez;

}

string s(double br) {

	return to_string(br).substr(0, to_string(br).find('.'));
}

double rs(string izraz) {

	Calculator calc;
	vector<string> stek = calc.InfixToRPN(izraz);
	double rez = calc.racunajRPN(stek);

	return rez;
}

double rs_naj(vector<string> naj) {

	string s = "";
	for (string ss : naj) {
		s += ss;
	}
	Calculator calc;
	vector<string> stek = calc.InfixToRPN(s);
	double rez = calc.racunajRPN(stek);

	return rez;
}


bool pronadji_rezultat2(vector<string> brojevi, double rezultat,vector<string>& resenje, vector<string>& najblizi) {


	char operacije[4] = { '+','-','*','/' };

	for (int i = 0; i < brojevi.size()-1; i++) {
		double a = rs(brojevi[i]);

		for (int j = i + 1; j < brojevi.size(); j++) {
			double b = rs(brojevi[j]);
			
			for (int k = 0; k < 4; k++) {

				if (operacije[k] == '/' && ((a / b != floor(a / b)) || b == 1 || b == 0)) {
					continue;
				}
				else if (operacije[k] == '-' && b > a) {
					continue;
				}
				else if (operacije[k] == '*' && (b == 1 || a == 1)) {
					continue;
				}

				string res = "(" + brojevi[i] + operacije[k] + brojevi[j] + ")";
				double temp = racunaj(a, b, operacije[k]);
				if (temp == 0) {
					continue;
				}

				vector<string> novi;
				novi.push_back(res);

				for (int x = 0; x < brojevi.size(); x++) {
					if (x == i || x == j) {
						continue;
					}
					else {
						novi.push_back(brojevi[x]);
					}
				}

				if (temp == rezultat) {
					resenje = novi;
					return true;
				}
				else if (novi.size() == 1) {
					if (abs(rezultat - temp) < abs(rezultat - rs_naj(najblizi))) {
						najblizi = novi;
					}
				}

				if (pronadji_rezultat2(novi, rezultat, resenje, najblizi)) {
					return true;
				}

			}
		}
	}
	return false;
}

double racunaj2(vector<double> brojevi, vector<int> oper) {

	char operacije[4] = { '+','-','*','/' };
	string izraz = "";

	Calculator calc;

	for (int i = 0; i < 5; i++) {
		izraz += to_string(brojevi[i]).substr(0, to_string(brojevi[i]).find('.'));
		izraz += operacije[oper[i]];
	}
	izraz += to_string(brojevi[5]).substr(0, to_string(brojevi[5]).find('.'));

	cout << "IZRAZ: " << izraz << endl;

	try {
		vector<string> str = calc.InfixToRPN(izraz);

		for (string s : str) {
			cout << s << " ";
		}

		double dab = calc.racunajRPN(str);
		return dab;
	}
	catch (...) {
		return 99999;
	}

}




void pronadji_resenje(vector<double> brojevi, double rezultat) {

	vector<string> najblizi;
	najblizi.push_back("999+999");
	vector<string> resenje;

	vector<string> brojevi2;
	for (double br : brojevi) {
		brojevi2.push_back(s(br));
	}

	Calculator cal;

	if (pronadji_rezultat2(brojevi2, rezultat, resenje, najblizi)) {
		vector<string> stek = cal.InfixToRPN(resenje[0]);
		double res = cal.racunajRPN(stek);

		if (rezultat == res) {
			cout << "Racunar je dobio tacan broj: " << rezultat << " = " << resenje[0] << endl;
		}
	}
	else {
		vector<string> stek = cal.InfixToRPN(najblizi[0]);
		double res = cal.racunajRPN(stek);
		cout << "Racunar je dobio najblizi broj: " << res << " = " << najblizi[0] << endl;
	}

}

void printajBrojeve(Broj igra) {

	cout << "Broj koji se trazi je: " << igra.resenje << ", a mozete koristiti brojeve: ";
	for (double br : igra.brojevi) {
		cout << to_string(br).substr(0, to_string(br).find('.')) << " ";
	}
	cout << endl;
}

vector<string> korisnikIzraz() {

	vector<string> izraz_broj;
	string unos;
	double broj;

	while (true) {
		try {
			Calculator calc;

			string input;
			cin >> input;

			vector<string> stek = calc.InfixToRPN(input);

			double racun = calc.racunajRPN(stek);
			unos = input;
			broj = racun;

			cout << "Rezultat vaseg izraza: " << racun << endl;
			break;
		}
		catch (...) {
			cout << "Uneli ste pogresan izraz,unesite novi! " << endl;
		}
	}

	izraz_broj.push_back(unos);
	izraz_broj.push_back(to_string(broj));

	return izraz_broj;

}

string koJePobednik(double resenjeA, double resenjeB, double odstupanjeA, double odstupanjeB) {

	string pobednik;

	if (resenjeA == resenjeB) {
		cout << "Korisnici su uneli isti broj,runda je neresena!" << endl;
		pobednik = "Nereseno";
	}
	else {
		if (odstupanjeA < odstupanjeB) {
			pobednik = "Igrac A";
			cout << "Korisnik A je pobedio rundu!" << endl;
		}
		else if (odstupanjeB < odstupanjeA) {
			pobednik = "Igrac B";
			cout << "Korisnik B je pobedio rundu!" << endl;
		}
		else {
			pobednik = "Nereseno";
			cout << "Korisnici su jednako udaljeni od trazenog broja,runda je neresena!" <<endl;
		}
	}
	return pobednik;


}


int main() {
	


	cout << "====================== MOJ BROJ ======================" << endl << endl;

	string unet_fajl = unos_fajla();
	vector<Broj> sve_igre = ucitaj_fajl(unet_fajl);
	vector<Runda> sve_runde;

	for (int i = 0; i < sve_igre.size(); i++) {
		//pronadji_resenje(sve_igre[i].brojevi, sve_igre[i].resenje);

		string izrazA = "Nema izraza";
		string izrazB = "Nema izraza";
		double resenjeA = 9999;
		double resenjeB = 9999;
		string pobednik = "Nema pobednika";
		double odstupanjeA = 999;
		double odstupanjeB = 999;
		//vector<string> unosi_igraca;

		cout << "---------------------- Runda " << i+1 << " ----------------------"<< endl;
		printajBrojeve(sve_igre[i]);
		
		if (sve_igre[i].na_potezu == "A") {
			cout << "Igrac A: ";
			vector<string> unos = korisnikIzraz();
			izrazA = unos[0]; resenjeA = stod(unos[1]); odstupanjeA = abs(sve_igre[i].resenje - stod(unos[1]));

			if (stod(unos[1]) == sve_igre[i].resenje) {
				cout << "Uneli ste tacan broj! Korisnik A je pobedio rundu!" << endl;
				pobednik = "Igrac A";
			}
			else {
				cout << "Igrac B: ";
				vector<string> unosB = korisnikIzraz();
				izrazB = unosB[0]; resenjeB = stod(unosB[1]); odstupanjeB = abs(sve_igre[i].resenje - stod(unosB[1]));

				if (resenjeB == sve_igre[i].resenje) {
					cout << "Uneli ste tacan broj! Korisnik B je pobedio rundu!" << endl;
					pobednik = "Igrac B";
				}

			}
		}
		else {
			cout << "Igrac B: ";
			vector<string> unos = korisnikIzraz();
			izrazB = unos[0]; resenjeB = stod(unos[1]); odstupanjeB = abs(sve_igre[i].resenje - stod(unos[1]));

			if (stod(unos[1]) == sve_igre[i].resenje) {
				cout << "Uneli ste tacan broj! Korisnik B je pobedio rundu!" << endl;
				pobednik = "Igrac B";
			}
			else {
				cout << "Igrac A: ";
				vector<string> unosA = korisnikIzraz();
				izrazA = unosA[0]; resenjeA = stod(unosA[1]); odstupanjeA = abs(sve_igre[i].resenje - stod(unosA[1]));
				if (resenjeA == sve_igre[i].resenje) {
					cout << "Uneli ste tacan broj! Korisnik A je pobedio rundu!" << endl;
					pobednik = "Igrac A";
				}
			}
		}
		pronadji_resenje(sve_igre[i].brojevi, sve_igre[i].resenje);

		if (pobednik == "Nema pobednika") {
			pobednik = koJePobednik(resenjeA, resenjeB, odstupanjeA, odstupanjeB);
		}
		else {
			Runda runda(izrazA, izrazB, resenjeA, resenjeB, pobednik, odstupanjeA, odstupanjeB);
			sve_runde.push_back(runda);
		}

	}

}

