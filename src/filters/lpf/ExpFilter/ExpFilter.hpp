#pragma once
#include <math.h>

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
