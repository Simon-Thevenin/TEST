//
// Created by simon on 1/25/2020.
//
#pragma once
#include "xprb_cpp.h"
#include "xprs.h"
#include "xprm_mc.h"
#include "Data.h"

#ifndef UNTITLED_MODELROBUST_H
#define UNTITLED_MODELROBUST_H

using namespace ::dashoptimization;

using namespace std;

class ModelRobust {
public:
    XPRBprob* pbRob;
    Data* data;
    int gamma1;
    int gamma2;
    int gamma3;
   // I: array(T) of mpvar	! Inventory level at end of period t
   XPRBvar* I;
   // Q: array(T, S) of mpvar ! Qtty ordered from    !supplier s in period t
   XPRBvar** Q;
   // Y: array(T, S) of mpvar ! equal 1 if an order is !passed to supplier s in period t
   XPRBvar** Y;
   //Del: array(T, T, S) of mpvar !equal 1 if ! order passed in t1 from supplier s ! is received in t2
   XPRBvar*** Delta;
    // c: array(T) of mpvar	! total holding or backordering cost
    XPRBvar* c;
    //rho: array(T) of mpvar ! dualvariables
    XPRBvar* rho1;
    XPRBvar* rho2;
    XPRBvar* rho3;
    // xhi: array(T,  T, S) of mpvar ! dualvariables
    XPRBvar*** xhi;
    // lambda: array(T,  T, S) of mpvar ! dualvariables
    XPRBvar*** lambda;
    // psi: array(T,  T, S) of mpvar ! dualvariables
    XPRBvar*** psi;
    // rhob: array(T) of mpvar ! dualvariables
    XPRBvar* rhob1;
    XPRBvar* rhob2;
    XPRBvar* rhob3;
    // xhib: array(T,  T, S) of mpvar ! dualvariables
    XPRBvar*** xhib;
    // lambdab: array(T,  T, S) of mpvar ! dualvariables
    XPRBvar*** lambdab;
    // psib: array(T,  T, S) of mpvar ! dualvariables
    XPRBvar*** psib;

    double LastRunning;

public:
    ModelRobust( void );
    ModelRobust( Data* d, int gamma1, int gamma2, int gamma3);
    double** getQuantities( );
    ~ModelRobust(void);
    void Solve(void);
    int** getY();
    double GetSetupCost();
    double GetPurshasingCosts();
};


#endif //UNTITLED_MODELROBUST_H
