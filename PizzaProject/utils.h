#ifndef UTILS_H
#define UTILS_H

#include <cmath>

#define PI 3.14159265359

namespace utils
{
    inline float sexagesimal_to_radian(const float& in_angle )
    {
        return in_angle * PI / 180.0f;
    }
    
}

#endif