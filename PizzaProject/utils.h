#ifndef UTILS_H
#define UTILS_H

#include <cmath>

#define PI 3.14159265359

namespace utils
{
    inline float ang_to_rad(const float& in_angle )
    {
        return in_angle * PI / 180.0f;
    }

    inline float rad_to_ang(const float& in_angle )
    {
        return in_angle * 180.0f / PI;
    }
    
}

#endif