#ifndef PATH_ALGORITHMS_H
#define PATH_ALGORITHMS_H

#include "Object.h"
#include <tuple>
#include <utility>
#include <cmath>
#include <algorithm>

class PathCalculator
{
public:
    virtual ~PathCalculator() = default;
    virtual std::tuple<int, int, int> calculatePath(const Object &follower, const Object &target) = 0;
};

class HeatSignature : public PathCalculator
{
public:
    explicit HeatSignature(float heatLevel = 50.0f);
    void setHeatLevel(float heatLevel);
    std::tuple<int, int, int> calculatePath(const Object &follower, const Object &target) override;

private:
    float heatLevel;
};

class GPSAlgorithm : public PathCalculator
{
public:
    explicit GPSAlgorithm(std::tuple<int, int, int> gpsCoordinates = {0, 0, 0});
    void setGPSData(const std::tuple<int, int, int> &gpsData);
    std::tuple<int, int, int> calculatePath(const Object &follower, const Object &target) override;

private:
    std::tuple<int, int, int> gpsCoordinates;
};

class DeadReckoning : public PathCalculator
{
public:
    std::tuple<int, int, int> calculatePath(const Object &follower, const Object &target) override;
};

class KalmanFilter : public PathCalculator
{
public:
    std::tuple<int, int, int> calculatePath(const Object &follower, const Object &target) override;
};

#endif // PATH_ALGORITHMS_H