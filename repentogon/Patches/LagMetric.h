#pragma once
#include <chrono>
#include "IsaacRepentance.h"
namespace LagMetric{
    extern int _lagMetric;
    extern int _lastLagMetric;
    extern std::chrono::system_clock::time_point _lastFrameTime;
    extern bool _init;
    extern int _lastMillis;
    inline bool is_circle_laser(Entity_Laser* in);
}