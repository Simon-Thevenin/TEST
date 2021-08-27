#include <iostream>
#include <time.h>
#include <fstream>
#include <cstdlib>
//#include "xprb_cpp.h"
#include "xprs.h"
#include "ModelQuantity.h"
#include "ModelRobust.h"
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

string pathfile = "../";
//string pathfile =  "C://Users//oussama.ben-ammar//CLionProjects//TEST//";

Data * getData(string file, int NbPeriod, int NbSupplier)
{
    string Thefile = pathfile+file;
    ifstream fichier( Thefile, ios::in);
    cout<<Thefile<<endl;
    Data *data = new Data(NbPeriod, NbSupplier, Thefile);
    return data;

}


void runFixAndOptRobust(string file, int NbPeriod, int NbSupplier, double gamma1, double gamma2, double gamma3) {
    Data *data = getData(file, NbPeriod, NbSupplier);
    ModelRobust* ModR= new ModelRobust(data, gamma1, gamma2, gamma3);
    ModR->FixAndOpt(false);
    /*
       for(int s=0; s<data->getNSup(); s++) {
            for (int t = 0; t < data->getNPer(); t++) {
                cout << " Y[" << t << "][" << s << "]=" << ModR->Y[t + 1][s + 1].getSol();
                cout << " Q[" << t << "][" << s << "]=" << ModR->Q[t + 1][s + 1].getSol();
                //  cout<<" I["<<t<<"]["<<s<<"]="<<mod->I[t+1][s+1].getSol();
                //  cout<<" B["<<t<<"]["<<s<<"]="<<mod->B[t+1][s+1].getSol();
            }
            cout << endl;
        }*/
    /*************************EVALUATE THE COST****************************/
    double ** associatedquantities =  ModR->getQuantities();
    SubProblem* ModSub = new SubProblem(data, gamma1, gamma2, gamma3);
    ModSub->BuildModel();
    ModSub->getWorstCaseDelta(associatedquantities);
    double cost=  ModSub->getAssociatedCost() +ModR->GetSetupCost() + ModR->GetPurshasingCosts();

    int** obtainedY2 = new int*[data->getNSup()];
    for(int s=0; s<data->getNSup(); s++)
    {
    obtainedY2[s] =  new int[data->getNPer()];
    for(int t=0; t<data->getNPer(); t++)
    {
    if(ModR->Y[t+1][s+1].getSol() > 0.5)
    obtainedY2[s][t] =  1;
    else
    obtainedY2[s][t] =  0;

    }
    }
    string FFile = pathfile + "resultat7.txt";


    data->Affich_Results(FFile, 0,  gamma1, gamma2,gamma3,"FixAndOpt_Robust_EvaluateYQ", obtainedY2, cost, ModR->pbRob->getObjVal(), ModR->durationFixAndOpt, ModR->LastLB, ModR->LastNrNode,  ModR->LastStatus, ModR->nriterationfixandopt, ModR->TimeBastSolFixAndOpt, ModSub->GetInventoryCosts(), ModSub->GetAvgInventory(), ModR->GetPurshasingCosts(), ModSub->GetBackorderCosts(), ModSub->GetAvgtBackorder(), -1);
    cout<<"robust cost::::"<<cost<<endl;
    cout<<"Inv Cost:"<<ModSub->GetInventoryCosts()<<" Avg Inv:"<<ModSub->GetAvgInventory()<<" Order Cost:"<<ModR->GetSetupCost()<<endl;
    cout<<"Back Cost:"<<ModSub->GetBackorderCosts()<<" Avg Back:"<<ModSub->GetAvgtBackorder()<<" Pursh cost:"<<ModR->GetPurshasingCosts()<<endl;


    /*int** obtainedY = new int*[data->getNSup()];
    for(int s=0; s<data->getNSup(); s++)
    {
    obtainedY[s] =  new int[data->getNPer()];
    for(int t=0; t<data->getNPer(); t++)
    {
    if(ModR->Y[t+1][s+1].getSol() > 0.5)
    obtainedY[s][t] =  1;
    else
    obtainedY[s][t] =  0;
    }
    }
    ModelQuantity* mod = new ModelQuantity(data,gamma1, gamma2, gamma3 );
    mod->BuildModel();
    cost = mod->Solve(true, obtainedY, false, 0.01);

    data->Affich_Results(FFile, 0,  gamma1, gamma2, gamma3, "Robust_FixAndOp_ResolveFixY", obtainedY, cost,  ModR->pbRob->getObjVal(), ModR->durationFixAndOpt, ModR->LastLB, ModR->LastNrNode,  ModR->LastStatus, ModR->nriterationfixandopt,  ModR->TimeBastSolFixAndOpt,mod->GetInventoryCosts(), mod->GetAvgInventory(), mod->GetPurshasingCosts(), mod->GetBackorderCosts(), mod->GetAvgtBackorder(), mod->nriteration);
    cout<<"optimal cost Adversarial Fix Y::::"<<cost<<endl;
    cout<<"Inv Cost:"<<mod->GetInventoryCosts()<<" Avg Inv:"<<mod->GetAvgInventory()<<" Order Cost:"<<mod->GetOrderingCosts()<<endl;
    cout<<"Back Cost:"<<mod->GetBackorderCosts()<<" Avg Back:"<<mod->GetAvgtBackorder()<<" Pursh cost:"<<mod->GetPurshasingCosts()<<endl;*/
}

void runFixAndOpt(string file, int NbPeriod, int NbSupplier, double gamma1, double gamma2, double gamma3) {

    Data *data = getData(file, NbPeriod, NbSupplier);
    ModelQuantity* ModQ= new ModelQuantity(data, gamma1, gamma2, gamma3);
    ModQ->BuildModel();
    string name= "FixAndOpt2";
    int** givenY2 = new int*[data->getNSup()];
    for(int s=0; s<data->getNSup(); s++)
    {
        givenY2[s] =  new int[data->getNPer()];
        for(int t=0; t<data->getNPer(); t++)
        {
            givenY2[s][t] =1;
        }
    }

    //ModQ->Solve(true, givenY2, false, 0.05);

    ModQ->FixAndOpt();

    int** obtainedY2 = new int*[data->getNSup()+1];
    for(int s=0; s<data->getNSup(); s++)
    {
        obtainedY2[s] =  new int[data->getNPer()];
        for(int t=0; t<data->getNPer(); t++)
        {
            obtainedY2[s][t] =ModQ->Y[t+1][s+1].getSol();
        }
    }
    double cost = ModQ->Solve(true, obtainedY2, false, 0.01);


    string FFile = pathfile + "resultat7.txt";
    data->Affich_Results(FFile, 0,  gamma1, gamma2,gamma3,name, obtainedY2, cost, ModQ->LastGap, ModQ->LastRunning, ModQ->LastLB, ModQ->LastNrNode,  ModQ->LastStatus,  ModQ->nriterationfixandopt, ModQ->TimeBastSolFixAndOpt, ModQ->GetInventoryCosts(), ModQ->GetAvgInventory(), ModQ->GetPurshasingCosts(), ModQ->GetBackorderCosts(), ModQ->GetAvgtBackorder(), ModQ->nriteration);

    cout<<"optimal cost::::"<<cost<<endl;
    cout<<"Inv Cost:"<<ModQ->GetInventoryCosts()<<" Avg Inv:"<<ModQ->GetAvgInventory()<<" Order Cost:"<<ModQ->GetOrderingCosts()<<endl;
    cout<<"Back Cost:"<<ModQ->GetBackorderCosts()<<" Avg Back:"<<ModQ->GetAvgtBackorder()<<" Pursh cost:"<<ModQ->GetPurshasingCosts()<<endl;
}
void runExact(string file, int NbPeriod, int NbSupplier, double gamma1, double gamma2, double gamma3, bool warmstart, int additionalsetup)
{

    Data *data = getData(file, NbPeriod, NbSupplier);
    data->setSetup(0, data->getSetup(0)+additionalsetup);
    ModelQuantity* ModQ= new ModelQuantity(data, gamma1, gamma2, gamma3);
    ModQ->LastRunning = 0.0;
    ModQ->BuildModel();
    string name= "ExactNoWarmStart";
    if(warmstart) {
           name= "Exact";
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
    }
    cout<<"solve with non fixed Y."<<endl;
    double cost = ModQ->Solve(false, nullptr, false, 0.0001);
    cout<<"solved."<<endl;
    int** obtainedY2 = new int*[data->getNSup()+1];
    for(int s=0; s<data->getNSup(); s++)
    {
        obtainedY2[s] =  new int[data->getNPer()];
        for(int t=0; t<data->getNPer(); t++)
        {
            obtainedY2[s][t] =ModQ->Y[t+1][s+1].getSol();
        }
    }
    string FFile = pathfile + "resultat7.txt";
    data->Affich_Results(FFile, additionalsetup,  gamma1, gamma2,gamma3,name, obtainedY2, cost, ModQ->LastGap, ModQ->LastRunning,  ModQ->LastLB, ModQ->LastNrNode,  ModQ->LastStatus, ModQ->LastNrIteration, -1, ModQ->GetInventoryCosts(), ModQ->GetAvgInventory(), ModQ->GetPurshasingCosts(), ModQ->GetBackorderCosts(), ModQ->GetAvgtBackorder(), ModQ->nriteration);

    cout<<"optimal cost::::"<<cost<<endl;
    cout<<"Inv Cost:"<<ModQ->GetInventoryCosts()<<" Avg Inv:"<<ModQ->GetAvgInventory()<<" Order Cost:"<<ModQ->GetOrderingCosts()<<endl;
    cout<<"Back Cost:"<<ModQ->GetBackorderCosts()<<" Avg Back:"<<ModQ->GetAvgtBackorder()<<" Pursh cost:"<<ModQ->GetPurshasingCosts()<<endl;
    ModQ->DisplaySol();
}

void runRobust(string file, int NbPeriod, int NbSupplier, double gamma1,  double gamma2, double gamma3, int additionalsetup)
{
    Data *data = getData(file, NbPeriod, NbSupplier);
    data->setSetup(0,  data->getSetup(0) + additionalsetup);
    ModelRobust* ModR= new ModelRobust(data, gamma1, gamma2, gamma3);
    ModR->Solve();
/*
   for(int s=0; s<data->getNSup(); s++) {
        for (int t = 0; t < data->getNPer(); t++) {
            cout << " Y[" << t << "][" << s << "]=" << ModR->Y[t + 1][s + 1].getSol();
            cout << " Q[" << t << "][" << s << "]=" << ModR->Q[t + 1][s + 1].getSol();
            //  cout<<" I["<<t<<"]["<<s<<"]="<<mod->I[t+1][s+1].getSol();
            //  cout<<" B["<<t<<"]["<<s<<"]="<<mod->B[t+1][s+1].getSol();
        }
        cout << endl;
    }*/

    /*************************EVALUATE THE COST****************************/
    double ** associatedquantities =  ModR->getQuantities();
    cout<<"Qty:";
    for(int s=1; s<=data->getNSup(); s++)
    {
        for(int t=1; t<=data->getNPer(); t++)
        { cout << associatedquantities[t][s]<<" ";}}
    SubProblem* ModSub = new SubProblem(data, gamma1, gamma2, gamma3);
    ModSub->BuildModel();
    ModSub->getWorstCaseDelta(associatedquantities);
    double cost=  ModSub->getAssociatedCost() +ModR->GetSetupCost() + ModR->GetPurshasingCosts();

    int** obtainedY2 = new int*[data->getNSup()];
    for(int s=0; s<data->getNSup(); s++)
    {
        obtainedY2[s] =  new int[data->getNPer()];
        for(int t=0; t<data->getNPer(); t++)
        {
            if(ModR->Y[t+1][s+1].getSol() > 0.5)
                obtainedY2[s][t] =  1;
            else
                obtainedY2[s][t] =  0;

        }
    }
    string FFile = pathfile + "resultat7.txt";


    data->Affich_Results(FFile, additionalsetup,   gamma1, gamma2,gamma3,"Robust_EvaluateYQ", obtainedY2, cost, ModR->pbRob->getObjVal(), ModR->LastRunning, ModR->LastLB, ModR->LastNrNode,  ModR->LastStatus, -1, -1, ModSub->GetInventoryCosts(), ModSub->GetAvgInventory(), ModR->GetPurshasingCosts(), ModSub->GetBackorderCosts(), ModSub->GetAvgtBackorder(), -1);
    cout<<"robust cost::::"<<cost<<endl;
    cout<<"Inv Cost:"<<ModSub->GetInventoryCosts()<<" Avg Inv:"<<ModSub->GetAvgInventory()<<" Order Cost:"<<ModR->GetSetupCost()<<endl;
    cout<<"Back Cost:"<<ModSub->GetBackorderCosts()<<" Avg Back:"<<ModSub->GetAvgtBackorder()<<" Pursh cost:"<<ModR->GetPurshasingCosts()<<endl;


    int** obtainedY = new int*[data->getNSup()];
    for(int s=0; s<data->getNSup(); s++)
    {
        obtainedY[s] =  new int[data->getNPer()];
        for(int t=0; t<data->getNPer(); t++)
        {
            if(ModR->Y[t+1][s+1].getSol() > 0.5)
                obtainedY[s][t] =  1;
            else
                obtainedY[s][t] =  0;
        }
    }
    ModelQuantity* mod = new ModelQuantity(data,gamma1, gamma2, gamma3 );
    mod->BuildModel();
    cost = mod->Solve(true, obtainedY, false, 0.01);

    data->Affich_Results(FFile, additionalsetup,  gamma1, gamma2, gamma3, "Robust_ResolveFixY", obtainedY, cost,  ModR->pbRob->getObjVal(), ModR->LastRunning, ModR->LastLB, ModR->LastNrNode,  ModR->LastStatus, -1, -1,mod->GetInventoryCosts(), mod->GetAvgInventory(), mod->GetPurshasingCosts(), mod->GetBackorderCosts(), mod->GetAvgtBackorder(), mod->nriteration);
    cout<<"optimal cost Adversarial Fix Y::::"<<cost<<endl;
    cout<<"Inv Cost:"<<mod->GetInventoryCosts()<<" Avg Inv:"<<mod->GetAvgInventory()<<" Order Cost:"<<mod->GetOrderingCosts()<<endl;
    cout<<"Back Cost:"<<mod->GetBackorderCosts()<<" Avg Back:"<<mod->GetAvgtBackorder()<<" Pursh cost:"<<mod->GetPurshasingCosts()<<endl;

}

/*void runGrasp(string file, int NbPeriod, int NbSupplier, double gamma, string TypeOfBudget)
{
    string Thefile = pathfile+file;
    ifstream fichier( Thefile, ios::in);
    Data *data = new Data(NbPeriod, NbSupplier, Thefile, TypeOfBudget);
    GRASP* g = new GRASP(data, gamma);
    g->solve();
}*/


void runDeterministic(string file, int NbPeriod, int NbSupplier, int gamma1, int gamma2, int gamma3, string Type, int addidtionalsetup)
{
    string Thefile = pathfile+file;
    ifstream fichier( Thefile, ios::in);
    Data *data = new Data(NbPeriod, NbSupplier, Thefile);
    data->setSetup(0,  data->getSetup(0) + addidtionalsetup);
    int* Lmean = new int[data->getNSup()+1];

    for(int s= 1; s<=data->getNSup(); s++)
    {
        if(Type== "Min")
            Lmean[s] = data->getLMin(s-1); //(int)((data->getLMax(s-1) + data->getLMin(s-1))/2);
        if(Type== "Mean")
            Lmean[s] =(int)((data->getLMax(s-1) + data->getLMin(s-1))/2);
        if(Type== "Max")
            Lmean[s] = data->getLMax(s-1); //(int)((data->getLMax(s-1) + data->getLMin(s-1))/2);
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

                //cout<<Lmean[s]<<endl;
               // cout<< delta[tau][t][s];
            }
        }
    }

    ModelQuantity* mod = new ModelQuantity(data,0, 0, 0 );
    mod->BuildModel();
    mod->AddScenario(delta);
    XPRSprob opt_prob =  mod->pbQ->getXPRSprob();
    XPRSsetintcontrol(opt_prob,XPRS_MAXTIME, data->getTimeLimite());
    mod->Solve(false, nullptr, true, 0.01);


    /*************************EVALUATE THE COST****************************/
    double ** associatedquantities =  mod->getQuantities();

    SubProblem* ModSub = new SubProblem(data, gamma1, gamma2, gamma3);

    ModSub->BuildModel();



    ModSub->getWorstCaseDelta(associatedquantities);
    double cost=  ModSub->getAssociatedCost() +mod->totalsetupcosts + mod->GetPurshasingCosts();

    int** obtainedY2 = new int*[data->getNSup()];
    for(int s=0; s<data->getNSup(); s++)
    {
        obtainedY2[s] =  new int[data->getNPer()];
        for(int t=0; t<data->getNPer(); t++)
        {
            obtainedY2[s][t] =mod->Y[t+1][s+1].getSol();
        //    cout<<" Y["<<t<<"]["<<s<<"]="<<mod->Y[t+1][s+1].getSol();
         //   cout<<" Q["<<t<<"]["<<s<<"]="<<mod->Q[t+1][s+1].getSol();
          //  cout<<" I["<<t<<"]["<<s<<"]="<<mod->I[t+1][s+1].getSol();
          //  cout<<" B["<<t<<"]["<<s<<"]="<<mod->B[t+1][s+1].getSol();
        }
       // cout<<endl;
    }
    string FFile = pathfile + "resultat7.txt";
    data->Affich_Results(FFile, 0,  gamma1, gamma2,gamma3,"determinist"+Type, obtainedY2, cost, mod->pbQ->getObjVal(), mod->LastRunning, mod->LastLB, mod->LastNrNode,  mod->LastStatus, -1, -1, ModSub->GetInventoryCosts(), ModSub->GetAvgInventory(), mod->GetPurshasingCosts(), ModSub->GetBackorderCosts(), ModSub->GetAvgtBackorder(), -1);

    cout<<"Deterministic"<<Type<<" cost::::"<<cost<<endl;
    cout<<"Inv Cost:"<<ModSub->GetInventoryCosts()<<" Avg Inv:"<<ModSub->GetAvgInventory()<<" Order Cost:"<<mod->GetOrderingCosts()<<endl;
    cout<<"Back Cost:"<<ModSub->GetBackorderCosts()<<" Avg Back:"<<ModSub->GetAvgtBackorder()<<" Pursh cost:"<<mod->GetPurshasingCosts()<<endl;

}

int mainSimon(string file, int nbp, int nbs, int gamma1, int gamma2, int gamma3) {
   runFixAndOpt(file, nbp, nbs, gamma1, gamma2, gamma3  );
   runFixAndOptRobust(file, nbp, nbs, gamma1, gamma2, gamma3  );
//  for(int a =0; a <=15; a++)
//{
//        int a=1;
//      int additionalSetup = 10*a;
 //     runRobust(file, nbp, nbs, gamma1, gamma2, gamma3, additionalSetup  );
//      runExact(file, nbp, nbs, gamma1, gamma2, gamma3, true, additionalSetup);
 //     runDeterministic(file, nbp, nbs, gamma1, gamma2, gamma3, "Min", additionalSetup);

// }
    //
    cout << nbp <<endl;
    runDeterministic(file, nbp, nbs, gamma1, gamma2, gamma3, "Min", 0);
    runDeterministic(file, nbp, nbs, gamma1, gamma2, gamma3, "Max", 0);
    runDeterministic(file, nbp, nbs, gamma1, gamma2, gamma3, "Mean", 0);
    runRobust(file, nbp, nbs, gamma1, gamma2, gamma3, 0  );
    cout<<"ExanctNW"<<endl;
    runExact(file, nbp, nbs, gamma1, gamma2, gamma3, false, 0);
    cout<<"Exanct"<<endl;
    runExact(file, nbp, nbs, gamma1, gamma2, gamma3, true, 0);
   //runGrasp(file, nbp, nbs,gamma);


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

int mainOussama(string file, int nbp, int nbs, int gamma1_,  int gamma2_, int gamma3_)  {
    int gamma1 = gamma1_;
    int gamma2 = gamma2_;
    int gamma3 = gamma3_;
    int NbPeriod = nbp;
    int NbSupplier =nbs;
   // string file = pathfile + "1.txt";
  //  ifstream fichier(file, ios::in);  // on ouvre le fichier en lecture
    //ifstream fichier(argv[1], ios::in);  // on ouvre le fichier en lecture

    if(true) {
// Files

        // Files
     //   string dataFile =file;
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
        Data *data = getData(file, NbPeriod, NbSupplier);
      //  Data *data = new Data(NbPeriod, NbSupplier, dataFile);

        // Appeler la classe Algo Gen
        FGenetic ag(data,  gamma1,   gamma2,   gamma3);

        // Ouverture fichier resultat pour écriture
        ofstream fichierS(pathfile+"resultat8.txt", ios::out | ios::app);
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
            ag.ModR->SetYToValue(nouv_gen[0]);
            ag.ModR->Solve();

            double cost = ag.ModR->pbRob->getObjVal();;// ag.ModQ->Solve(true, nouv_gen[0], false, 0.01, false);
           // double cost = ag.ModQ->Solve(false, nullptr, false, 0.001);

            int** obtainedY = new int*[data->getNSup()];
            for(int s=0; s<data->getNSup(); s++)
            {
                obtainedY[s] =  new int[data->getNPer()];
                for(int t=0; t<data->getNPer(); t++)
                {
                    if(ag.ModR->Y[t+1][s+1].getSol() > 0.5)
                        obtainedY[s][t] =  1;
                    else
                        obtainedY[s][t] =  0;
                }
            }
            ModelQuantity* mod = new ModelQuantity(data,gamma1, gamma2, gamma3 );
            mod->BuildModel();
            cost = mod->Solve(true, obtainedY, false, 0.01);

            data->Affich_Results(FFile, 0, gamma1, gamma2, gamma3, "GA", nouv_gen[0], cost, fitness[0],  temps, -1, -1, -1,  MeillsolPopIni, Iteration+1,mod->GetInventoryCosts(), mod->GetAvgInventory(), mod->GetPurshasingCosts(), mod->GetBackorderCosts(), mod->GetAvgtBackorder(), Iteration+1);



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


//mainSimon(string(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

    //cout<<"REMOVE THE +11 !!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    mainSimon(string(argv[1]), atoi(argv[2])+11, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]),atoi(argv[6]));
    cout<<"GA"<<endl;
    mainOussama(string(argv[1]), atoi(argv[2])+11, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

}