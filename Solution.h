/*
 * Solution.hpp
 *
 *      Author: Oussama
 *		  Year: 2019
 */

#include <iostream>
#include <string>

#include <time.h>
#include <fstream>

#include "Data.h"

#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

using namespace std;

class Solution
{
	private:


	    Data *data_;

	public:
		Solution(int*** _x, int _a, Data *data);
		virtual ~Solution();
		double solve(int*** _x, int _a);

		void lire1(const char* nom_fichier, double **tab, int n,int u);
		void afficher1(string c , int** tab, int a, int b);
        void afficher(string c , int* tab, int a, int b);
        void afficher(string c , double* tab, int a, int b);


};

#endif /* SOLUTION_HPP_ */
