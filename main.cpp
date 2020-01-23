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


int main() {
    std::cout << "Hello, World!" << std::endl;

    ifstream fichier("FirstTest.dat", ios::in);  // on ouvre le fichier en lecture
//ifstream fichier(argv[1], ios::in);  // on ouvre le fichier en lecture

    if (true) {
// Files
        string dataFile = "FirstTest.dat";
//string dataFile = argv[1];

//Create Data
        Data *data = new Data(dataFile);

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