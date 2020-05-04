#pragma once
#include <cstddef>
#include "xprb_cpp.h"
#include "xprs.h"
#include "xprm_mc.h"
#include "Data.h"

using namespace ::dashoptimization;
using namespace std;

class SubProblem
{
public: 
	Data* data;
	int Gamma1;
	int Gamma2;
	int Gamma3;
	XPRBprob* pbSub;
    XPRBvar*** delta;
	XPRBctr* InventoryConstraint;
	double** Q;
    XPRBvar* I;
	double BigM1;
	XPRBvar* B;
public:
	SubProblem(void);
	SubProblem(Data* d, int gam1, int gam2, int gam3);
	~SubProblem(void);
	void BuildModel(void); 
	void UpdateConstrains(double** givenQ);
	void Solve(void);
	void DisplaySol(void);
	double***  getWorstCaseDelta(double** Q);
	double  getAssociatedCost(void);
    double GetInventoryCosts( );
    double GetAvgInventory( );
    double GetBackorderCosts( );
    double GetAvgtBackorder( );
};

