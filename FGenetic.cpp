#include "FGenetic.h"
#include <ctime>
#define Affich_Mut 0
using namespace std;


FGenetic::FGenetic(Data *data, int gamma)
{
    //ctor
    this->ModQ= new ModelQuantity(data, gamma);
    this->ModQ->BuildModel();
}

FGenetic::~FGenetic()
{
    //dtor
}


// Fonction fitness
double FGenetic::RCOST(int*** X, int a, Data *data)
{

    double tempE = 100000;

    /** Xpress ***************/
    tempE=this->ModQ->Solve(true, X[a], false, 0.01);

    //si fastUB = true : il va donner une évaluation approchée avec des quantités approchées
    // stopgap : tant que le gap est plus grand que 0.01 on continue de tourner
    /*************************/

    return tempE;

    //return c;
}

void FGenetic::PopInit(int nb_c, int*** g, double* fit, Data *data){

    //ofstream fichierS("resultat7.txt", ios::out | ios::app);

    // On genere nb_c chromosomes (individus)
	/*for(int i=0;i<nb_c;i++)
        {
            for(int j=0;j<data->getNSup();j++)
            {
                for(int k=0;k< data->getNPer() ; k++)
                    {
                        g[i][j][k]=val_rand(0,1);
                        //fichierS<<g[i][j][k]<<" ";
                    }
                //fichierS<<endl;
            }
            // Je calcule la fitnesse
            fit[i]=RCOST(g, i, data);
            //fichierS<<" fitness: "<<fit[i]<<endl;
        }*/

	// On Fixe le nombre de commande
	// On génére aléatoirement où les placer !!!!
	int NbrOrders;
	NbrOrders=val_rand(data->getMinTBO(),data->getMaxTBO());
    // On genere nb_c chromosomes (individus)
    for(int i=0;i<nb_c;i++)
    {
        for(int j=0;j<data->getNSup();j++)
        {
            for(int k=0;k< data->getNPer() ; k++)
            {
                g[i][j][k]=0;
                //fichierS<<g[i][j][k]<<" ";
            }
            //fichierS<<endl;
        }
        // On genere aléatoirement on veut mettre cette commande
        for(int j=0;j<NbrOrders;j++)
        {
            g[i][val_rand(0,data->getNSup()-1)][val_rand(0,data->getNPer()-1)]=1;
        }


        fit[i]=RCOST(g, i, data);
        //fichierS<<" fitness: "<<fit[i]<<endl;
    }


}
void FGenetic::Cross(int *_nbcc0, int _pc, int*** f1, int s_2, int nb_c, int ***par_p, double* f_cross, Data *data){

    int *valeur;
    valeur=new int[s_2];
    valeur[0] = val_rand(0,s_2-1);

    int p, p5;
    int k1;
    int _nbcc;

    //ofstream fichierS("resultat7.txt", ios::out | ios::app);

	for ( int i=1;i<s_2;i++)
		{
            valeur[i] = val_rand(0,s_2-1);
			start2:
			for (int j=0;j<i;j++)
			{
				if(	valeur[i]==valeur[j])
				{
				    cout<<"yo"<<endl;
					valeur[i] = val_rand(0,s_2-1);
					goto start2;
				}
			}
		}
    cout<<"da"<<endl;
    // on s�lectionne N/4 couple d'une mani�re al�atoire
    _nbcc=0;
    for(int i=0 ; i<nb_c/4; i++)
        {
            p=val_rand(0,100);
            if (p<_pc)
                {
                    // choisir si on fait croisement vertical ou horizontal
                    p5=val_rand(0,100);
                    if (p5<=50)
                    {
                        // k1 c'est le point de croisement vertical
                        k1=val_rand(1,data->getNPer()-1);
                        //fichierS <<"croisement jsk la colonne verticale: "<<k1<<endl;
                        for( int j=0;j<k1;j++)
                        {
                            for( int k=0;k < data->getNSup(); k++)
                            {
                                f1[2*_nbcc][k][j] = par_p[valeur[2*_nbcc]][k][j];
                                f1[(2*_nbcc)+1][k][j] = par_p[valeur[(2*_nbcc)+1]][k][j];
                            }
                        }
                        for(int j=k1;j<data->getNPer();j++)
                        {
                            for( int k=0;k < data->getNSup(); k++)
                            {
                                f1[2*_nbcc][k][j] = par_p[valeur[(2*_nbcc)+1]][k][j];
                                f1[(2*_nbcc)+1][k][j] = par_p[valeur[2*_nbcc]][k][j];
                            }
                        }

                        f_cross[2*_nbcc]=RCOST(f1, 2*_nbcc, data);
                        f_cross[(2*_nbcc)+1]=RCOST(f1, (2*_nbcc)+1, data);
                        _nbcc++;
                    }
                    else
                    {
                        // k1 c'est le point de croisement horizontal
                        k1=val_rand(1,data->getNSup()-1);
                        //fichierS <<"croisement jsk la colonne horizontale : "<<k1<<endl;
                        for( int j=0;j<k1;j++)
                        {
                            for( int k=0;k < data->getNPer(); k++)
                            {
                                f1[2*_nbcc][j][k] = par_p[valeur[2*_nbcc]][j][k];
                                f1[(2*_nbcc)+1][j][k] = par_p[valeur[(2*_nbcc)+1]][j][k];
                            }
                        }
                        for(int j=k1;j<data->getNSup();j++)
                        {
                            for( int k=0;k < data->getNPer(); k++)
                            {
                                f1[2*_nbcc][j][k] = par_p[valeur[(2*_nbcc)+1]][j][k];
                                f1[(2*_nbcc)+1][j][k] = par_p[valeur[2*_nbcc]][j][k];
                            }
                        }
                        f_cross[2*_nbcc]=RCOST(f1, 2*_nbcc, data);
                        f_cross[(2*_nbcc)+1]=RCOST(f1, (2*_nbcc)+1, data);
                        _nbcc++;
                    }
                }
            }
    cout<<"bou"<<endl;
    *_nbcc0=_nbcc;
    delete[] valeur;
}

void FGenetic::Mutation(int *_nbcm0, int _pm, int*** f2, int s_2, int nb_c, int*** g, int* i1, Data *data){

        int p;
        int _nbcm;
        int muta;
        int r1, r2, p2, rl, rc, randl, randc, ligne, colonne;

        //ofstream fichierS("resultat7.txt", ios::out | ios::app);
        int** tabM;
        tabM = new int*[data->getNSup()];
        for( int i=0 ; i<data->getNSup(); i++)
        {
            tabM[i] = new int [data->getNPer()];
        }

        // Initialiser compteur de nombr'indiv mutés
        _nbcm=0;
        for( int i=0 ; i<s_2; i++)
        {
            muta = 0;
            for(int j=0;j<data->getNSup();j++)
            {
                for(int k=0;k< data->getNPer() ; k++)
                {
                    tabM[j][k]=0;
                }
            }
            for(int j=0;j<data->getNSup();j++)
            {
                for(int k=0;k< data->getNPer() ; k++)
                {
                    p=val_rand(0,100); // la probabilit� de mutation
                    if (p<=_pm)
                        {
                            if (g[i1[i]][j][k]==0)
                                {
                                    tabM[j][k]=1;
                                    //if (Affich_Mut==1)fichierS<<"on mute la 1: "<<j<<","<<k<<" : "<<tabM[j][k]<<endl;
                                }
                            if (g[i1[i]][j][k]==1)
                            {
                                //colonne et ligne on va les bouger
                                ligne=0;
                                colonne =0;
                                p2=val_rand(0,100);
                                // on l'affecte � un autre fournisseur � une utre p�riode
                                if (p2>95)
                                {
                                    rl = val_rand(0,data->getNSup()-1);
                                    rc = val_rand(0,data->getNPer()-1);
                                    tabM[rl][rc]=1;
                                    tabM[j][k]=0;
                                    //if (Affich_Mut==1)fichierS<<"on mute la 2 (transfere a ici): "<<rl<<","<<rc<<" : "<<tabM[rl][rc]<<endl;
                                    //if (Affich_Mut==1)fichierS<<"on mute la 3 (a partir de la): "<<j<<","<<k<<" : "<<tabM[j][k]<<endl;
                                }
                                else if (p<90)
                                {
                                    tabM[j][k]=0;
                                    //if (Affich_Mut==1)fichierS<<"on mute la 4: "<<j<<","<<k<<" : "<<tabM[j][k]<<endl;
                                }
                                else
                                {
                                    // Ligne -1 0 ou 1
                                    rl=val_rand(1,3);
                                    // si ligne 0
                                    if (rl==3)randl=0;
                                    // si ligne +1 et on n'est pas a la derniere ligne
                                    if ((rl==2) && (j<data->getNSup()-1))
                                    {
                                        randl=1;
                                    }
                                    // si ligne +1 et on est a la derniere ligne
                                    if ((rl==2) && (j==data->getNSup()-1))
                                    {
                                        // on reste sur la meme ligne
                                        randl=j;
                                        // On reste sur la meme ligne 1 sinon 0
                                        ligne=1;
                                    }
                                    // si ligne -1 et on n'est pas a la premiere ligne
                                    if ((rl==2) && (j>0))
                                    {
                                        randl=-1;
                                    }
                                    // si ligne -1 et on est a la premiere ligne
                                    if ((rl==2) && (j==0))
                                    {
                                        // on reste sur la meme ligne
                                        randl=j;
                                        // On reste sur la meme ligne 1 sinon 0
                                        ligne=1;
                                    }

                                    // Colonne -1 0 1
                                    rc=val_rand(1,3);
                                    // si colonne 0
                                    if (rc==3)randc=0;
                                    // si colonne +1 et on n'est pas a la derniere colonne
                                    if ((rc==2) && (k<data->getNPer()-1))
                                    {
                                        randc=1;
                                    }
                                    // si colonne +1 et on est a la derniere colonne
                                    if ((rc==2) && (k==data->getNPer()-1))
                                    {
                                        // on reste sur la meme colonne
                                        randc=k;
                                        // On reste sur la meme colonne 1 sinon 0
                                        colonne=1;
                                    }
                                    // si colonne -1 et on n'est pas a la premiere colonne
                                    if ((rc==2) && (k>0))
                                    {
                                        randc=-1;
                                    }
                                    // si ligne -1 et on est a la premiere colonne
                                    if ((rc==2) && (k==0))
                                    {
                                        // on reste sur la meme colonne
                                        randc=k;
                                        // On reste sur la meme colonne 1 sinon 0
                                        colonne=1;
                                    }
                                    if ((colonne==1)&&(ligne==1))
                                    {
                                        tabM[j][k]==0;
                                    }
                                    tabM[randl][randc]=1;
                                    //if (Affich_Mut==1)fichierS<<"on mute la 5: "<<randl<<","<<randc<<" : "<<tabM[randl][randc]<<endl;
                                }
                            }
                            muta=1;
                        }
                        else
                        {
                            tabM[j][k]=g[i1[i]][j][k];
                        }
                }
            }
            // pour compter le nombre d'individus mut�s
            if (muta==1)
            {
                //if (Affich_Mut==1)fichierS<<"oui on mute:"<<muta<<" "<<_nbcm<<endl;
                for(int j=0;j<data->getNSup();j++)
                {
                    for(int k=0;k< data->getNPer() ; k++)
                    {
                        f2[_nbcm][j][k]=tabM[j][k];
                    }
                }
                _nbcm++;
            }
        }

        *_nbcm0=_nbcm;

        /** vider la memoire *****/
        for( int i=0 ; i<data->getNSup(); i++)
        {
            delete [] tabM[i];
        }
        delete [] tabM;
        /*************************/
}
int FGenetic::val_rand(int a,int b){

int c;

		c = a +(int) ((b-a + 1)*rand()/(RAND_MAX+1.0));

	return c;
}
double FGenetic::my_min1double (double* T, int n){

    int i;
    double inf = T[0];
    for(i=0; i<n; i++){ if (inf>T[i]) inf = T[i];}
    return inf;
}

