#include "Data.h"
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
   cout<<file<<endl;
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
void Data::print(string s)
{
	if(true)
		cout<<s<<endl; 
}
void Data::print(string s, double value)
{
	if(true)
		cout<<s<<value<<endl; 
}