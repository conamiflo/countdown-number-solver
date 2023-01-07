#pragma once
#include <string>

using namespace std;

class Runda {

public:
	string izrazA;
	string izrazB;
	double resenjeA;
	double resenjeB;
	string pobednik;
	double odstupanjeA;
	double odstupanjeB;

	Runda(string A, string B, double rA, double rB, string pobednik, double odA, double odB);


};