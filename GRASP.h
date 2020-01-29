#pragma once
#include "Data.h"

class GRASP
{
	Data* data;
	int gamma;
    int** LT;
    double*** delta;

public:
		GRASP(void);
        GRASP(Data* d, int  _gamma);
		~GRASP(void);
		double solve();
		void translateLeadTime();
		void generateLeadTime();
        bool CheckLeadTimeInBudget();
        bool PushDelta();
};

