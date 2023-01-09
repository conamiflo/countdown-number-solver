#pragma once
#include <string>
#include <vector>
using namespace std;

class Runda {

public:
	string izrazA;		//Izraz igraca A
	string izrazB;		//Izraz igraca B
	double resenjeA;	//Broj koji je dobio igrac A
	double resenjeB;	//Broj koji je dobio igrac B
	string pobednik;	//Ko je pobedio trenutnu rundu
	double odstupanjeA;	//Koliko broj koji je dobio korisnik A razlikuje od trazenog
	double odstupanjeB;	//Koliko broj koji je dobio korisnik A razlikuje od trazenog
	string racunarIzraz;
	vector<double> brojevi;
	double trazeni;

	Runda(string A, string B, double rA, double rB, //Konstruktor
		  string pobednik, double odA, double odB,
		  string racIz,vector<double> br,double traz);	


};