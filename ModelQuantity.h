#pragma once
#include "xprb_cpp.h"
#include "xprs.h"
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
	double LastRunning;
	int LastNrIteration;
    double LastGap;
    double LastLB;
    int LastNrNode;
    int LastStatus;
    int nriteration;
    //int ** givenY;
	XPRBvar** Q;
	XPRBvar** Y;
	XPRBvar C;
	XPRBctr** BigMConstraints;
    XPRBctr* HoldingConstraint;
    XPRBctr* BackorderConstraint;
	Data* D;
	int Gamma1;
	int Gamma2;
	int Gamma3;
	double totalsetupcosts;
    int nriterationfixandopt;
    double durationFixAndOpt;
    double TimeBastSolFixAndOpt;
public:
	
	ModelQuantity( void );
	ModelQuantity( Data* d, int gamma1, int gamma2, int gamma3 );
	~ModelQuantity(void);
	void BuildModel(void); 
	void SetYToValue(int** Y);
	void Solve(void);
	void DisplaySol(void);
	double getCost();
	double** getQuantities();
	void AddScenario(double*** givendelta);
	double Solve(bool givenY, int** givenY2, bool fastUB, double stopatgap);
    void UpdateLastScenario(double*** givendelta);
	void SetYBinary();
    void OpenInteval(int a, int b, int** givenYvalues);
    void GetYResults( int** givenYvalues );
    double GetPurshasingCosts( );
    double GetOrderingCosts( );
    double GetInventoryCosts( );
    double GetAvgInventory( );
    double GetBackorderCosts( );
    double GetAvgtBackorder( );
	void FixInitSol(void);
    void FixAndOpt(void);
    void FixNonSelectSuppliers(int a, int b, int** givenYvalues);

};

