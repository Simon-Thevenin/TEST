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
    string TypeOfBudget;

public:
    Data(int nbPeriods, int nbSuppliers, string dataFile);
    virtual ~Data();

    void parseData(string file);
    void ecriture(string  nom_fichier, string c , double* tab,int n);
    void ecriture(string  nom_fichier, string c , int* tab,int n);

    void Affich_Results(string nom_fichier,   int gamma1, int gamma2, int gamma3,  string method, int** X, double RC, double t, double g, double BIIP, int IterBestSol, double InvCost, double AvgInv, double PurshCost, double backCost, double AvgBavk );


    int getNPer();
    int getNSup();

    double getch();
    double getcb();


    string getTypeOfBudget();

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
    int getTimeLimite();

};



#endif /* DATA_HPP_ */
