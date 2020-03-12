#include "iparametric.hpp"
#include <stdio.h>

typedef ValueInterpolator<double> ParamD;

void main()
{
    ParamD dp(0,1, 0,255);

    for (double i=0; i <=1.0; i += 0.1) {
        printf("dp, u: %f  v: %f\n", i, dp(i));
    }

}