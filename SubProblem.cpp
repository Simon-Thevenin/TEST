#include "../../OneDrive/Documents/UncertainLT/UncertainLT/stdafx.h"
#include "SubProblem.h"


void print(char* s)
{
	if(false)
		cout<<s<<endl; 
}

SubProblem::SubProblem(void)
{
}

SubProblem::SubProblem(Data* d, int gam)
{
	Data::print("Start Sub Prob");
	this->Gamma = gam;
	this->data=d;
	this->pbSub = new XPRBprob("SubProb");
	this->pbSub->setMsgLevel(0);

	Q = new double*[this->data->getNPer()+1];
	for(int t=1; t<=this->data->getNPer(); t++)
    {
	  Q[t] = new double[this->data->getNSup()+1];
	  for(int s= 1; s<=this->data->getNSup(); s++)
	  {
		Q[t][s] = 0.0; //XPRB_BV
	  }
    }
	//delta: array(T, T, S) of mpvar !equal 1 if
	//								! order passed in t1 from supplier s
	//								! is received in t2
    this->delta= new XPRBvar**[this->data->getNPer()+1];
	for(int t=1; t<=this->data->getNPer(); t++)
	{
		this->delta[t]= new XPRBvar*[this->data->getNPer()+1];
		for(int tau=1; tau<=this->data->getNPer(); tau++)
		{  
			this->delta[t][tau]= new XPRBvar[this->data->getNSup()+1];
			for(int s=1; s<=this->data->getNSup(); s++)
			{
				this->delta[t][tau][s] = this->pbSub->newVar("delta");
			}
		}
	}
}

SubProblem::~SubProblem(void)
{
}

void SubProblem::BuildModel(void){

	
	Data::print("Define variables");
	
	//I: array(T) of mpvar	! Inventory level at end of period t
	
	XPRBvar* I= new XPRBvar[this->data->getNPer()+1];
	for(int t=1; t<=this->data->getNPer(); t++)
		I[t] = this->pbSub->newVar("I");

	//B: array(T) of mpvar	! Inventory level at end of period t
	 B= new XPRBvar[this->data->getNPer()+1];
	for(int t=1; t<=this->data->getNPer(); t++)
		B[t] = this->pbSub->newVar("B");

   //alpha: array(T) of mpvar	! Inventory level at end of period t
	XPRBvar* alpha= new XPRBvar[this->data->getNPer()+1];
	for(int t=1; t<=this->data->getNPer(); t++)
		alpha[t] = this->pbSub->newVar("alpha",XPRB_BV);

	
	//BigM1 := sum(t in T)d(t)
    //BigM2 := sum(t in T, s in S)Q(t,s)
	//BigM := BigM1
	//if BigM2 > BigM1 then
	//   BigM := BigM2
    //end-if
	BigM1 = 0; 
	for(int t=1; t<=this->data->getNPer(); t++)
	{
		BigM1+=this->data->getDemand(t);
	}
	 

	//TotalCost :=  sum(t in T)( I(t)*h + B(t)*b )
	Data::print("Build Objective");
    XPRBexpr lobj;
	for(int t = 1; t<= this->data->getNPer(); t++)
		  lobj += (I[t]*this->data->getch() + B[t]*this->data->getcb() );
	
	this->pbSub->setObj(this->pbSub->newCtr("OBJ", lobj));
	this->pbSub->setSense(XPRB_MAXIM);

	//forall(t in T)
	//ConstraintInventoryBack(t) := I(t) - B(t) = (sum(tau in 1..t, s in S) delta(tau,t,s) * sol_Q(tau,s)-sum(tau in 1..t)d(tau))
	XPRBexpr* cumulativeQty=new XPRBexpr[this->data->getNPer()+1];
	for(int t=1; t<=this->data->getNPer(); t++)
	{
		cumulativeQty[t] = *new XPRBexpr();
		for(int tau=1; tau<=t; tau++)
		{
			for(int s =1; s<=this->data->getNSup(); s++)
			{
				cumulativeQty[t] += delta[tau][t][s] * Q[tau][s];
			}
			cumulativeQty[t] += -this->data->getDemand(tau);
		
		}
	}
	this->InventoryConstraint = new XPRBctr[this->data->getNPer()+1];
	for(int t=1; t<=this->data->getNPer(); t++)
		this->InventoryConstraint[t] = this->pbSub->newCtr(XPRBnewname("ConstraintInventoryBack%d",t),
								I[t] - B[t] == cumulativeQty[t]  );

		

	
    //forall(tau in T, t in T, s in S)
	//if t < Periods then
	//	Constraintdelta1(tau,t,s) :=  delta(tau,t,s) <= delta(tau,t+1,s)
	//end-if
	for(int t=1; t<this->data->getNPer(); t++)
		for(int tau=1; tau<=this->data->getNPer(); tau++)
			for(int s= 1; s<=this->data->getNSup(); s++)
					this->pbSub->newCtr(XPRBnewname("Constraintdelta1%d%d%d",t, tau, s),
								delta[tau][t][s] <= delta[tau][t+1][s] );
		
	//forall(t in T)
	//	if t > maxL  then
	//		Constraintdelta2(t) :=  sum(s in S) delta(t-Lmin(s),t,s) >= -Gamma + Suppliers
	//	end-if
	double maxL =0; 
	for(int s= 1; s<=this->data->getNSup(); s++)
		if(this->data->getLMax(s) >= maxL)
			maxL = this->data->getLMax(s);

	
	
	XPRBexpr* cumulativeDelta=new XPRBexpr[this->data->getNPer()+1];
	for(int t=1; t<this->data->getNPer(); t++)
	{
		cumulativeDelta[t] = *new XPRBexpr();
		for(int s =1; s<=this->data->getNSup(); s++)
		{
			cumulativeDelta[t] += delta[t-this->data->getLMin(s)][t][s];
		}
	}
	for(int t=1; t<this->data->getNPer(); t++)
	{
		if( t> maxL)
		{
		 this->pbSub->newCtr(XPRBnewname("Constraintdeltat%d",t),
			 cumulativeDelta[t] >= -this->Gamma + this->data->getNSup() );
		}					
	}

	//forall(tau in T, t in T, s in S) do
	//if t >tau+Lmax(s) then
	//	Constraintdelta3(tau,t,s) := delta(tau,t,s) = 1
	//end-if
	for(int t=1; t<=this->data->getNPer(); t++)
		for(int tau=1; tau<=this->data->getNPer(); tau++)
			for(int s= 1; s<=this->data->getNSup(); s++)
			{	if(t>tau+this->data->getLMax(s))
				{
					this->pbSub->newCtr(XPRBnewname("Constraintdelta3%d%d%d", tau, t, s),
								delta[tau][t][s] == 1);
				}
			    if(t<tau-this->data->getLMin(s))
				{
					this->pbSub->newCtr(XPRBnewname("Constraintdelta4%d%d%d", tau, t, s),
								delta[tau][t][s] == 0);
				}
				this->pbSub->newCtr(XPRBnewname("Constraintdelta5%d%d%d", tau, t, s),
							     	delta[tau][t][s] <= 1);
				this->pbSub->newCtr(XPRBnewname("Constraintdelta6%d%d%d", tau, t, s),
									delta[tau][t][s] >= 0);
			}


	for(int t=1; t<=this->data->getNPer(); t++)
	{
		//ConstraintInv(t) := I(t)  >= 0
		this->pbSub->newCtr(XPRBnewname("ConstraintInv%d",  t),
							I[t] >= 0);
		//ConstraintBacj(t) := B(t) >= 0
		this->pbSub->newCtr(XPRBnewname("ConstraintBacj%d",  t),
							B[t] >= 0);
		//ConstraintInv2(t) := I(t)  <= alpha(t)*BigM
		this->pbSub->newCtr(XPRBnewname("ConstraintInv2%d",  t),
							I[t] <= alpha[t]*BigM1);
		//ConstraintBac2(t) := B(t) <= (1- alpha(t))*BigM
		this->pbSub->newCtr(XPRBnewname("ConstraintBac2%d",  t),
							B[t] <= (1-alpha[t])*BigM1);
		//Constraint_alphaIntegrality(t) := Constraint_alphaIntegrality(t) is_binary
		this->pbSub->newCtr(XPRBnewname("ConstraintInv%d",  t),
							I[t] >= 0);
	}
}

void SubProblem::UpdateConstrains(double** givenQ)
{
	Data::print("update constraint");
	for(int t=1; t<=this->data->getNPer(); t++)
	{
		for(int tau=1; tau<=t; tau++)
		{
			for(int s =1; s<=this->data->getNSup(); s++)
			{
				this->InventoryConstraint[t].setTerm(this->delta[tau][t][s], -1*givenQ[tau][s]);
			}
		}
	}
	

	double BigM2 = 0; 
	for(int t=1; t<=this->data->getNPer(); t++)
	{
		for(int s= 1; s<=this->data->getNSup(); s++)
		  {
			  BigM2+=givenQ[t][s];
		  }
	}
	if( BigM2 > this->BigM1)
		this->BigM1 = BigM2;

}


void SubProblem::Solve(void)
{
	this->pbSub->mipOptimize();
}


void SubProblem::DisplaySol(void)
{
	this->pbSub->mipOptimize();
	for(int t=1; t<=this->data->getNPer(); t++)
	{
		for(int tau=1; tau<=this->data->getNPer(); tau++)
		{
			for(int s=1; s<=this->data->getNSup(); s++)
				cout << this->delta[t][tau][s].getName() << ":" <<this->delta[t][tau][s].getSol() << " ";  
		}
	}

		
 cout << endl;
}



double***  SubProblem::getWorstCaseDelta(double** Q)
{
	Data::print("Compute worst case delta"); 
	this->UpdateConstrains(Q);
	this->pbSub->exportProb(1,"lps");
	this->pbSub->mipOptimize();
	double*** soldelta = new double**[this->data->getNPer()+1]; 
    for(int t=1; t<=this->data->getNPer(); t++)
	{
		
		soldelta[t] = new double*[this->data->getNPer()+1];
		for(int tau=1; tau<=this->data->getNPer(); tau++)
		{
			soldelta[t][tau] = new double[this->data->getNSup()+1];
			for(int s=1; s<=this->data->getNSup(); s++)
				{
					soldelta[t][tau][s] = (this->delta[t][tau][s].getSol());
				}
			
		 }
	  }
	return soldelta;

}

double  SubProblem::getAssociatedCost(void)
{
	return this->pbSub->getObjVal();
}



