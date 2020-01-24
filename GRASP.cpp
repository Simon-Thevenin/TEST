#include "GRASP.h"
#include <stdlib.h>

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


double*** GRASP::translateLeadTime(int** L)
{

return nullptr;
}


int** GRASP::generateLeadTime()
{
	int** LT = new int*[this->data->getNPer()];
	 for(int t=1; t<data->getNPer()+1; t++)
	{
		
			for(int s=1; s<=data->getNSup(); s++)
			{
				LT[t][s] = rand();
		    }
	 }
    return nullptr;
}


bool GRASP::CheckLeadTimeInBudget(int** L)
{

   return false;
}