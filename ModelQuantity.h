#pragma once
#include "xprb_cpp.h"
#include "xprs.H"
#include "xprm_mc.h"
#include "Data.h"
#include "SubProblem.h"
#include <cmath>
using namespace ::dashoptimization;

using namespace std;


class ModelQuantity
{
public: 
	SubProblem* ModSub;
	XPRBprob* pbQ;
    int T; 
    int W;
	int S;
	double **** delta;
	//int ** givenY;
	XPRBvar** Q;
	XPRBvar** Y;
	XPRBvar C;
	XPRBctr** BigMConstraints;
	Data* D;
	double totalsetupcosts;

public:
	
	ModelQuantity( void );
	ModelQuantity( Data* d, int gamma);
	~ModelQuantity(void);
	void BuildModel(void); 
	void SetYToValue(int** Y);
	void Solve(void);
	void DisplaySol(void);
	double getCost();
	double** getQuantities();
	void AddScenario(double*** givendelta);
	double Solve(bool givenY, int** givenY2, bool fastUB);
	void SetYBinary();
};

