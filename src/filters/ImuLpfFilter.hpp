#pragma once
#include <filters/BiquadFilter/BiquadFilter.hpp>
#include <filters/ExpFilter/ExpFilter.hpp>

#include <utils/srimb_topics/imu_topic/imu_topic.hpp>

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

    void apply(ImuData& imu_data){
        

        imu_data.filtered_acc.x = _lpf_acc_x.apply(imu_data.raw_acc.x);
        imu_data.filtered_acc.y = _lpf_acc_y.apply(imu_data.raw_acc.y);
        imu_data.filtered_acc.z = _lpf_acc_z.apply(imu_data.raw_acc.z);

        imu_data.filtered_gyro.x = _lpf_gyro_x.apply(imu_data.raw_gyro.x);
        imu_data.filtered_gyro.y = _lpf_gyro_y.apply(imu_data.raw_gyro.y);
        imu_data.filtered_gyro.z = _lpf_gyro_z.apply(imu_data.raw_gyro.z);


    }
};
