#include "PathAlgorithms.h"

HeatSignature::HeatSignature(float heatLevel) : heatLevel(heatLevel) {}

void HeatSignature::setHeatLevel(float heatLevel)
{
    this->heatLevel = heatLevel;
}

std::tuple<int, int, int> HeatSignature::calculatePath(const Object &follower, const Object &target)
{
    auto [fx, fy, fz] = follower.getPosition3D();
    auto [tx, ty, tz] = target.getPosition3D();
    float factor = std::clamp(heatLevel / 100.0f, 0.05f, 0.2f);
    return {fx + static_cast<int>(factor * (tx - fx)), fy + static_cast<int>(factor * (ty - fy)), fz + static_cast<int>(factor * (tz - fz))};
}

GPSAlgorithm::GPSAlgorithm(std::tuple<int, int, int> gpsCoordinates) : gpsCoordinates(gpsCoordinates) {}

void GPSAlgorithm::setGPSData(const std::tuple<int, int, int> &gpsData)
{
    gpsCoordinates = gpsData;
}

std::tuple<int, int, int> GPSAlgorithm::calculatePath(const Object &follower, const Object &target)
{
    // Fallback to target position if GPS not set
    if (gpsCoordinates == std::tuple<int, int, int>{0, 0, 0})
    {
        return target.getPosition3D();
    }

    return gpsCoordinates;
}

std::tuple<int, int, int> DeadReckoning::calculatePath(const Object &follower, const Object &target)
{
    auto [fx, fy, fz] = follower.getPosition3D();
    auto [tx, ty, tz] = target.getPosition3D();
    return {fx + (tx - fx) / 2, fy + (ty - fy) / 2, fz + (tz - fz) / 2};
}

std::tuple<int, int, int> KalmanFilter::calculatePath(const Object &follower, const Object &target)
{
    auto [fx, fy, fz] = follower.getPosition3D();
    auto [tx, ty, tz] = target.getPosition3D();
    float kalmanGain = 0.5; // ***Placeholder value*** to be tuned
    return {fx + static_cast<int>(kalmanGain * (tx - fx)), fy + static_cast<int>(kalmanGain * (ty - fy)), fz + static_cast<int>(kalmanGain * (tz - fz))};
}