#include "defineAlgorithm.h"
#include <cmath>

float calculateDistance(float lat1, float lon1, float lat2, float lon2) 
{
    float dx = lat2 - lat1;
    float dy = lon2 - lon1;
    return std::sqrt(dx * dx + dy * dy) * 111000.0f;
}

float calculateHeading(float lat1, float lon1, float lat2, float lon2) 
{
    float dx = lon2 - lon1;
    float dy = lat2 - lat1;
    return std::atan2(dx, dy) * (180.0f / M_PI);
}