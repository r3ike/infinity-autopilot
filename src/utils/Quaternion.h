#pragma once
#include <cmath>
#include <utils/Vector3f.h>

struct Quaternion
{
    float w, x, y, z;

    Quaternion() : w(1), x(0), y(0), z(0) {}

    // From euler angles to quaternion
    Quaternion(float roll, float pitch, float yaw){
        float cr = cos(roll / 2.0);
        float sr = sin(roll / 2.0);
        float cp = cos(pitch / 2.0);
        float sp = sin(pitch / 2.0);
        float ch = cos(yaw / 2.0);
        float sh = sin(yaw / 2.0);

        
        w = cr * cp * ch + sr * sp * sh;
        x = sr * cp * ch - cr * sp * sh;
        y = cr * sp * ch + sr * cp * sh;
        z = cr * cp * sh - sr * sp * ch;
        
    }

    Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

    void normalize(){
        float norm = std::sqrt(w * w + x * x + y * y + z * z);  //std:: definisce che vogliamo usare nello specifico la funzione della libreria di cmath
        /*
        w /= norm;
        x /= norm;
        y /= norm;
        z /= norm;*/

        if(norm > 1e-6f){
            w /= norm;
            x /= norm;
            y /= norm;
            z /= norm;
        }
        /*
        float n = std::sqrt(w * w + x * x + y * y + z * z);
        if (n > 1e-6f) { w /= n; x /= n; y /= n; z /= n; }*/

    }

    Quaternion operator* (const Quaternion& q) const {
        return{
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        };
    }

    Vector3f rotateVecFromBodyToENU (const Vector3f& v){
        return {0,0,0};
    }
};
