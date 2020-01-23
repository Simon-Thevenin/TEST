/*
 * Solution.cpp
 *
 */

#include "Solution.h"

Solution::Solution(int*** _x, int _a, Data *data){
	data_ = data;



}

Solution::~Solution(){

}


double Solution::solve(int*** _x, int _a){

	// Cout donn� par le mod�le
	double XpressCost=0;

	// To solve the problem
	//ModQ->Solve(true, givenY2, false);

    // Un exemple simple -> une fonction fitness qui calcule la somme des valeurs du tab
    for(int i=0;i<data_->getNSup();i++)
        for(int j=0;j<data_->getNPer();j++)
            XpressCost += _x[_a][i][j];
	//cout <<"robust cost: "<<c<<endl;

    // � int�grer code Xpress
	return XpressCost;
}

void Solution::lire1(const char* nom_fichier, double **tab, int n,int u){

	FILE *fichier = fopen(nom_fichier, "r");

	for (int i=0; i<n; i++)
	{
		for (int j=0; j<u; j++)
		{
			fscanf(fichier, "%lf ", tab[i]+j);
		}

	}
	fclose(fichier);


}
void Solution::afficher1(string c , int** tab, int a, int b){
	cout<<c<<":"<<endl;
	for (int i=0; i<a; i++)
	{
		for (int j=0; j<b; j++)
		{
			cout<<tab[i][j]<<" ";
		}
		cout<<endl;
	}cout<<endl;
}
void Solution::afficher(string c , int* tab, int a, int b){
	cout<<c<<":     ";
	for( int  i=a;i<b;i++)
	{
		cout<<tab[i]<<" ";
	}
	cout<<endl;cout<<endl;
}
void Solution::afficher(string c , double* tab, int a, int b){
	cout<<c<<":     ";
	for( int  i=a;i<b;i++)
	{
		cout<<tab[i]<<" ";
	}
	cout<<endl;cout<<endl;
}
