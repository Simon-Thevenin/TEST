#pragma once
#include <iostream>
#include <string>

#include <time.h>
#include <fstream>
#include <cstdlib>

#ifndef DATA_HPP_
#define DATA_HPP_

using namespace std;

class Data
{
	private:
		double cb, ch;

		// Nombre de suppliers
		int NSup;

        // Nombre de Périodes
		int NPer;

		string dataFile_;

	public:
		Data(string dataFile);
		virtual ~Data();

		void parseData(string file);
		void ecriture(const char* nom_fichier, int* tab,int n);
        double getch();
        double getcb();
		double getDemand(int t);
		double getSetup(int s);
		double getPrice(int s);
		int getLMin(int s);
		int getLMax(int s);
        void setch(double value);
        void setcb(double value);
		int getNPer();
		int getNSup();
		static void print(string s);
		static void print(string s, double d);
};


#endif /* DATA_HPP_ */
