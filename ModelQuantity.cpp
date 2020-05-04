#include <cstddef>
#include "ModelQuantity.h"
#include <iostream>
#include <cmath>

//#include "xprb_cpp.h"
//#include "xprs.h"
using namespace ::dashoptimization;

using namespace std;

/*void print(char* s)
{
	if(false)
		cout<<s<<endl; 
}*/

ModelQuantity::ModelQuantity(void)
{
	cout<<"Not Implemented: DO NOT USE" <<endl;
}

ModelQuantity::ModelQuantity( Data* d, int gamma1, int gamma2, int gamma3 )
{
	 this->ModSub = new SubProblem(d, gamma1, gamma2, gamma3 );
	 
	 ModSub->BuildModel();
	 this->D=d;
	 Data::print("start");
	 pbQ = new XPRBprob("MyProb");
    XPRSprob opt_prob =  this->pbQ->getXPRSprob();
    XPRSsetintcontrol(opt_prob,XPRS_MAXTIME,  this->D->getTimeLimite());
    XPRSsetintcontrol(opt_prob,XPRS_MIPTHREADS,  1);
	 pbQ->setMsgLevel(0);
	 W= 1;
	 delta = new double***[this->W+1];
	 for(int w=1; w<=W; w++)
	 {
		 delta[w]= new double**[this->D->getNPer()+1]; 
		 for(int tau=1; tau<=this->D->getNPer(); tau++)
		 {
			delta[w][tau] = new double*[this->D->getNPer()+1];
			for(int t=1; t<=this->D->getNPer(); t++)
			{
				delta[w][tau][t] = new double[this->D->getNSup()+1];
				for(int s=1; s<=this->D->getNSup(); s++) {
                    if (tau > t - this->D->getLMin(s - 1))
                        delta[w][tau][t][s] = 0;
                    else
                        delta[w][tau][t][s] = 1;
                }
			}
		 }
	  }

	/* givenY = new int*[this->D->getNSup()+1]; 
	 for(int s=1; s<=this->D->getNSup(); s++)
	 {
		 givenY[s] =  new int[this->D->getNPer()];
	 }*/

	Q = new XPRBvar*[this->D->getNPer()+1];
	for(int t=1; t<=this->D->getNPer(); t++)
    {
	  Q[t] = new XPRBvar[this->D->getNSup()+1];
	  for(int s= 1; s<=this->D->getNSup(); s++)
	  {
		Q[t][s] = this->pbQ->newVar("Q",XPRB_PL,0.0,XPRB_INFINITY); //XPRB_BV
	  }
    }

	Y = new XPRBvar*[this->D->getNPer()+1];
	for(int t=1; t<=this->D->getNPer(); t++)
    {
	  Y[t] = new XPRBvar[this->D->getNSup()+1];
	  for(int s= 1; s<=this->D->getNSup(); s++)
	  {
		Y[t][s] = this->pbQ->newVar("Y",XPRB_PL,0.0,1.0); //XPRB_BV
	  }
    }



}


ModelQuantity::~ModelQuantity(void)
{
}

void ModelQuantity::BuildModel(void){


	//Q: array(T, S) of mpvar ! Qtty ordered from
	//								!supplier s in period t
	
	
	Data::print("Define variables");
	

	//c: array(T,WMax) of mpvar	! total holding or backordering cost in period t for scenario w
/*    XPRBvar** c= new XPRBvar*[this->D->getNPer()+1];
	for(int t=1; t<=this->D->getNPer(); t++)
	{
		c[t] =  new XPRBvar[this->W+1];
		for(int w =1; w<=this->W; w++)
		{
			c[t][w] = this->pbQ->newVar("c"); //XPRB_BV
		}
	} */
	//I: array(T) of mpvar	! Inventory level at end of period t
	XPRBvar* I= new XPRBvar[this->D->getNPer()+1];
	for(int t=1; t<=this->D->getNPer(); t++)
	{
		I[t] = this->pbQ->newVar("I",XPRB_PL,0.0,XPRB_INFINITY);  //XPRB_BV
	}

	//Cw: array(WMax) of mpvar	! The worst case scenario cost.
/*	XPRBvar* Cw= new XPRBvar[this->W+1];
	for(int w =1; w<=this->W; w++)
		{
			Cw[w] = this->pbQ->newVar("Cw"); //XPRB_BV
		}*/
	//C: mpvar	! The worst case scenario cost.
	this->C =  this->pbQ->newVar("C",XPRB_PL,0.0,XPRB_INFINITY);
	

	double BigM = 0; 
	for(int t=1; t<=this->D->getNPer(); t++)
	{
		BigM+=this->D->getDemand(t-1);
	}
	
	Data::print("Define Constraints");


  //!Constraint related to holding cost
  //forall(t in T, w in W)
  //	ConstraintHolding(t,w) := c(t,w) >= h*(sum(tau in 1..t, s in S) delta(tau,t,s,w) * Q(tau,s)-sum(tau in 1..t)d(tau))
/*    XPRBexpr** cumulativeQuantity=new XPRBexpr*[this->W+1];

	for(int w =1; w<=this->W; w++)
	{
		cumulativeQuantity[w]=new XPRBexpr[this->D->getNPer()+1];
		for(int t = 1; t<= this->D->getNPer(); t++)
		{
		  cumulativeQuantity[w][t] = *new XPRBexpr();
		  for(int s= 1; s<=this->D->getNSup(); s++)
		  {
			for(int tau = 1; tau <= t; tau++)
			{
				cumulativeQuantity[w][t] += this->delta[w][tau][t][s]*Q[tau][s];
			}
		  }
		}
	}
	Data::print("Create expression cumulativeDemand ");
	XPRBexpr* cumulativeDemand=new XPRBexpr[this->D->getNPer()+1];
	for(int t = 1; t<= this->D->getNPer(); t++)
	{
		cumulativeDemand[t] = *new XPRBexpr();
		  for(int tau = 1; tau <= t; tau++)
        {
            cumulativeDemand[t] += this->D->getDemand(tau);
        }
	}
	Data::print("ConstraintHoldin");
	for(int w= 1; w<=this->W; w++)
		for(int t =1; t<= this->D->getNPer(); t++)
		{
			this->pbQ->newCtr(XPRBnewname("ConstraintHolding%d%d",w,t),
								c[t][w] >= this->D->getch()  * (cumulativeQuantity[w][t] - cumulativeDemand[t] )   );
	
		}
//forall(t in T, w in W)
//	ConstraintBackorder(t,w) := c(t, w) >= -b*(sum(tau in 1..t, s in S) delta(tau,t,s,w) * Q(tau,s)-sum(tau in 1..t)d(tau))
	Data::print("ConstraintBacklog");
		for(int w= 1; w<=W; w++)
		for(int t =1; t<= this->D->getNPer(); t++)
		{
			this->pbQ->newCtr(XPRBnewname("ConstraintBacklog%d%d",t),
										c[t][w] >= -this->D->getcb() * (cumulativeQuantity[w][t] - cumulativeDemand[t] ) );
		}
   //forall(w in W)
	//ConstraintCostScenario(w) := Cw(w) = sum(t in T)  c(t, w)
	Data::print("ConstraintCostScenario");
	XPRBexpr* cumulativeCost=new XPRBexpr[this->W+1];
	for(int w= 1; w<=this->W; w++)
	{
		cumulativeCost[w]  = *new XPRBexpr();
		for(int t =1; t<= this->D->getNPer(); t++)
        {
            cumulativeCost[w] += c[t][w];
        }
	}

	for(int w= 1; w<=W; w++)
		this->pbQ->newCtr(XPRBnewname("ConstraintCostScenario%d",w),
										Cw[w] >= cumulativeCost[w] );
		
	Data::print("ConstraintWorstCaseCost");
//forall(w in W)
//	ConstraintWorstCaseCost(w) := C >= Cw(w)
	for(int w= 1; w<=this->W; w++)
		this->pbQ->newCtr(XPRBnewname("ConstraintWorstCaseCost%d",w),
										C >= Cw[w] );
	*/
//!Constraint link between binary variables Y(t,s) and quantities Q(t,s)
//forall(t in T, s in S)
//	ConstraintBigM(t,s) := Q(t,s) <= BigM*YY(t,s)
	Data::print("ConstraintBigM");
	BigMConstraints = new XPRBctr*[ this->D->getNPer()+1];
	for(int t = 1; t<= this->D->getNPer(); t++)
	{
	   BigMConstraints[t] = new XPRBctr[ this->D->getNSup()+1];

	  for(int s= 1; s<=this->D->getNSup(); s++)
	  {
			BigMConstraints[t][s]=this->pbQ->newCtr(XPRBnewname("ConstraintBigM%d%d",t,s),
									this->Q[t][s] <= this->Y[t][s]*BigM );
	  }
	}

//Constrain_SatisfyAllDemand := sum(t in T, s in S) Q(t,s) >= sum(t in T) d(t)
	Data::print("cumulativeQuantityTot");
	XPRBexpr* cumulativeQuantityTot=new XPRBexpr();
	for(int s= 1; s<=this->D->getNSup(); s++)
	{
		for(int t =1; t<= this->D->getNPer(); t++)
        {
            *cumulativeQuantityTot += this->Q[t][s];
        }
	}
	Data::print("Create expression cumulativeDemand ");
	XPRBexpr* cumulativeDemand=new XPRBexpr[this->D->getNPer()+1];
	for(int t = 1; t<= this->D->getNPer(); t++)
	{
		cumulativeDemand[t] = *new XPRBexpr();
		  for(int tau = 1; tau <= t; tau++)
        {
            cumulativeDemand[t] += this->D->getDemand(tau-1);
        }
	}
	Data::print("MeetDemand");
	this->pbQ->newCtr(XPRBnewname("MeetDemand"),
						   *cumulativeQuantityTot >= cumulativeDemand[this->D->getNPer()] );


   
	for(int w =1; w<=this->W; w++)
	{
		this->AddScenario(this->delta[w]);
	}
	
	Data::print("Build Objective");
    //TotalCost := C + sum(t in T, s in S)(YY(t,s)*o(s)+Q(t,s)*p(s))
    XPRBexpr lobj;
	lobj += C;
	for(int t = 1; t<= this->D->getNPer(); t++)
		for(int s= 1; s<=this->D->getNSup(); s++)
			lobj += ( Q[t][s]*this->D->getPrice(s-1) + this->Y[t][s]*this->D->getSetup(s-1));
	

	this->pbQ->setObj(this->pbQ->newCtr("OBJ", lobj));
	this->pbQ->setSense(XPRB_MINIM);
}

void ModelQuantity::SetYToValue(int** givenY)
{
	Data::print("update constraint");
   this->totalsetupcosts = 0;
	for(int t = 1; t<= this->D->getNPer(); t++)
	{
	  for(int s= 1; s<=this->D->getNSup(); s++)
	  {
          if(givenY[s-1][t-1] == 0)
          {
              this->Y[t][s].setLB(1.0* givenY[s-1][t-1]);
              this->Y[t][s].setUB(1.0* givenY[s-1][t-1]);
          } else{
              this->Y[t][s].setUB(1.0* givenY[s-1][t-1]);
              this->Y[t][s].setLB(1.0* givenY[s-1][t-1]);

          }


		  this->totalsetupcosts += this->D->getSetup(s-1)*givenY[s-1][t-1];
	  }
	}
}

void ModelQuantity::SetYBinary()
{
	Data::print("update constraint");

	for(int t = 1; t<= this->D->getNPer(); t++)
	{
	  for(int s= 1; s<=this->D->getNSup(); s++)
	  {
		  this->Y[t][s].setType(XPRB_BV);
		  this->Y[t][s].setLB(0.0);
		  this->Y[t][s].setUB(1.0);
	  }
	}
}

void ModelQuantity::UpdateLastScenario(double*** givendelta)
{

    int**  cumulativeQuantity=new int*[this->D->getNPer()+1];
    for(int t = 1; t<= this->D->getNPer(); t++)
    {
        cumulativeQuantity[t] = new int[this->D->getNSup()+1];
        for(int s= 1; s<=this->D->getNSup(); s++)
        {
            for(int tau = 1; tau <= t; tau++)
            {
                cumulativeQuantity[t][s] += givendelta[tau][t][s];
            }
        }

    }



    for(int t=1; t<=this->D->getNPer(); t++)
    {
          for(int s =1; s<=this->D->getNSup(); s++)
            {
                this->HoldingConstraint[t].setTerm(this->Q[t][s], -this->D->getch() * cumulativeQuantity[t][s]);
                this->BackorderConstraint[t].setTerm(this->Q[t][s], +this->D->getcb() * cumulativeQuantity[t][s]);
            }

    }

    for(int t = 1; t<= this->D->getNPer(); t++) {
        delete[] cumulativeQuantity[t];
    }
    delete[] cumulativeQuantity;


}

void ModelQuantity::AddScenario(double*** givendelta)
{

	Data::print("Define variables");
	

	//c: array(T,WMax) of mpvar	! total holding or backordering cost in period t for scenario w
    XPRBvar* c= new XPRBvar[this->D->getNPer()+1];
	for(int t=1; t<=this->D->getNPer(); t++)
	{
		c[t]  = this->pbQ->newVar("c",XPRB_PL,0.0,XPRB_INFINITY);  //XPRB_BV
		
	}
	
	//Cw: array(WMax) of mpvar	! The worst case scenario cost.
	XPRBvar Cw= this->pbQ->newVar("Cw",XPRB_PL,0.0,XPRB_INFINITY); //XPRB_BV
		


	double BigM = 0; 
	for(int t=1; t<=this->D->getNPer(); t++)
	{
		BigM+=this->D->getDemand(t-1);
	}
	
	Data::print("Define Constraints");


  //!Constraint related to holding cost
  //forall(t in T, w in W)
  //	ConstraintHolding(t,w) := c(t,w) >= h*(sum(tau in 1..t, s in S) delta(tau,t,s,w) * Q(tau,s)-sum(tau in 1..t)d(tau))
     XPRBexpr*  cumulativeQuantity=new XPRBexpr[this->D->getNPer()+1];
	 for(int t = 1; t<= this->D->getNPer(); t++)
	 {
		  cumulativeQuantity[t] = *new XPRBexpr();
		  for(int s= 1; s<=this->D->getNSup(); s++)
		  {
			for(int tau = 1; tau <= t; tau++)
			{
				cumulativeQuantity[t] += givendelta[tau][t][s]*Q[tau][s];

			}
		  }
		
	}
	Data::print("Create expression cumulativeDemand ");
	XPRBexpr* cumulativeDemand=new XPRBexpr[this->D->getNPer()+1];
	for(int t = 1; t<= this->D->getNPer(); t++)
	{
		cumulativeDemand[t] = *new XPRBexpr();
		  for(int tau = 1; tau <= t; tau++)
        {
            cumulativeDemand[t] += this->D->getDemand(tau-1);
        }
	}
     this->HoldingConstraint = new XPRBctr[this->D->getNPer()+1];
	Data::print("ConstraintHoldin");
		for(int t =1; t<= this->D->getNPer(); t++)
		{
            this->HoldingConstraint[t] = this->pbQ->newCtr(XPRBnewname("ConstraintHolding%d",t),
								c[t] >= this->D->getch()  * (cumulativeQuantity[t] - cumulativeDemand[t] )   );
	
		}
//forall(t in T, w in W)
//	ConstraintBackorder(t,w) := c(t, w) >= -b*(sum(tau in 1..t, s in S) delta(tau,t,s,w) * Q(tau,s)-sum(tau in 1..t)d(tau))
	Data::print("ConstraintBacklog");
    this->BackorderConstraint = new XPRBctr[this->D->getNPer()+1];
		for(int t =1; t<= this->D->getNPer(); t++)
		{
            BackorderConstraint[t]=this->pbQ->newCtr(XPRBnewname("ConstraintBacklog%d",t),
										c[t] >= -this->D->getcb() * (cumulativeQuantity[t] - cumulativeDemand[t] ) );
		}
   //forall(w in W)
	//ConstraintCostScenario(w) := Cw(w) = sum(t in T)  c(t, w)
	Data::print("ConstraintCostScenario");
	XPRBexpr cumulativeCost  = *new XPRBexpr();
		for(int t =1; t<= this->D->getNPer(); t++)
        {
            cumulativeCost += c[t];
        }
	

	this->pbQ->newCtr(XPRBnewname("ConstraintCostScenario"),
										Cw >= cumulativeCost );
		
	Data::print("ConstraintWorstCaseCost");
//forall(w in W)
//	ConstraintWorstCaseCost(w) := C >= Cw(w)
		this->pbQ->newCtr(XPRBnewname("ConstraintWorstCaseCost"),
										this->C >= Cw );

		
}



void ModelQuantity::Solve(void)
{
	this->pbQ->mipOptimize();
}


void ModelQuantity::DisplaySol(void)
{
	cout<<"****************SOLUTION***************"<<endl;
	for(int t = 1; t<= this->D->getNPer(); t++)
		for(int s= 1; s<=this->D->getNSup(); s++)
		   cout << this->Q[t][s].getName() << ":" << this->Q[t][s].getSol() << " ";  
    cout << endl;
	cout<<"**************************************"<<endl;
}

double ModelQuantity::GetPurshasingCosts( ) {
    double result = 0.0;
     for(int t=1; t <= this->D->getNPer(); t++)
    {

        for(int s=1; s<=this->D->getNSup(); s++)
        {
            result+=this->Q[t][s].getSol() * this->D->getPrice(s-1);
        }

    }
    return result;
}

double ModelQuantity::GetOrderingCosts( ) {
    double result = 0.0;
    for(int t=1; t <= this->D->getNPer(); t++)
    {

        for(int s=1; s<=this->D->getNSup(); s++)
        {
            result+=this->Y[t][s].getSol() * this->D->getSetup(s-1);
            //cout<<this->Y[t][s].getSol() <<"__"<< this->D->getSetup(s-1)<<endl;
        }

    }
    return result;
}

double ModelQuantity::GetInventoryCosts() {
    this->ModSub->GetInventoryCosts();
}


double ModelQuantity::GetAvgInventory( )
{
    this->ModSub->GetInventoryCosts();
}

double ModelQuantity::GetBackorderCosts() {
    this->ModSub->GetBackorderCosts();
}


double ModelQuantity::GetAvgtBackorder( )
{
    this->ModSub->GetAvgtBackorder();
}


double** ModelQuantity::getQuantities( )
{
	
	
	Data::print("Backlog/Inventory Cost:",this->C.getSol());
	this->totalsetupcosts = 0.0;
	double** sol_Q = new double*[this->D->getNPer()+1]; 
    for(int t=1; t <= this->D->getNPer(); t++)
	{
		sol_Q[t] = new double[this->D->getNSup()+1];
		
		for(int s=1; s<=this->D->getNSup(); s++)
			{sol_Q[t][s] = this->Q[t][s].getSol();
			this->totalsetupcosts+=this->Y[t][s].getSol() * this->D->getSetup(s-1);
			}
			
	  }
	return sol_Q;
}
double ModelQuantity::getCost()
{
	return this->pbQ->getObjVal();
}


double ModelQuantity::Solve(bool givenY, int** givenYvalues, bool fastUB, double stopatgap)
{
	double UB= 9999999999.0; 
	double LB= 0.0; 
	int nriteration =0;
	if(givenY)
	{
		this->SetYToValue(givenYvalues);
	}
	else
	{
		this->SetYBinary();
	}

    clock_t start, end;
    start = clock();
    double temps=0;

	while((UB-LB)/UB>stopatgap && (!fastUB || nriteration<1) && temps <= this->D->getTimeLimite())
	{
		 nriteration++;
        //this->pbQ->exportProb(1,"lpq");
        XPRSprob opt_prob =  this->pbQ->getXPRSprob();
        XPRSsetintcontrol(opt_prob,XPRS_MIPTHREADS,  1);
        bool status=false;
        if(givenY) {
            this->pbQ->lpOptimise();
            status = this->pbQ->getLPStat() == 1;
        }
        else{
             this->pbQ->mipOptimise();
             status = this->pbQ->getMIPStat() == 6 || this->pbQ->getMIPStat() == 4 ;
        }

         if(!status)
         { return XPRB_INFINITY;}

		 double ** associatedquantities =  this->getQuantities();
		 LB = this->getCost();
		 Data::print("get the associated costs", this->getCost());
		 double totprice = 0.0;
		 for(int s=1; s<=this->D->getNSup(); s++)
		 {
			 for(int t=1; t<=this->D->getNPer(); t++)
			  {
				  totprice+=associatedquantities[t][s]*this->D->getPrice(s-1);
			  }
		  }

		 /*          The function below are not implemnted      */

		int gamma = 3;
	
		double*** worstdelta = this->ModSub->getWorstCaseDelta(associatedquantities);
        //this->ModSub->DisplaySol();
		UB = ModSub ->getAssociatedCost() +this->totalsetupcosts + totprice;
	
		//cout<<"LB: " << LB << " UB:"<<UB<<  "  setup:"<<this->totalsetupcosts << " price:" <<totprice<< endl;
		 this->AddScenario(worstdelta);

        end = clock();
        temps = (double) (end-start)/ CLOCKS_PER_SEC;

	}
	this->LastRunning = temps;
    this->LastGap = (UB-LB)/LB;
    //cout<<"optimization cost"<<UB<<endl;
   // cout<<"Inv Cost:"<<this->GetInventoryCosts()<<" Avg Inv:"<<this->GetAvgInventory()<<" Order Cost:"<<this->GetOrderingCosts()<<endl;
   // cout<<"Back Cost:"<<this->GetBackorderCosts()<<" Avg Back:"<<this->GetAvgtBackorder()<<" Pursh cost:"<<this->GetPurshasingCosts()<<endl;

    return UB;
}
