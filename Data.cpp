#include "Data.h"
#include <math.h>
Data::Data(int nbPeriods, int nbSuppliers, string dataFile)
{
    dataFile_ = dataFile;

    NPer = nbPeriods;
    NSup = nbSuppliers;

    parseData(dataFile_);

}

Data::~Data()
{
    // TODO Auto-generated destructor stub
}

void Data::parseData(string file){

   FILE *fichier = fopen(file.c_str(), "r");
   //cout<<file<<endl;
    if(fichier)
    {
        /** Si on ouvre le fichier correctement, on déclare et on importe**/
        // Setup (odering) costs
        cs= new double[NSup];

        // Unit price
        up= new double[NSup];

        //Demand
        d= new int[NPer];

        // lead-times limits
        LMax= new int[NSup];
        LMin= new int[NSup];

        // Importer Setup costs
        for(int i = 0; i < NSup; i++)
        {
            fscanf(fichier, "%lf ", &cs[i]);
        }
        // Importer Unit prices
        for(int i = 0; i < NSup; i++)
        {
            fscanf(fichier, "%lf ", &up[i]);
        }
        // Importer cb ch et Imax
        fscanf (fichier, "%lf", &ch);
        fscanf (fichier, "%lf", &cb);
        fscanf (fichier, "%d", &Imax);

        // Importer Demand
        for(int i = 0; i < NPer; i++)
        {
            fscanf(fichier, "%d ", &d[i]);
        }
        // Importer Demand
        for(int i = 0; i < NSup; i++)
        {
            fscanf(fichier, "%d ", &LMax[i]);
        }
        // Importer Demand
        for(int i = 0; i < NSup; i++)
        {
            fscanf(fichier, "%d ", &LMin[i]);
        }

        // Calculer le TBO
        double MeanD=0;
        for(int i = 0; i < NPer; i++)
        {
            MeanD+=d[i];
        }
        MeanD=MeanD/(NPer-11);
        bestmaxTBO=0;
        bestminTBO=50;
        for(int i=0 ; i<NSup; i++)
        {
           // cout <<"TBO("<<i<<"): "<<(int)(sqrt(2*cs[i]/(MeanD*ch))) <<" vs. "<<(int)(sqrt(2*cs[i]/(MeanD*ch))*sqrt((ch+cb)/cb))<<endl;
            if (bestminTBO>(int)(sqrt(2*cs[i]/(MeanD*ch))))
                bestminTBO=(int)(sqrt(2*cs[i]/(MeanD*ch)));
            if (bestmaxTBO<(int)(sqrt(2*cs[i]/(MeanD*ch))*sqrt((ch+cb)/cb)))
                bestmaxTBO=(int)(sqrt(2*cs[i]/(MeanD*ch))*sqrt((ch+cb)/cb));
        }
        //cout << " TBO entre :"<<bestminTBO<<"  et  "<<bestmaxTBO<<endl;


        fclose(fichier);

    }
    else
    {
        cerr << "Impossible d'ouvrir le fichier !!?" << endl;
    }
}


void Data::ecriture(string nom_fichier,  string c , double* tab,int n){

    ofstream f(nom_fichier.c_str(), ios::out | ios::app);
    if (!f.is_open())
        cout << "Impossible d'ouvrir le fichier en écriture !" << endl;
    else
    {
        f << c;
        for (int i=0; i<n; i++)
        {
            f<<tab[i]<<" ";
        }
        f<<endl;
    }

    f.close();
}
void Data::ecriture(string nom_fichier,  string c , int* tab,int n){

    ofstream f(nom_fichier.c_str(), ios::out | ios::app);
    if (!f.is_open())
        cout << "Impossible d'ouvrir le fichier en écriture !" << endl;
    else
    {
        f << c;
        for (int i=0; i<n; i++)
        {
            f<<tab[i]<<" ";
        }
        f<<endl;
    }

    f.close();
}
void Data::Affich_Results(string nom_fichier,  int** X, double RC, double t, int g, double BIIP, int IterBestSol){

    ofstream f(nom_fichier.c_str(), ios::out | ios::app);
    if (!f.is_open())
        cout << "Impossible d'ouvrir le fichier en écriture !" << endl;
    else
    {
        f <<"Method  Sol status LB CPU Gap Nodes #Setups ";
        f<<"Avge#S/Per Tot#S Used Max#S/Per AvgInventory InvCost Ordering  PurchCost DistSuppl ReliabilitySuppl";
        f<<" minTBO maxTBO nbrGenerations BestSolInit_Pop GeneraBestSolution"<<endl;

        double nrsetup=0;
        for(int j=0;j<NSup;j++)
        {
            for(int k=0;k< NPer ; k++)
            {
                if (X[j][k]>0)
                    nrsetup+=1;
            }
        }

        double periodwithsetup = 0;
        double maxSupPerPeriod = 0;
        double avgleadtimeousedsuppliers = 0;
        double avgrangeusedsuppliers =  0;
        bool isperiodwithsetup;
        double nrsupplierinperiod;
        double Avgsetup;

        for(int k=0;k< NPer ; k++)
            {
                isperiodwithsetup = false;
                nrsupplierinperiod = 0;
                for(int j=0;j<NSup;j++)
                {
                    if (X[j][k]>0)
                    {
                        isperiodwithsetup = true;
                        nrsupplierinperiod += 1;
                        avgleadtimeousedsuppliers +=  (LMax[j]  + LMin[j] ) /2;
                        avgrangeusedsuppliers += LMax[j]  - LMin[j];
                    }
                }
                if(isperiodwithsetup)
                    periodwithsetup+=1;
                if(nrsupplierinperiod > maxSupPerPeriod)
                    maxSupPerPeriod = nrsupplierinperiod;
            }
        Avgsetup = nrsetup/periodwithsetup;
        avgrangeusedsuppliers = avgrangeusedsuppliers / nrsetup;
        avgleadtimeousedsuppliers = avgleadtimeousedsuppliers / nrsetup;

        int nrsuppliers = 0;
        bool issupplierused;
        for(int j=0;j<NSup;j++)
        {
            issupplierused = false;
            for(int k=0;k< NPer ; k++)
            {
                if (X[j][k] > 0)
                    issupplierused = true;
            }
                if (issupplierused)
                    nrsuppliers += 1;
        }

        /*** à finir ********************************************************/
        double inventoryTot=0;
        double Avginventory=0;
        double holding=0;
        double ordering =0;
        double purchasing =0;

        /**inventoryTot = sum(t in T)(sum(tau in 1..t, s in S)getsol(Q(tau,s))-sum(tau in 1..t)d(tau))
        Avginventory := inventoryTot / Periods
        holding := sum(t in T)getsol(c(t))
        ordering := sum(t in T, s in S)o(s)*getsol(Y(t,s))
        purchasing := sum(t in T, s in S)p(s)*getsol(Q(t,s))
        /*********************************************************************/
        f<<"'GA "<< RC<<" "<< "?" <<  " "<< "?" << " "<< t << " "<< "?"<< " "<<  "?"<< " "<< nrsetup<< " "<< Avgsetup<<  " "<< nrsuppliers<<  " "<< maxSupPerPeriod<< " "<< Avginventory<< " "<< holding<< " "<< ordering<< " "<< purchasing<< " "<< avgleadtimeousedsuppliers<< " "<< avgrangeusedsuppliers<<" ";
        f<< bestminTBO <<" "<<bestmaxTBO<<" "<<g<<" "<<BIIP<<" "<<IterBestSol<<endl;
    }

    f.close();

}

int Data::getNPer(){
    return NPer;
}
int Data::getNSup(){
    return NSup;
}
int Data::getDemand(int i){
    return d[i];
}
int* Data::getTabD(){
    return d;
}
double Data::getch(){
    return ch;
}

double Data::getcb(){
    return cb;
}
double Data::getSetup(int i){
    return cs[i];
}
double* Data::getcs(){
    return cs;
}
double Data::getPrice(int i){
    return up[i];
}
int Data::getLMax(int i){
    return LMax[i];
}
int Data::getLMin(int i){
    return LMin[i];
}
int* Data::getTabLMax(){
    return LMax;
}
int* Data::getTabLMin(){
    return LMin;
}

int Data::getMinTBO(){
    return bestminTBO;
}
int Data::getMaxTBO(){
    return bestmaxTBO;
}
void Data::print(string s)
{
	if(false)
		cout<<s<<endl; 
}
void Data::print(string s, double value)
{
	if(false)
		cout<<s<<value<<endl; 
}