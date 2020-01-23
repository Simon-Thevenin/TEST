#ifndef FGENETIC_H
#define FGENETIC_H

#include <iostream>
#include <string>

#include <time.h>
#include <fstream>
#include <cstdlib>

#include "Data.h"
#include "Solution.h"

class FGenetic
{
    protected:

    private:
        Data *_data;

    public:
        FGenetic(Data *data);
        virtual ~FGenetic();

        double RCOST(int*** X, int a, Data *data);
        void PopInit(int nb_c, int*** n_g, double* fit, Data *data);
        void Cross(int *_nbcc0, int _pc, int*** f1, int s_2, int nb_c, int ***par_p, double* f_cross, Data *data);
        void Mutation(int *_nbcm0, int _pm, int*** f2, int s_2, int nb_c, int*** g, int* i1, Data *data);
        int val_rand(int a,int b);
};

#endif // FGENETIC_H
