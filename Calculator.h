#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;


class Calculator {

public:

	Calculator(){};
	vector<string> InfixToRPN(string input);
	double racunajRPN(vector<string> stek);
	int prvenstvo(char operacija);
	bool jeloperacija(char c);
	double racunanje(double a, double b, char op);
	bool rucniDigit(string izraz);
	bool jelDobarRPN(vector<string> stek);
};