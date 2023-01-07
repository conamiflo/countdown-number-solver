#include "Calculator.h"

using namespace std;

//Funkcija proverava koja operacija ima prednost,
//* i / imaju veci prioritet od + i - 
int Calculator::prvenstvo(char operacija) {

	if (operacija == '*' || operacija == '/') {
		return 1;
	}
	else {
		return 0;
	}

}

//Funkcija proverava da li je karakter operacija
bool Calculator::jeloperacija(char c) {

	char operacije[4] = { '+','-','*','/' };

	for (int i = 0; i < 4; i++) {
		if (c == operacije[i]) {
			return true;
		}
	}

	return false;

}

//Funkcija sluzi da proveri ako izraz ima vise karaktera(npr dvocifren broj)
//da li su svi brojevi
bool Calculator::rucniDigit(string izraz) {


	for (char ch : izraz) {
		if (!isdigit(ch)) {
			return false;
		}
	}

	return true;

}

//Funkcija broji brojeve i operacije u izrazu i vraca gresku ako izraz nije dobar
bool Calculator::jelDobarRPN(vector<string> stek) {

	int i = 0;

	//Ako je broj dodaje +1
	for (string s: stek) {
		if (rucniDigit(s)) {
			i++;
		}
	//Ako je operacije skida -2 pa dodaje +1
		else {
			i -= 2;
			i++;
		}

	}
	//Izraz nije dobar ako na kraju i nije 1
	if (i != 1) {
		return false;
	}
	else {
		return true;
	}

}

//Funkcija pretvara korisnikov unos u RPN notaciju kako bi se lakse racunao izraz
//koji ima zagrade.
vector<string> Calculator::InfixToRPN(string input) {

	vector<char> stek1;
	vector<char> operacije;
	vector<string> stek3; //Stek u koji se smestaju brojevi i operacije
	string izraz = "";	//String koji se koristi za dodavanje brojeva na stek,posto
						//ako je char stek ne mogu u njega da ubacim dvocifrene brojeve

	//Prolazimo kroz svaki karakter u stringu i dodajemo ga u odgovarajuci stek
	for (char s : input) {

		//Proverava da li je karakter broj,ako jeste dodaje ga na string izraz
		if (isdigit(s)) {
			izraz += s;
		}
		else {
			//Ako nije karakter proverava jel izraz prazan,ako nije dodaje ga na stek
			//Radi se zbog toga sto mogu biti 2 cifre jedna do druge u stringu tj dvocifreni broj
			if (izraz != "") {
				stek3.push_back(izraz);
				izraz = "";
			}
			//Proverava da li je karakter operacija,kao i prioritet odnosno da li je + ili * i pusuhje ga na stek
			if (jeloperacija(s)) {

				while (!operacije.empty() && jeloperacija(operacije.back()) && prvenstvo(s) <= prvenstvo(operacije.back())) {
					string temp = ""; temp += operacije.back();
					operacije.pop_back();
					stek3.push_back(temp);
				}
				operacije.push_back(s);

			}
			//Proverava da li su zagrade
			else if (s == '(') {
				operacije.push_back(s);
			}
			//Ako je stek prazan baca mu gresku zbog toga sto pre toga nije bila otvorena zagrada,
			//ako nije pushuje na stek sve operacije dok ne nadje otvorenu zagradu
			else if (s == ')') {

				if (!operacije.empty()) {
					while (operacije.back() != '(') {
						if (operacije.empty()) {
							throw "GRESKA";
						}
						string temp = ""; temp += operacije.back();
						stek3.push_back(temp);
						operacije.pop_back();
					}
					operacije.pop_back();
				}
				else {
					throw "GRESKA";
				}

			}
			else {
				throw "GRESKA";
			}

		}
		
	}
	//Dodajemo broj ako je ostao 
	if (izraz != "") {
		stek3.push_back(izraz);
		izraz = "";
	}

	//Dodajemo preostale operacije,ako je ostala zagrada baca gresku jer posle nece
	//biti zatvorene zagrade
	while (!operacije.empty()) {
		if (operacije.back() == '(') {
			throw "GRESKA";
		}
		string temp = ""; temp += operacije.back();
		stek3.push_back(temp);
		operacije.pop_back();
	}

	//Proverava da li je dobar postfiksni izraz,tj da li je dobar broj operacija i brojeva
	if (jelDobarRPN(stek3)) {
		return stek3;
	}
	else {
		throw "GRESKA";
	}


	return stek3;

}

//Funkcija racuna izraz u zavisnosti od operacije i vraca ga
double Calculator::racunanje(double i, double j, char op) {

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
		if (j == 0 || floor(i/j) != i/j) {
			throw "GRESKA";
		}
		rez = i / j;
		break;

	default:
		break;
	}

	return rez;

	
}

//Funkcija racuna korisnikov unos u RPN notaciji
double Calculator::racunajRPN(vector<string> stek) {

	vector<double> rezultati;
	double rez;

	try {
		//Skida brojeve i operacije sa steka i racuna,pa pushuje na stek rezultata
		for (string c : stek) {

			if (rucniDigit(c)) {
				rezultati.push_back(stod(c));
			}
			else if (jeloperacija(c[0])) {
				double j = rezultati.back(); rezultati.pop_back();
				double i = rezultati.back(); rezultati.pop_back();
				rez = racunanje(i, j, c[0]);
				rezultati.push_back(rez);
			}
		}

		//Ako je na steku ostalo vise od 1 broja ili manje baca gresku
		if (rezultati.size() > 1 || rezultati.size() < 1) {
			throw "GRESKA";
		}
		//Popuje broj sa steka koji predstavlja rezultat celog izraza
		return rezultati.back();
	}
	catch (...) {
		throw "GRESKA";
	}
	

}
