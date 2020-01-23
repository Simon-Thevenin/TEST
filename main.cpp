#include <iostream>
#include <time.h>
#include <fstream>
#include <cstdlib>
#include "xprb_cpp.h"
#include "xprs.h"
#include "ModelQuantity.h"
#include "SubProblem.h"
#include "Data.h"

using namespace std;
using namespace ::dashoptimization;
int mainOussama() {

    cout << "Hello, World!!" << endl;
    int NbPeriod =21;
    int NbSupplier =6;

    ifstream fichier("C://Users//oussama.ben-ammar//CLionProjects//TEST//1.txt", ios::in);  // on ouvre le fichier en lecture
    //ifstream fichier(argv[1], ios::in);  // on ouvre le fichier en lecture


    if (true) {

// Files
        cout<<"Hello Rennes !!2"<<endl;
        // Files
        string dataFile = "C://Users//oussama.ben-ammar//CLionProjects//TEST//1.txt";
        //string dataFile = argv[1];

        //NbPeriod = atoi(argv[2]) + 11;
        //NbSupplier = atoi(argv[3]);

        // On en a besoin pour afficher quelque chose
        string message;

// Paramétrage de la fonction rand
        srand(time(NULL));

        // Compteur temps
        double temps;
        clock_t start, end;

        //Create Data
        Data *data = new Data(NbPeriod, NbSupplier, dataFile);
        // Ouverture fichier resultat pour écriture
        ofstream fichierS("C://Users//oussama.ben-ammar//CLionProjects//TEST//resultat7.txt", ios::out | ios::app);
        //ofstream fichierS(argv[4], ios::out | ios::app);

        string FFile = "C://Users//oussama.ben-ammar//CLionProjects//TEST//resultat7.txt";
        //string FFile = argv[4];

        fichierS<<"Nombre de suppliers: "<<data->getNSup()<<endl;
        fichierS<<"Nombre de periods: "<<data->getNPer()<<endl;

        message = "Setup costs1 : ";
        data->ecriture(FFile, message,data->getcs(), data->getNSup());

        fichierS<<"Holding cost: "<<data->getch()<<endl;
        fichierS<<"Backlogging cost: "<<data->getcb()<<endl;

        message = "Demand : ";
        data->ecriture(FFile, message,data->getTabD(), data->getNPer());

        message = "Lmax : ";
        data->ecriture(FFile, message,data->getTabLMax(), data->getNSup());

        message = "Lmin : ";
        data->ecriture(FFile, message,data->getTabLMin(), data->getNSup());






        cout << "Create the model once:" << endl;
        ModelQuantity *ModQ = new ModelQuantity(data, 3);
        ModQ->BuildModel();


        cout << "create a Y to try!!!" << endl;
        int **givenY2 = new int *[data->getNSup()];
        for (int s = 0; s < data->getNSup(); s++) {
            givenY2[s] = new int[data->getNPer()];
            for (int t = 0; t < data->getNPer(); t++) {
                givenY2[s][t] = 1;
            }
        }

        ModQ->Solve(true, givenY2, false);
        cout << "Now with Y" << endl;
        ModQ->Solve(false, nullptr, false);
        int a;
        cin >> a;

    }
}
int main(int argc, char** argv){
    mainOussama();

}