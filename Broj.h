#pragma once
#include <vector>
#include <string>
using namespace std;

class Broj {

public:

	vector<double> brojevi;		//Brojevi koji su na raspolaganju za racunanje
	double resenje;				//Broj koji se trazi
	string na_potezu;			//Koji igrac je na potezu

	Broj(vector<double> b, double r, string p);		//Konstruktor

};