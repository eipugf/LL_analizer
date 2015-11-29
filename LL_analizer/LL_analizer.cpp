// LL_analizer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <conio.h>
#include <locale.h>
#include "LLAnalyzer.h"


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "russian");
	string im = "";
	string current;
	ifstream file;
	file.open("input.java");
	while (!file.eof()){
		getline(file, current);
		im += current + "\n";
	}
	file.close();
	int code = 0;
	IM t;
	strcpy_s(t, im.c_str());
	LLAnalyzer *analizer = new LLAnalyzer();
	analizer->analyze(t);
	cout << "Код распознан! Ошибок нет!";
	_getch();
	return 0;
}

