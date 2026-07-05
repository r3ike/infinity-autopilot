#pragma once
#include <cmath>


#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880f
#endif

/*--------------------------------------------------
    Filtro LPF 1 ordine esponenziale
--------------------------------------------------*/

struct ExpFilter
{
    float alpha = 0;
    float y_prev = 0;

    void computeCoefficient(float fs,float fc){
        alpha = expf(-(2*M_PI*fc)/fs);
    }

    void setCoefficient(float alpha){
        alpha = alpha;
    }

    float apply(float x){
        float y = (alpha * y_prev) + (1 - alpha) * x;
        y_prev = y;
        return y;
    }
};
