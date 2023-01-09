#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include "Broj.h"
#include "Calculator.h"
#include "Runda.h"


using namespace std;


//Funkcija ucitava brojeve za svaku rundu i smesta ih u vektor brojeva

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
//brojeve za svaku rundu,vraca string naziva fajla.

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

//Funkcija prima 2 broja i operaciju i racuna izraz u zavisnosti
// od operacije i vraca ga,baca gresku ukoliko dodje do deljenja 0.
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

//Funkcija pretvara double vrednost u string.
string s(double br) {

	return to_string(br).substr(0, to_string(br).find('.'));
}

//Funkcija racuna izraz pretvaranjem u RPN notaciju i izracunavanjem.
double rs(string izraz) {

	Calculator calc;
	vector<string> stek = calc.InfixToRPN(izraz);
	double rez = calc.racunajRPN(stek);

	return rez;
}

//Funkcija prima vektor koji sadrzi izraz koji je racunar pronasao,
//i izracunava ga, vraca njegovu vrednost.
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


bool pronadji_rezultat(vector<string> brojevi, double rezultat,vector<string>& resenje, vector<string>& najblizi) {


	char operacije[4] = { '+','-','*','/' };

	for (int i = 0; i < brojevi.size(); i++) {
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

				if (pronadji_rezultat(novi, rezultat, resenje, najblizi)) {
					return true;
				}

			}
		}
	}
	return false;
}


//Funkcija u kojoj racunar pronalazi resenje i ispisuje ga za broj koji se trazi.
//Kao parametre prima brojeve i trazeni broj, a na kraju samo ispisuje izraz na ekran.

void pronadji_resenje(vector<double> brojevi, double rezultat,string& izraz) {

	vector<string> najblizi;		//Vektor u kome ce se nalaziti izraz najblizeg broja
	najblizi.push_back("999+999");	
	vector<string> resenje;			//Vektor u kome ce se nalaziti izraz trazenog broja

	vector<string> brojevi2;		
	for (double br : brojevi) {
		brojevi2.push_back(s(br));
	}

	Calculator cal;

	//U koliko funkcija pronadji_rezultat vrati true,znaci da je racunar pronasao resenje,
	//te ga ispisuje na konzolu. U koliko resenje nije pronadjeno,funkcija ce izracunati
	//najblizi izraz i ispisati najblizi broj trazenom koji je racunar pronasao.

	if (pronadji_rezultat(brojevi2, rezultat, resenje, najblizi)) {
		vector<string> stek = cal.InfixToRPN(resenje[0]);	//Vektor u kome se nalazi izraz u RPN notaci
		double res = cal.racunajRPN(stek);					//Izracunat izraz

		if (rezultat == res) {
			cout << "Racunar je dobio tacan broj: " << rezultat << " = " << resenje[0] << endl;
			izraz = s(rezultat) + " = " + resenje[0];
		}
	}
	else {
		vector<string> stek = cal.InfixToRPN(najblizi[0]);
		double res = cal.racunajRPN(stek);
		cout << "Racunar je dobio najblizi broj: " << res << " = " << najblizi[0] << endl;
		izraz = s(res) + " = " + najblizi[0];
	}

}

//Funkcija printuje brojeve koje korisnik moze da korisni na konzolu,kao i broj koji se trazi.

void printajBrojeve(Broj igra) {

	cout << "Broj koji se trazi je: " << igra.resenje << ", a mozete koristiti brojeve: ";
	for (double br : igra.brojevi) {
		cout << to_string(br).substr(0, to_string(br).find('.')) << " ";
	}
	cout << endl;
}

//Funkcija proverava da li se broj nalazi u vektoru i brise ga nakon nalazenja.

bool provera(vector<double>& brojevi, double broj) {

	bool ima = false;
	for (int i = 0; i < brojevi.size(); i++) {

		if (broj == brojevi[i]) {
			ima = true;
			brojevi.erase(brojevi.begin() + i);
			break;
		}

	}

	return ima;

}

//Funkcija prima ponudjene brojeve i korisnikov uneti izraz i proverava
//da li je korisnik uneo samo brojeve koji su ponudjeni uz pomoc funkcije
// provera.

bool ispravni_brojevi(vector<double> brojevi, vector<string> unosRPN) {

	Calculator c;
	try {
		for (string str : unosRPN) {
			if (c.rucniDigit(str)) {
				if (!provera(brojevi, stod(str))) {
					return false;
				}
			}

		}
	}
	catch (...) {
		return false;
	}

	return true;
	
}


//Funkcija proverava izraz koji je korisnik uneo tako sto pokusava da ga pretvori u RPN
//notaciju i izracuna,u koliko je korisnik uneo los izraz ili uneo brojeve koji nisu
// ponudjeni catch ce uhvatiti gresku i korisnik mora ponovo da unese novi izraz. 
// Funkcija vraca broj koji je korisnik dobio, kao i izraz koji je uneo.

vector<string> korisnikIzraz(vector<double> brojevi) {

	vector<string> izraz_broj;
	string unos;
	double broj;

	while (true) {
		try {
			Calculator calc;

			string input;
			cin >> input;

			vector<string> stek = calc.InfixToRPN(input);
			if (!ispravni_brojevi(brojevi, stek)) {
				throw "Ne mozete koristiti brojeve koji nisu ponudjeni!";
			}

			double racun = calc.racunajRPN(stek);
			unos = input;
			broj = racun;
			cout << "Rezultat unetog izraza: " << racun << endl;
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


//Funkcija vraca string i ispisuje ko je od korisnika pobedio u zavisnosti od njihovih resenja.
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


void upis_rezultata(int A, int B,vector<Runda> runde) {

	ofstream upis("Rezultati.txt");
	//upis.open("Rezultati.txt");

	for (int i = 0; i < runde.size(); i++) {
		Runda runda = runde[i];

		upis << "Broj runde: " << i + 1 << endl;
		upis << "		Brojevi na raspolaganju: ";

		for (double br : runda.brojevi) {
			upis << s(br) << " ";
		}
		upis << endl;
		upis << "		Trazeni broj: " << s(runda.trazeni) << endl;
		upis << "		Igrac A:" << endl;
		upis << "			Dobijeni broj: " << s(runda.resenjeA) << endl;
		upis << "			Odstupanje: " << s(runda.odstupanjeA) << endl;
		upis << "			Izraz: " << runda.izrazA << endl;
		upis << "		Igrac B:" << endl;
		upis << "			Dobijeni broj: " << s(runda.resenjeB) << endl;
		upis << "			Odstupanje: " << s(runda.odstupanjeB) << endl;
		upis << "			Izraz: " << runda.izrazB << endl;
		upis << "		Pobednik runde: " << runda.pobednik << endl;
		upis << "		Racunar: " << runda.racunarIzraz << endl;

	}
	upis << "Broj osvojenih rundi: " << endl;
	upis << "		Igrac A: " << A << endl;
	upis << "		Igrac B: " << B << endl;

	if (A > B) {
		upis << "Pobednik igre je igrac A! " << endl;
	}
	else if (B > A) {
		upis << "Pobenik igre je igrac B!" << endl;
	}
	else {
		upis << "Igra je neresena! " << endl;
	}

	upis.close();
}



int main() {
	

	cout << "====================== MOJ BROJ ======================" << endl << endl;

	string unet_fajl = unos_fajla();
	vector<Broj> sve_igre = ucitaj_fajl(unet_fajl);
	vector<Runda> sve_runde;

	for (int i = 0; i < sve_igre.size(); i++) {

		string izrazA = "Nema izraza";
		string izrazB = "Nema izraza";
		double resenjeA = 9999;
		double resenjeB = 9999;
		string pobednik = "Nema pobednika";
		double odstupanjeA = 999;
		double odstupanjeB = 999;
		string racunar_izraz;

		cout << "---------------------- Runda " << i+1 << " ----------------------"<< endl;
		printajBrojeve(sve_igre[i]);
		
		if (sve_igre[i].na_potezu == "A") {
			cout << "Igrac A: ";
			vector<string> unos = korisnikIzraz(sve_igre[i].brojevi);
			izrazA = unos[0]; resenjeA = stod(unos[1]); odstupanjeA = abs(sve_igre[i].resenje - stod(unos[1]));

			if (stod(unos[1]) == sve_igre[i].resenje) {
				cout << "Uneli ste tacan broj! Korisnik A je pobedio rundu!" << endl;
				pobednik = "Igrac A";
			}
			else {
				cout << "Igrac B: ";
				vector<string> unosB = korisnikIzraz(sve_igre[i].brojevi);
				izrazB = unosB[0]; resenjeB = stod(unosB[1]); odstupanjeB = abs(sve_igre[i].resenje - stod(unosB[1]));

				if (resenjeB == sve_igre[i].resenje) {
					cout << "Uneli ste tacan broj! Korisnik B je pobedio rundu!" << endl;
					pobednik = "Igrac B";
				}

			}
		}
		else {
			cout << "Igrac B: ";
			vector<string> unos = korisnikIzraz(sve_igre[i].brojevi);
			izrazB = unos[0]; resenjeB = stod(unos[1]); odstupanjeB = abs(sve_igre[i].resenje - stod(unos[1]));

			if (stod(unos[1]) == sve_igre[i].resenje) {
				cout << "Uneli ste tacan broj! Korisnik B je pobedio rundu!" << endl;
				pobednik = "Igrac B";
			}
			else {
				cout << "Igrac A: ";
				vector<string> unosA = korisnikIzraz(sve_igre[i].brojevi);
				izrazA = unosA[0]; resenjeA = stod(unosA[1]); odstupanjeA = abs(sve_igre[i].resenje - stod(unosA[1]));
				if (resenjeA == sve_igre[i].resenje) {
					cout << "Uneli ste tacan broj! Korisnik A je pobedio rundu!" << endl;
					pobednik = "Igrac A";
				}
			}
		}
		pronadji_resenje(sve_igre[i].brojevi, sve_igre[i].resenje,racunar_izraz);

		if (pobednik == "Nema pobednika") {
			pobednik = koJePobednik(resenjeA, resenjeB, odstupanjeA, odstupanjeB);
		}

		Runda runda(izrazA, izrazB, resenjeA, resenjeB, pobednik, odstupanjeA, 
					odstupanjeB,racunar_izraz,sve_igre[i].brojevi,sve_igre[i].resenje);
		sve_runde.push_back(runda);

	}

	int A = 0;	//Broj rundi koji je igrac A osvojio
	int B = 0;	//Broj rundi koji je igrac A osvojio

	for (Runda runda : sve_runde) {
		if (runda.pobednik == "Igrac A") {
			A++;
		}
		else if (runda.pobednik == "Igrac B") {
			B++;
		}
	}

	if (A > B) {
		cout << "Pobednik igre je igrac A koji je osvojio " << A
			<< " poena, dok je igrac B osvojio " << B << " poena!" << endl;
	}
	else if (B > A) {
		cout << "Pobednik igre je igrac B koji je osvojio " << B
			<< " poena, dok je igrac B osvojio " << A << " poena!" << endl;
	}
	else {
		cout << "Igra je neresena, a oba igraca su osvojila po " << A << " poena!" << endl;
	}

	upis_rezultata(A, B, sve_runde);

}

