#pragma once
#include <filters/BiquadFilter/BiquadFilter.hpp>
#include <filters/ExpFilter/ExpFilter.hpp>

#include <utils/uav_data.hpp>

struct ImuLpfFilter
{
    BiquadFilter _lpf_acc_x;
    BiquadFilter _lpf_acc_y;
    BiquadFilter _lpf_acc_z;

    ExpFilter _lpf_gyro_x;
    ExpFilter _lpf_gyro_y;
    ExpFilter _lpf_gyro_z;

    void init_lpf_acc(float fs, float fc) {

        _lpf_acc_x.compute_butterworth(fs, fc);
        _lpf_acc_y.compute_butterworth(fs, fc);
        _lpf_acc_z.compute_butterworth(fs, fc);
    }

    void init_lpf_gyro(float fs, float fc){
        _lpf_gyro_x.computeCoefficient(fs, fc);
        _lpf_gyro_y.computeCoefficient(fs, fc);
        _lpf_gyro_z.computeCoefficient(fs, fc);
    }

    ImuData apply(ImuData imu_data_raw){
        ImuData imuFiltered;

        imuFiltered.acc.x = _lpf_acc_x.apply(imu_data_raw.acc.x);
        imuFiltered.acc.y = _lpf_acc_y.apply(imu_data_raw.acc.y);
        imuFiltered.acc.z = _lpf_acc_z.apply(imu_data_raw.acc.z);

        imuFiltered.gyro.x = _lpf_gyro_x.apply(imu_data_raw.gyro.x);
        imuFiltered.gyro.y = _lpf_gyro_y.apply(imu_data_raw.gyro.y);
        imuFiltered.gyro.z = _lpf_gyro_z.apply(imu_data_raw.gyro.z);

        imuFiltered.temp = imu_data_raw.temp;

        return imuFiltered;
    }
};
