#include "GRASP.h"
#include <stdlib.h>
#include "ModelQuantity.h"

GRASP::GRASP(void)
{
}


GRASP::GRASP(Data* d, int  _gamma)
{
	this->data = d;
	this->gamma = _gamma;
    LT = new int*[this->data->getNPer()+1];
    for(int t=1; t<=data->getNPer(); t++) {
        LT[t] = new int[this->data->getNSup() + 1];
    }
  delta = new double**[this->data->getNPer()+1];

    for(int tau=1; tau<=this->data->getNPer(); tau++) {
        delta[tau] = new double *[this->data->getNPer() + 1];
        for (int t = 1; t <= this->data->getNPer(); t++) {
            delta[tau][t] = new double[this->data->getNSup() + 1];
        }
    }
}


GRASP::~GRASP(void)
{
}


/*
double GRASP::solve()
{
    int**  Lt ;
    double*** delta ;
    double temps=0;
    double bestcost= XPRB_INFINITY;
    int** bestY=new int*[this->data->getNSup()];
    for(int s=0; s < this->data->getNSup(); s++) {
        bestY[s] = new int[this->data->getNPer() ];
    }
    ModelQuantity* mod = new ModelQuantity(this->data, this->gamma);
    mod->BuildModel();

    clock_t start, end;
    start = clock();
    int iter = 0;
    while(temps<=this->data->getTimeLimite())
    {

        iter++;
        do{
           this->generateLeadTime();
            this->translateLeadTime();
        }
        while(this->CheckLeadTimeInBudget());

        this->PushDelta();
        mod->UpdateLastScenario(this->delta);

        double cost = mod->Solve(false, nullptr, true, 0.01);

        if(cost< bestcost)
        {
            bestcost = cost;
            cout<<cost<<endl;
            for(int t=1; t <= this->data->getNPer(); t++)
            {
                for(int s=1; s<=this->data->getNSup(); s++)
                {
                    bestY[s-1][t-1] = mod->Y[t][s].getSol();
                }
            }
        }
        end = clock();
        temps = (double) (end-start)/ CLOCKS_PER_SEC;
    }

    ModelQuantity* mod2 = new ModelQuantity(this->data, this->gamma);
    mod2->BuildModel();
    double cost = mod2->Solve(true, bestY, false, 0.001);
    int** obtainedY2 = new int*[data->getNSup()];
    for(int s=0; s<data->getNSup(); s++)
    {
        obtainedY2[s] =  new int[data->getNPer()];
        for(int t=0; t<data->getNPer(); t++)
        {
            obtainedY2[s][t] =mod2->Y[t+1][s+1].getSol();
        }
    }

    string FFile = "..//resultat7.txt";
    data->Affich_Results(FFile,  gamma, "Grasp", obtainedY2, cost, temps, -1, -1, -1,mod2->GetInventoryCosts(), mod2->GetAvgInventory(), mod2->GetPurshasingCosts(), mod2->GetBackorderCosts(), mod2->GetAvgtBackorder());

    cout<<"Grasp cost::::"<<cost<<" temps"<<temps<<endl;
    cout<<"Inv Cost:"<<mod->GetInventoryCosts()<<" Avg Inv:"<<mod->GetAvgInventory()<<" Order Cost:"<<mod->GetOrderingCosts()<<endl;
    cout<<"Back Cost:"<<mod->GetBackorderCosts()<<" Avg Back:"<<mod->GetAvgtBackorder()<<" Pursh cost:"<<mod->GetPurshasingCosts()<<endl;
    return cost;
}
*/
void GRASP::translateLeadTime()
{


        for(int tau=1; tau<=this->data->getNPer(); tau++)
        {
             for(int t=1; t<=this->data->getNPer(); t++)
            {
                for(int s=1; s<=this->data->getNSup(); s++)
                {
                    if (tau <= t - this->LT[tau][s])
                        delta[tau][t][s] = 1;
                    else
                        delta[tau][t][s] = 0;
                }
            }
        }

}


void GRASP::generateLeadTime()
{
	 for(int t=1; t<=data->getNPer(); t++)
	{
      		for(int s=1; s<=data->getNSup(); s++)
			{
				LT[t][s] =this->data->getLMin(s-1); //(int)(this->data->getLMin(s-1) + ((double) rand() / (RAND_MAX)) * (this->data->getLMax(s-1) - this->data->getLMin(s-1)));
		    }
	 }
}


bool GRASP::CheckLeadTimeInBudget()
{
    for(int t=1; t<=data->getNPer(); t++)
    {
        int sum = 0;
        for(int s=1; s<=data->getNSup(); s++)
        {
            if(t-this->data->getLMin(s-1)>0)
                sum = sum + this->delta[t-this->data->getLMin(s-1)][t][s];
        }
       if(sum >= this->data->getNSup()+1 - this->gamma)
           return false;
    }
    return true;
}

bool GRASP::PushDelta()
{
    for(int k = 0; k <= 1000; k++) {
        int s = (int) (1) + ((double) rand() / (RAND_MAX)) * (this->data->getNSup() - 1);
        int t = (int) (1) + ((double) rand() / (RAND_MAX)) * (this->data->getNPer() - 1);

        LT[t][s] += 1;
        this->translateLeadTime();
        if (!CheckLeadTimeInBudget()) {
            LT[t][s] -= 1;
        }

    }

}