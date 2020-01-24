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

    // Nombre de suppliers
    int NSup;

    // Nombre de Périodes
    int NPer;

    // Backordering and holding costs
    double cb, ch;

    // Setup (odering) costs
    double* cs;

    // Unit price
    double* up;

    //Demand
    int* d;

    // lead-times limits
    int* LMax;
    int* LMin;

    // Imax
    int Imax;

    // TBO
    int bestmaxTBO;
    int bestminTBO;


    string dataFile_;

public:
    Data(int nbPeriods, int nbSuppliers, string dataFile);
    virtual ~Data();

    void parseData(string file);
    void ecriture(string  nom_fichier, string c , double* tab,int n);
    void ecriture(string  nom_fichier, string c , int* tab,int n);


    int getNPer();
    int getNSup();

    double getch();
    double getcb();


    // Setup (odering) costs
    double getSetup(int i);
    double* getcs();

    // Unit price
    double getPrice(int i);

    //Demand
    int getDemand(int i);
    int* getTabD();

    // lead-times limits
    int getLMax(int i);
    int getLMin(int i);
    int* getTabLMax();
    int* getTabLMin();

    // TBO
    int getMinTBO();
    int getMaxTBO();


    static void print(string s);
    static void print(string s, double d);

};



#endif /* DATA_HPP_ */
