#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include "Igra.h"
#include <stack>
#include <iterator>
#include <set>
#include <limits>
#include "Calculator.h"
#include "Runda.h"

using namespace std;

//Funkcija ucitava brojeve za svaku rundu i smesta ih u vektor igara

vector<Igra> ucitaj_fajl(string ime_fajla) {
	ifstream in(ime_fajla);

	vector<Igra> vector_igre;
	int i = 1;
	while (!in.eof()) {

		double br1, br2, br3, br4, br5, br6, rezultat;
		in >> br1 >> br2 >> br3 >> br4 >> br5 >> br6 >> rezultat;

		vector<double> brojevi;
		brojevi.push_back(round(br1)); brojevi.push_back(round(br2)); brojevi.push_back(round(br3));
		brojevi.push_back(round(br4)); brojevi.push_back(round(br5)); brojevi.push_back(round(br6));

		if (i % 2 != 0) {
			Igra igra(brojevi, rezultat, "A");
			vector_igre.push_back(igra);
		}
		else {
			Igra igra(brojevi, rezultat, "B");
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
double racunaj(double a, double b, char op) {
	double rez;

	switch (op)
	{
	case '+':
		rez = a + b;
		break;
	case '-':
		rez = a - b;
		break;
	case '*':
		rez = a * b;
		break;
	case '/':
		rez = a / b;
		break;

	default:
		break;
	}

	return rez;

}


vector<string> pronadji_rezultat(vector<double> brojevi, double rezultat, double najblizi, vector<string> stack) {
	
	vector<string> stek = stack;
	vector<double> br = brojevi;
	char operacije[4] = { '+','-','*','/' };
	double a;
	double b;

	for (int i = 0; i < br.size()-1; i++) {
		for (int j = i + 1; j < br.size(); j++) {
			a = br[i];
			b = br[j];
			br.erase(br.begin());
			br.erase(br.begin());

			for (int x = 0; x < 4; x++) {

				double temp = racunaj(a, b, operacije[x]);
				br.push_back(temp);
				cout << "TEMP:" << temp << endl;
				if (temp == rezultat) {
					cout << "REZULTAT: " << temp << endl;
					return stek;
				}
				if ((rezultat - temp) < 10) {
					cout << "NASAO BAR MANJI: " << temp << endl;
				}


				if (temp != 0 and pronadji_rezultat(br, rezultat, najblizi, stek).empty()) {
					stek.push_back("(" + to_string(a) + " " + operacije[x] + " " + to_string(b) + ")");
					return stek;

				}
				else {
					br.pop_back();
				}





				/*if (operacije[x] == '/' && (b == 0 || floor(a / b) != a / b)) {
					continue;
				}
				else if (operacije[x] == '-' && b > a) {
					continue;
				}

				double temp = racunaj(a, b, operacije[x]);
				if (temp == rezultat) {
					return stek;
				}

				if (temp == 0) {
					continue;
				}

				//double razlika = rezultat - temp;
				vector<double> br2 = br;
				br2.erase(br2.begin()); br2.erase(br2.begin());
				br2.push_back(temp);
				stek.push_back("(" + to_string(a) + " " + operacije[x] + " " + to_string(b) + ")");
				vector<string> stek2 = pronadji_rezultat(br2, rezultat, najblizi, stek);

				if (stek2.empty()) {
					stek.pop_back();
					br2.pop_back();
				}

				//if (razlika < 10) {
				//	for (int i = 0; i < stek.size(); i++) {
				//		cout << stek[i] << " ";
				//	}
				//}*/


			}

			br.push_back(br[j]);

		}

		br.push_back(br[i]);

	}



	/*vector<double> num = brojevi;
	vector<int> op = oper;
	double rez = rezultat;
	double naj = najblizi;
	char operacije[4] = { '+','-','*','/' };
	double temp = 0;

	for (int i = 0; i < op.size(); i++) {

		if (num.size() < 1) {
			return temp;
		}

		double a = num[0];
		double b = num[1];

		if (operacije[op[i]] == '/' && (b == 0 || floor(a / b) != a / b)) {
			continue;
		}
		else if (operacije[op[i]] == '-' && b > a) {
			continue;
		}
		temp = racunaj(a, b, operacije[op[i]]);
		num.erase(num.begin()); num.erase(num.begin());
		num.push_back(temp);
		//op.erase(op.begin());

		if (temp == rez) {
			return temp;
		}

	}

	for (int j = 0; j < op.size(); j++) {
		cout << operacije[op[j]] << " ";
	}
	cout << endl;



	return temp;*/

	/*for (int i = 0; i < op.size(); i++) {

		a = num[0];
		b = num[1];

		if (operacije[op[i]] == '/' && (b == 0 || floor(a / b) == a / b)) {
			continue;
		}
		else if (operacije[op[i]] == '-' && b > a) {
			continue;
		}

		double temp = racunaj(a, b, operacije[op[i]]);

		if (rez == temp) {
			return temp;
		}
		else if (abs(rez - temp) < naj) {
			naj = temp;
		}

		num.erase(num.begin()); num.erase(num.begin()); op.erase(op.begin());
		num.push_back(temp);

		if (num.size() < 1 || op.size() == 0) {
			return temp;
		}

		double as = pronadji_rezultat(num, rez, naj, op);

		cout << "U PETLJI " << as << endl;
	}*/
}


double pronadji_resenje(vector<double> brojevi, double rezultat) {


	
	vector<vector<double>> sve_permutacije = permutacija_vektora(brojevi);
	vector<vector<int>> sve_operacije = permutacija_operacija();
	vector<string> stek;

	double trazeni = rezultat;
	double najblizi = 9999;
	double rez;
	double rezf = 0;

	stek = pronadji_rezultat(brojevi, trazeni, najblizi, stek);

	for (int i = 0; i < stek.size(); i++) {
		cout << stek[i] << endl;
	}

	cout << rezf;


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

void printajBrojeve(Igra igra) {

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
			cout << "IZRACUNATO: " << racun << endl;
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

	}

	//vector<double> vec = sve_igre[1].brojevi;
	//vector<vector<double>> vec_perm = permutacija_vektora(vec);
	//vector<vector<int>> op = permutacija_operacija();


	//cout << "Broj permutacija: " << vec_perm.size() << endl;
	//double ac = pronadji_resenje(vec, 687);

	//cout << ac << endl;
}

