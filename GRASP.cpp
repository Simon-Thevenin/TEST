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
}


GRASP::~GRASP(void)
{
}



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
    clock_t start, end;
    start = clock();
    while(temps<=this->data->getTimeLimite())
    {

        do{
            Lt = this->generateLeadTime();
            delta = this->translateLeadTime(Lt);
        }
        while(this->CheckLeadTimeInBudget(delta));

        ModelQuantity* mod = new ModelQuantity(this->data, this->gamma);
        mod->BuildModel();
        mod->AddScenario(delta);
        double cost = mod->Solve(false, nullptr, true, 0.01);

        if(cost< bestcost)
        {
            bestcost = cost;

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

    ModelQuantity* mod = new ModelQuantity(this->data, this->gamma);
    mod->BuildModel();
    double cost = mod->Solve(true, bestY, false, 0.01);
    cout<<"Grasp cost::::"<<cost<<" temps"<<temps<<endl;
    cout<<"Inv Cost:"<<mod->GetInventoryCosts()<<" Avg Inv:"<<mod->GetAvgInventory()<<" Order Cost:"<<mod->GetOrderingCosts()<<endl;
    cout<<"Back Cost:"<<mod->GetBackorderCosts()<<" Avg Back:"<<mod->GetAvgtBackorder()<<" Pursh cost:"<<mod->GetPurshasingCosts()<<endl;
    return cost;
}

double*** GRASP::translateLeadTime(int** L)
{

    double*** delta = new double**[this->data->getNPer()+1];

        for(int tau=1; tau<=this->data->getNPer(); tau++)
        {
            delta[tau] = new double*[this->data->getNPer()+1];
            for(int t=1; t<=this->data->getNPer(); t++)
            {
                delta[tau][t] = new double[this->data->getNSup()+1];
                for(int s=1; s<=this->data->getNSup(); s++)
                {
                    if (tau <= t - L[tau][s])
                        delta[tau][t][s] = 1;
                    else
                        delta[tau][t][s] = 0;
                }
            }
        }


    return delta;
}


int** GRASP::generateLeadTime()
{
	int** LT = new int*[this->data->getNPer()+1];
	 for(int t=1; t<=data->getNPer(); t++)
	{
        LT[t] = new int[this->data->getNSup()+1];
			for(int s=1; s<=data->getNSup(); s++)
			{
				LT[t][s] =(int)(this->data->getLMin(s-1) + ((double) rand() / (RAND_MAX)) * (this->data->getLMax(s-1) - this->data->getLMin(s-1)));
		    }
	 }
    return LT;
}


bool GRASP::CheckLeadTimeInBudget(double*** delta)
{
    for(int t=1; t<=data->getNPer(); t++)
    {
        int sum = 0;
        for(int s=1; s<=data->getNSup(); s++)
        {
            if(t-this->data->getLMin(s-1)>0)
                sum = sum + delta[t-this->data->getLMin(s-1)][t][s];
        }
       if(sum >= this->data->getNSup() - this->gamma)
           return false;
    }
    return true;
}