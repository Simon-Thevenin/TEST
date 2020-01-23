#pragma once
#include "Data.h"

class GRASP
{
	Data* data;

	public:
		GRASP(void);
        GRASP(Data* d);
		~GRASP(void);
		double*** translateLeadTime(int** L);
		int** generateLeadTime();
		bool CheckLeadTimeInBudget(int** L);
};

