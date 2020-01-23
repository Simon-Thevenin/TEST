#include "Data.h"

Data::Data(string dataFile)
{
	dataFile_ = dataFile;
	parseData(dataFile_);

}

Data::~Data()
{
	// TODO Auto-generated destructor stub
}

void Data::parseData(string file){
	ifstream fichier(file.c_str());

	if(fichier)
	{
		fichier >> NSup >> NPer >> cb >> ch;
	}
	else
	{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}




int Data::getNPer(){
	return 5;
}

double Data::getch(){
	return 1.0;
}
double Data::getcb(){
	return 10.0;
}
void Data::setch(double value){
    ch = value;
}
void Data::setcb(double value){
    cb = value;
}
int Data::getNSup(){
	return 2;
}
double Data::getDemand(int t){
	return 5;
}

double Data::getSetup(int s){
	return 10.0;
}

double Data::getPrice(int s){
	return 1.0;
}


int Data::getLMin(int s){
	return 0;
}

int Data::getLMax(int s){
	return 0;
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