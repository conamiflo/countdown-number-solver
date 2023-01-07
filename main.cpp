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

//Funkcija prima vektor nasih brojeva koji se permutuje kroz rekurziju u kojoj fiksiramo prvi element,
//pa ostale elemente izmesamo i dodamo vektor u vektor permutacija i tako sve dok nam pozicija ne bude
//na poslednjem elementu u vektoru brojeva. Set sluzi kako bi uklonili duplikate ,prilikom mesanja u set
//dodajemo svaki broj i ukoliko on vec postoji preskacemo ga,a ukoliko ne postoji normalno radimo kao i pre.

void permutuj_brojeve(vector<vector<double>>& permutacije,vector<double>& brojevi, int pozicija) {

	if (pozicija == brojevi.size()-1) {
		permutacije.push_back(brojevi);
		return;
	}

	set<double> temp;

	for (int el = pozicija; el < brojevi.size(); el++) {

		if (temp.find(brojevi[el]) != temp.end()) {
			continue;
		}

		temp.insert(brojevi[el]);
		swap(brojevi[el], brojevi[pozicija]);
		permutuj_brojeve(permutacije, brojevi, pozicija + 1);
		swap(brojevi[el], brojevi[pozicija]);


	}

}

//Funkcija u kojoj se poziva funkcija permutuj_brojeve() koja permutuje vektor,
//i dodaje ga u vektor permutovani,funkcija vraca vektor vektora.

vector<vector<double>> permutacija_vektora(vector<double>& brojevi) {

	vector<vector<double>> permutovani;
	permutuj_brojeve(permutovani, brojevi, 0);
	return permutovani;
}


// Funkcija vraca sve moguce kombinacije operacija kroz 5 for petlji.

vector<vector<int>> permutacija_operacija() {

	vector<vector<int>> permutacije_op;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				for (int l = 0; l < 4; l++) {
					for (int x = 0; x < 4; x++) {

						vector<int> temp;
						temp.push_back(i); temp.push_back(j); temp.push_back(k);
						temp.push_back(l); temp.push_back(x);

						permutacije_op.push_back(temp);

					}
				}
			}
		}
	}

	return permutacije_op;

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


double pronadji_rezultat(vector<double> brojevi, double rezultat, double najblizi) {
	
	char operacije[4] = { '+','-','*','/' };

	for (int i = 0; i < brojevi.size()-1; i++) {

		double a = brojevi[i];
		vector<double> brojevi2 = brojevi;
		brojevi2.erase(brojevi2.begin());

		for (int j = i + 1; j < brojevi2.size(); j++) {

			double b = brojevi2[j];

			for (int k = 0; k < 4; k++) {

				double temp = racunaj(a, b, operacije[k]);
				cout << temp << endl;
				if (temp == 0) {
					continue;
				}
				if (temp == rezultat) {
					cout << "NASAO REZ!" << endl;
				}

				if (brojevi.size() > 2) {
					vector<double> brojevi3 = brojevi;
					brojevi3.erase(brojevi3.begin());
					brojevi3.erase(brojevi3.begin());
					brojevi3.push_back(temp);

					double r = pronadji_rezultat(brojevi3, rezultat, najblizi);

					if (r != NULL) {
						return temp;
					}

				}
				
			}
		}
	}


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




double pronadji_resenje(vector<double> brojevi, double rezultat) {


	
	vector<vector<double>> sve_permutacije = permutacija_vektora(brojevi);
	vector<vector<int>> sve_operacije = permutacija_operacija();
	vector<string> stek;

	double trazeni = rezultat;
	double najblizi = 9999;
	double rez;
	double rezf = 0;

	for (vector<double> vec : sve_permutacije) {

		for (vector<int> op : sve_operacije) {

			double rr = racunaj2(vec, op);
			cout << "Broj: " << rr << endl;
			if (rr == trazeni) {
				cout << "PRONASAO !!!!!!" << endl;
				return rr;
			}
		}
	}
	rezf = pronadji_rezultat(brojevi, trazeni, najblizi);



	//for (int i = 0; i < stek.size(); i++) {
	//	cout << stek[i] << endl;
	//}

	//cout << rezf;


	/*for (vector<double> broj : sve_permutacije) {
		for (vector<int> op : sve_operacije) {
			rez = pronadji_rezultat(broj, trazeni, najblizi,stek);
			cout << "REZ: " << najblizi << endl;
			if (rez == trazeni) {
				return rez;
			}
			else if (abs(trazeni - rez) < abs(trazeni-najblizi)) {
				najblizi = rez;
			}
		}
	}

	cout << "NAJBLIZI: " << najblizi << endl;
	return najblizi;*/

	return 0;

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
			
			for (string s : stek) {
				cout << s << " ";
			}

			cout << endl;
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
	
	vector<Broj> sve_igre = ucitaj_fajl("brojevi.txt");
	vector<double> vec = sve_igre[1].brojevi;
	vector<vector<double>> vec_perm = permutacija_vektora(vec);
	vector<vector<int>> op = permutacija_operacija();

	

	//cout << "Broj permutacija: " << vec_perm.size() << endl;
	double ac = pronadji_resenje(vec, 687);

	cout << ac << endl;


	/*cout << "====================== MOJ BROJ ======================" << endl << endl;

	string unet_fajl = unos_fajla();
	vector<Igra> sve_igre = ucitaj_fajl(unet_fajl);
	vector<Runda> sve_runde;

	for (int i = 0; i < sve_igre.size(); i++) {

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

		if (pobednik == "Nema pobednika") {
			pobednik = koJePobednik(resenjeA, resenjeB, odstupanjeA, odstupanjeB);
		}
		else {
			Runda runda(izrazA, izrazB, resenjeA, resenjeB, pobednik, odstupanjeA, odstupanjeB);
			sve_runde.push_back(runda);
		}

	}*/

}

