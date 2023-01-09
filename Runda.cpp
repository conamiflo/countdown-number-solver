#include "Runda.h"
#include <vector>
using namespace std;

//Konstruktor
Runda::Runda(string A, string B, double rA, double rB, string pobednik, 
			 double odA, double odB,string racIz,vector<double> br,double traz) :
	izrazA(A), izrazB(B), resenjeA(rA), resenjeB(rB), pobednik(pobednik), 
	odstupanjeA(odA), odstupanjeB(odB) , racunarIzraz(racIz), brojevi(br),trazeni(traz) {};