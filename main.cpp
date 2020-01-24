#include <iostream>
#include <time.h>
#include <fstream>
#include <cstdlib>
#include "xprb_cpp.h"
#include "xprs.h"
#include "ModelQuantity.h"
#include "SubProblem.h"
#include "Data.h"
#include "FGenetic.h"
#include "Solution.h"
#define AffichAG 1
#include "GRASP.h"

using namespace std;
using namespace ::dashoptimization;

// résoudee probleme nouv gen
// penser  vider la mémoire partout

string pathfile = "..//";
//string pathfile =  "C://Users//oussama.ben-ammar//CLionProjects//TEST//";

void runExact(string file, int NbPeriod, int NbSupplier, double gamma)
{
    string Thefile = pathfile+file;
    ifstream fichier( Thefile, ios::in);
    Data *data = new Data(NbPeriod, NbSupplier, Thefile);
    ModelQuantity* ModQ= new ModelQuantity(data, gamma);
    ModQ->BuildModel();

    int** givenY2 = new int*[data->getNSup()];
    for(int s=0; s<data->getNSup(); s++)
    {
        givenY2[s] =  new int[data->getNPer()];
        for(int t=0; t<data->getNPer(); t++)
        {
            givenY2[s][t] =1;
        }
    }

    ModQ->Solve(true, givenY2, false, 0.01);
    double cost = ModQ->Solve(false, nullptr, false, 0.001);
    string FFile = pathfile + "resultat7.txt";
    data->Affich_Results(FFile, givenY2, cost, ModQ->LastRunning, -1, -1, -1,ModQ->GetInventoryCosts(), ModQ->GetAvgInventory(), ModQ->GetPurshasingCosts(), ModQ->GetBackorderCosts(), ModQ->GetAvgtBackorder());
    cout<<"optimal cost::::"<<cost<<endl;
    cout<<"Inv Cost:"<<ModQ->GetInventoryCosts()<<" Avg Inv:"<<ModQ->GetAvgInventory()<<" Order Cost:"<<ModQ->GetOrderingCosts()<<endl;
    cout<<"Back Cost:"<<ModQ->GetBackorderCosts()<<" Avg Back:"<<ModQ->GetAvgtBackorder()<<" Pursh cost:"<<ModQ->GetPurshasingCosts()<<endl;

}

void runGrasp(string file, int NbPeriod, int NbSupplier, double gamma)
{
    string Thefile = pathfile+file;
    ifstream fichier( Thefile, ios::in);
    Data *data = new Data(NbPeriod, NbSupplier, Thefile);
    GRASP* g = new GRASP(data, gamma);
    g->solve();
}


void runDeterministic(string file, int NbPeriod, int NbSupplier, int gamma)
{
    string Thefile = pathfile+file;
    ifstream fichier( Thefile, ios::in);
    Data *data = new Data(NbPeriod, NbSupplier, Thefile);
    int* Lmean = new int[data->getNSup()+1];
    for(int s= 1; s<=data->getNSup(); s++)
    {
        Lmean[s] = (int)((data->getLMax(s-1) + data->getLMin(s-1))/2);
    }
    double*** delta = new double**[data->getNPer()+1];

    for(int tau=1; tau<=data->getNPer(); tau++)
    {
        delta[tau] = new double*[data->getNPer()+1];
        for(int t=1; t<=data->getNPer(); t++)
        {
            delta[tau][t] = new double[data->getNSup()+1];
            for(int s=1; s<=data->getNSup(); s++)
            {
                if (tau <= t -Lmean[s])
                    delta[tau][t][s] = 1;
                else
                    delta[tau][t][s] = 0;
            }
        }
    }

    ModelQuantity* mod = new ModelQuantity(data,-1 );
    mod->BuildModel();
    mod->AddScenario(delta);
    mod->Solve(false, nullptr, true, 0.01);
    /*************************EVALUATE THE COST****************************/
    double ** associatedquantities =  mod->getQuantities();

    SubProblem* ModSub = new SubProblem(data, gamma);

    ModSub->BuildModel();



    ModSub->getWorstCaseDelta(associatedquantities);
    double cost=  ModSub->getAssociatedCost() +mod->totalsetupcosts + mod->GetPurshasingCosts();

    cout<<"Deterministic cost::::"<<cost<<endl;
    cout<<"Inv Cost:"<<ModSub->GetInventoryCosts()<<" Avg Inv:"<<ModSub->GetAvgInventory()<<" Order Cost:"<<mod->GetOrderingCosts()<<endl;
    cout<<"Back Cost:"<<ModSub->GetBackorderCosts()<<" Avg Back:"<<ModSub->GetAvgtBackorder()<<" Pursh cost:"<<mod->GetPurshasingCosts()<<endl;

}

int mainSimon() {
    runExact("1.txt", 21, 6,3);
    runGrasp("1.txt", 21, 6, 3);
    runDeterministic("1.txt", 21, 6, 3);
}

double* fitness;
double* fitness2;
double* fitness_croismut;
// Fonction qu'on a besoin pour classer les individus
int echanger (const void *a, const void *b){

    if ( fitness[* (int *)a] < fitness[* (int *)b] )
        return -1 ;
    else if (fitness[* (int *)a] == fitness[* (int *)b])
        return 0;
    else return 1;

}

int mainOussama() {
    int gamma = 3;
    int NbPeriod = 21;
    int NbSupplier =6;
    string file = pathfile + "1.txt";
    ifstream fichier(file, ios::in);  // on ouvre le fichier en lecture
    //ifstream fichier(argv[1], ios::in);  // on ouvre le fichier en lecture

    if(fichier) {
// Files

        // Files
        string dataFile =file;
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

        // Appeler la classe Algo Gen
        FGenetic ag(data, gamma);

        // Ouverture fichier resultat pour écriture
        ofstream fichierS(pathfile+"resultat7.txt", ios::out | ios::app);
        //ofstream fichierS(argv[4], ios::out | ios::app);

        string FFile = pathfile + "resultat7.txt";
        //string FFile = argv[4];

        /*fichierS << "Nombre de suppliers: " << data->getNSup() << endl;
        fichierS << "Nombre de periods: " << data->getNPer() << endl;

        message = "Setup costs1 : ";
        data->ecriture(FFile, message, data->getcs(), data->getNSup());

        fichierS << "Holding cost: " << data->getch() << endl;
        fichierS << "Backlogging cost: " << data->getcb() << endl;

        message = "Demand : ";
        data->ecriture(FFile, message, data->getTabD(), data->getNPer());

        message = "Lmax : ";
        data->ecriture(FFile, message, data->getTabLMax(), data->getNSup());

        message = "Lmin : ";
        data->ecriture(FFile, message, data->getTabLMin(), data->getNSup());*/

/** ALGO GEN ******************************************************************************************/


        /** Paramètres de l'AG*****************************/
        // Nombre de chromosomes
        int nb_chr=60;

        // Tx croisement et mutation
        int pc,pm;

        // Nombre de générations
        int nb_gen;

        // Time limit
        int TimeLimit = data->getTimeLimite();

        // Nombre de generations
        nb_gen=10000;

        // Taux de mutation et de croisement
        pc = 90;
        pm = 2;
        /**************************************************/

        /************* Declarations des tableaux et des matrices *******/
        // Simplification d'ecrtiture
        int NbS = data->getNSup();
        int NbP = data->getNPer();


        // Matrice 3D contenant les individus d'une génération
        int***nouv_gen ;
        int***nouv_gen1 ;
        int***nouv_gen2 ;
        int***nouv_gen3 ;
        int*** par_pop;
        int*** fils1;
        int*** fils2;
        int*** fils3;
        int essai;

        int* indices1;
        int* indices2;
        double MeillsolPopIni;
        double CoutIter=0;
        int Iteration=0;

        // Nombre d'individus qui ont subi du croisement
        int nbcc;

        // Nombre d'indiv qui ont subi une mutation
        int nbcm;

        // Nombre d'individus qui peuvent subir mutation et taille population
        int selection1_2=nb_chr+nb_chr/2;
        int selection_2=nb_chr/2;


        indices1 = new int[nb_chr];
        indices2 = new int[nb_chr*2];
        nouv_gen=new int**[nb_chr];
        for(int i=0;i<nb_chr;i++)
        {
            nouv_gen[i]=new int*[NbS];
        }
        for(int i=0;i<nb_chr;i++)
        {
            for(int j=0;j<NbS;j++)
            {
                nouv_gen[i][j]=new int[NbP];
            }
        }

        nouv_gen1 = new int**[selection1_2+selection_2];
        nouv_gen2 = new int**[selection1_2+selection_2];
        nouv_gen3 = new int**[nb_chr];
        for(int i=0;i<selection1_2+selection_2;i++)
        {
            nouv_gen1[i]=new int*[NbS];
            nouv_gen2[i]=new int*[NbS];
        }
        for(int i=0;i<selection1_2+selection_2;i++)
        {
            for(int j=0;j<NbS;j++)
            {
                nouv_gen1[i][j]=new int[NbP];
                nouv_gen2[i][j]=new int[NbP];
            }
        }
        for(int i=0;i<nb_chr;i++)
        {
            nouv_gen3[i]=new int*[NbS];
        }
        for(int i=0;i<nb_chr;i++)
        {
            for(int j=0;j<NbS;j++)
            {
                nouv_gen3[i][j]=new int[NbP];
            }
        }


        // Tableau évaluations
        fitness=new double[selection1_2+selection_2];
        fitness2=new double[nb_chr*2];
        fitness_croismut=new double[selection_2];

        par_pop=new int**[selection_2];
        fils1=new int**[selection_2];
        fils2=new int**[selection_2];
        fils3=new int**[selection_2];
        for(int i=0;i<selection_2;i++)
        {
            par_pop[i]=new int*[NbS];
            fils1[i]=new int*[NbS];
            fils2[i]=new int*[NbS];
            fils3[i]=new int*[NbS];
        }

        for(int i=0;i<selection_2;i++)
        {
            for(int j=0;j<NbS;j++)
            {
                par_pop[i][j]=new int[NbP];
                fils1[i][j]=new int[NbP];
                fils2[i][j]=new int[NbP];
                fils3[i][j]=new int[NbP];
            }
        }
        /******************************************************************************/
        /** Lancement des iterations **************************************************/
        // 0 pour ne pas tourner l'Algo Gen
        int goo=1;
        if (goo==1) {
            //Initialiser le compteur CPU times
            start = clock();
            srand(time(NULL));
            temps = 0;

            /** Générer la population initial et l'évaluer    **/
            ag.PopInit(nb_chr, nouv_gen, fitness, data);

            /** On lance l'AG   **/
             int gen=0;

             // meilleur indiv population initiale
            MeillsolPopIni= ag.my_min1double(fitness, nb_chr);


             while ((temps < TimeLimit) && (gen<nb_gen)){
                //----------------------- selection de (selection_2) meilleurs chromosomes: parents --------------------
                // Un trie selon les fitness
                for (int i = 0; i < nb_chr; i++) {
                    indices1[i] = i;
                }
                qsort(indices1, nb_chr, sizeof(int), echanger);

                // On les récupère
                //----------------------- Parents --------------------
                for (int i = 0; i < selection_2; i++) {
                    for (int j = 0; j < NbS; j++) {
                        for (int k = 0; k < NbP; k++) {
                            essai = nouv_gen[indices1[i]][j][k];
                            par_pop[i][j][k] = essai;
                        }
                    }
                }
                //Affichage meilleurs parents:
                if (AffichAG == 0) {
                    fichierS << gen << " Meilleurs (nchr/2) parents ds nouv_gen " << endl;
                    for (int i = 0; i < selection_2; i++) {
                        for (int j = 0; j < NbS; j++) {
                            for (int k = 0; k < NbP; k++) {
                                fichierS << par_pop[i][j][k] << " ";
                            }
                            fichierS << endl;
                        }
                        fichierS << " fitness: " << fitness[indices1[i]] << endl;
                    }
                }
                //----------------------- generation des (selection_2) valeurs distinctes--
                ag.Cross(&nbcc, pc, fils1, selection_2, nb_chr, par_pop, fitness_croismut, data);

                // On génére nb_c chromosomes (individus)
                if (AffichAG == 0) {
                    fichierS << "Nombre de fils issus du croisement: " << nbcc * 2 << endl;
                    for (int i = 0; i < nbcc + 1; i++) {
                        for (int j = 0; j < data->getNSup(); j++) {
                            for (int k = 0; k < data->getNPer(); k++) {
                                fichierS << fils1[i][j][k] << " ";
                            }
                            fichierS << endl;
                        }
                        fichierS << " fitness: " << fitness_croismut[i] << endl;
                    }
                }
                //+++++++++++++++++++++++++++++++++++++++MUTATION+++++++++++++++++++++++++++++++++++
                // Mutation sur les commandes
                ag.Mutation(&nbcm, pm, fils2, selection_2, nb_chr, nouv_gen, indices1, data);

                if (AffichAG == 0) {
                    fichierS << "nombre dindiv mutes: " << nbcm << endl;
                    for (int i = 0; i < nbcm; i++) {
                        for (int j = 0; j < data->getNSup(); j++) {
                            for (int k = 0; k < data->getNPer(); k++) {
                                fichierS << fils2[i][j][k] << " ";
                            }
                            fichierS << endl;
                        }
                        fichierS << endl;
                    }
                }
                //+++++++++++++++++++++++++++++++++++++++SELECTION NOUVELLE GENERATION+++++++++++++++++++++++++++++++++++
                // on récupère la génération initiale
                for (int i = 0; i < nb_chr; i++) {
                    for (int j = 0; j < data->getNSup(); j++) {
                        for (int k = 0; k < data->getNPer(); k++) {
                            nouv_gen1[i][j][k] = nouv_gen[i][j][k];
                        }
                    }
                }
                // on rajoute N/2 individus à la population choisie: 3N/2
                for (int i = nb_chr; i < nbcm + nb_chr; i++) {
                    for (int j = 0; j < data->getNSup(); j++) {
                        for (int k = 0; k < data->getNPer(); k++) {
                            nouv_gen1[i][j][k] = fils2[i - nb_chr][j][k];
                        }
                    }
                }

                for (int i = 0; i < nbcc * 2; i++) {
                    for (int j = 0; j < data->getNSup(); j++) {
                        for (int k = 0; k < data->getNPer(); k++) {
                            fils3[i][j][k] = fils1[i][j][k];
                            nouv_gen1[i + nbcm + nb_chr][j][k] = fils3[i][j][k];
                        }
                    }
                    fitness[i + nbcm + nb_chr] = fitness_croismut[i];
                }

                if (AffichAG == 0) {
                    fichierS << "Toute la population: " << nbcm + nb_chr + nbcc * 2 << endl;
                    for (int i = 0; i < nbcm + nb_chr + nbcc * 2; i++) {
                        for (int j = 0; j < data->getNSup(); j++) {
                            for (int k = 0; k < data->getNPer(); k++) {
                                fichierS << nouv_gen1[i][j][k] << " ";
                            }
                            fichierS << endl;
                        }
                        fichierS << endl;
                    }
                }

                // on évolue les fils issus de la mutation //
                //#pragma omp parallel for private(i) schedule(dynamic) reduction(+:solopt)
                for (int i = nb_chr; i < nbcm + nb_chr; i++) {
                    fitness[i] = ag.RCOST(nouv_gen1, i, data);
                }
                for (int i = 0; i < nbcm + nb_chr + nbcc * 2; i++) {
                    indices2[i] = i;
                }
                qsort(indices2, nbcm + nb_chr + nbcc * 2, sizeof(int), echanger);

                for (int i = 0; i < nbcm + nb_chr + nbcc * 2; i++) {
                    nouv_gen2[i] = nouv_gen1[indices2[i]];
                }
                for (int i = 0; i < nbcm + nb_chr + nbcc * 2; i++) {
                    fitness2[i] = fitness[indices2[i]];
                }
                for (int i = 0; i < nbcm + nb_chr + nbcc * 2; i++) {
                    fitness[i] = fitness2[i];
                }

                for (int i = 0; i < nb_chr; i++) {
                    for (int j = 0; j < data->getNSup(); j++) {
                        for (int k = 0; k < data->getNPer(); k++) {
                            nouv_gen3[i][j][k] = nouv_gen2[i][j][k];
                            nouv_gen[i][j][k] = nouv_gen3[i][j][k];
                        }
                    }
                }

                if (AffichAG == 0) {
                    fichierS << "Toute la population !!: " << nb_chr << endl;
                    for (int i = 0; i < nb_chr; i++) {
                        for (int j = 0; j < data->getNSup(); j++) {
                            for (int k = 0; k < data->getNPer(); k++) {
                                fichierS << nouv_gen[i][j][k] << " ";
                            }
                            fichierS << endl;
                        }
                        fichierS << endl;
                    }
                }
                for (int i = 0; i < nb_chr; i++) {
                    for (int j = 0; j < data->getNSup(); j++) {
                        for (int k = 0; k < data->getNPer(); k++) {
                            nouv_gen2[i][j][k] = 0;
                        }
                    }
                }
                for (int i = 0; i < nb_chr * 2; i++) {
                    for (int j = 0; j < data->getNSup(); j++) {
                        for (int k = 0; k < data->getNPer(); k++) {
                            nouv_gen1[i][j][k] = 0;
                        }
                    }
                }
                if (AffichAG == 0) {
                    fichierS << "Toute la population !?!: " << nb_chr << endl;
                    for (int i = 0; i < nb_chr; i++) {
                        for (int j = 0; j < data->getNSup(); j++) {
                            for (int k = 0; k < data->getNPer(); k++) {
                                fichierS << nouv_gen[i][j][k] << " ";
                            }
                            fichierS << endl;
                        }
                        fichierS << endl;
                    }
                }

                 // iteration de la meilleure solution
                 if (gen==0)
                 {

                     CoutIter=fitness[0];
                 }
                 if (CoutIter > fitness[0])
                 {
                     //fichierS<<"yes ! "<<CoutIter<<" "<<fitness[0]<<" ";
                     CoutIter=fitness[0];
                     Iteration=gen;
                 }

                /*fichierS << "Meilleure_solution a la generation: " << gen << endl;
                for (int j = 0; j < data->getNSup(); j++) {
                    for (int k = 0; k < data->getNPer(); k++) {
                        fichierS << nouv_gen[0][j][k] << " ";
                    }
                    fichierS << endl;
                }
                fichierS << "fitness: ---------------------> " << fitness[0] << endl;
                fichierS << endl;*/


                // arreter compteur temps
                end = clock();
                // Calculer temps de calcul
                temps = (double) (end - start) / CLOCKS_PER_SEC;
                gen++;
            }
            // Calculer temps de calcul
            temps = (double) (end-start)/ CLOCKS_PER_SEC;

            // Afficher meilleure solution
            /*fichierS<<"--------------> Meilleure_solution finale: "<<endl;
            for(int j=0;j<data->getNSup();j++)
            {
                for(int k=0;k< data->getNPer() ; k++)
                {
                    fichierS<<nouv_gen[0][j][k]<<" " ;
                }
                fichierS<<endl;
            }
            fichierS<<endl;
            fichierS <<fitness[0]<<" TcPU: "<<temps <<endl;*/
             ag.ModQ->Solve(true, nouv_gen[0], false, 0.01);
            double cost = ag.ModQ->Solve(false, nullptr, false, 0.001);

            data->Affich_Results(FFile, nouv_gen[0], fitness[0], temps, gen, MeillsolPopIni, Iteration+1,ag.ModQ->GetInventoryCosts(), ag.ModQ->GetAvgInventory(), ag.ModQ->GetPurshasingCosts(), ag.ModQ->GetBackorderCosts(), ag.ModQ->GetAvgtBackorder());



            /** Libérer un tableau alloué dynamiquement ******************************/

            delete[] indices1;
            delete[] indices2;

            for(int i=0;i<nb_chr;i++)
            {
                for(int j=0;j<NbS;j++)
                {
                    delete nouv_gen[i][j];
                    delete nouv_gen3[i][j];
                }
            }
            delete [] nouv_gen;
            delete [] nouv_gen3;

            for(int i=0;i<selection1_2+selection_2;i++)
            {
                for(int j=0;j<NbS;j++)
                {
                    delete nouv_gen1[i][j];
                    //delete nouv_gen2[i][j];
                }
            }
            delete [] nouv_gen1;
            delete [] nouv_gen2;

            delete[] fitness;
            delete[] fitness2;
            delete[] fitness_croismut;

            for(int i=0;i<selection_2;i++)
            {
                for(int j=0;j<NbS;j++)
                {
                    delete par_pop[i][j];
                    delete fils1[i][j];
                    delete fils2[i][j];
                    delete fils3[i][j];
                }
            }
            delete [] par_pop;
            delete [] fils1;
            delete [] fils2;
            delete [] fils3;
        }
    }
        else
        {
            cerr << "Impossible d'ouvrir le fichier ! 0" << endl;
        }
        return 0;
}
int main(int argc, char** argv){
    //mainOussama();
    mainSimon();
}