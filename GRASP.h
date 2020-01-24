#pragma once
#include "Data.h"

class GRASP
{
	Data* data;
	int gamma;

	public:
		GRASP(void);
        GRASP(Data* d, int  _gamma);
		~GRASP(void);
		double solve();
		double*** translateLeadTime(int** L);
		int** generateLeadTime();
        bool CheckLeadTimeInBudget(double*** delta);
};

