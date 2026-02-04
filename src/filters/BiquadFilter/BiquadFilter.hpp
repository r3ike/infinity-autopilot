#pragma once
#include <math.h>


/*------------------------------
    Implementazione filtro LPF
    del secondo ordine biquad 
--------------------------------*/

struct BiquadFilter {
    float b0, b1, b2;
    float a1, a2;

    float x1 = 0, x2 = 0;
    float y1 = 0, y2 = 0;

    void setCoefficients(float b0_, float b1_, float b2_, float a1_, float a2_) {
        b0 = b0_; b1 = b1_; b2 = b2_;
        a1 = a1_; a2 = a2_;
    }
    /*
        fs = Frequency sample
        fc = Frequency cutoff
    */
    void compute_butterworth(float fs, float fc) {

        float K = tanf(M_PI * fc / fs);
        float norm = 1.0f / (1.0f + M_SQRT2 * K + K*K);
        b0 = K*K * norm;
        b1 = 2.0f * b0;
        b2 = b0;
        a1 = 2.0f * (K*K - 1.0f) * norm;
        a2 = (1.0f - M_SQRT2*K + K*K) * norm;
    }

    float apply(float x) {
        float y = b0*x + b1*x1 + b2*x2 - a1*y1 - a2*y2;
        x2 = x1; x1 = x;
        y2 = y1; y1 = y;
        return y;
    }
};





