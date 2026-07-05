#pragma once

/**
 * Classe che si occupa di effettuare il preprocessing dei sample delle imu:
 *  - filtering (LPF. notche, ecc..)
 *  - calibration
 *  - checks (...)
 */
class ImuPreprocessor
{
private:
    /* data */
public:
    ImuPreprocessor(/* args */);
    ~ImuPreprocessor() = default;
};

