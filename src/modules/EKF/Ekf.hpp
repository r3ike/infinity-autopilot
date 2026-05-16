#pragma once

#include <Eigen/Core>

class Ekf
{
private:
    /* data */
public:
    Ekf();
    ~Ekf() = default;

    void updateGps();

    void updateLidar();

    void updateMag();

    void updateBaro();
};



