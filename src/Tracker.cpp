#include "../include/Tracker.h"
#include <cmath>
#include <iostream>

constexpr int MODE_SWITCH_THRESHOLD = 25;
constexpr int MODE_SWITCH_DELAY = 3;

Tracker::Tracker(Object &follower)
    : follower(follower), target(nullptr), active(true),
      heatSignatureLevel(0.0f), modeSwitchCounter(0) {}

void Tracker::setTarget(const Object &targetObj)
{
    target = &targetObj;
}

void Tracker::setTrackingMode(const std::string &mode)
{
    if (mode == "gps")
        pathCalculator = std::make_unique<GPSAlgorithm>();
    else if (mode == "heat")
        pathCalculator = std::make_unique<HeatSignature>();
    else if (mode == "dead_reckoning")
        pathCalculator = std::make_unique<DeadReckoning>();
    else if (mode == "kalman")
        pathCalculator = std::make_unique<KalmanFilter>();
    else
    {
        std::cerr << "[ERROR] Unknown mode: " << mode << "\n";
        active = false;
        return;
    }

    trackingMode = mode;
    std::cout << "[MODE] Tracking mode set to: " << mode << "\n";
}

void Tracker::startTracking(int iterations, int speed)
{
    int step = 0;
    while (isTrackingActive() && (iterations == 0 || step < iterations))
    {
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / speed));
        ++step;
    }
}

void Tracker::update()
{
    if (!target || !pathCalculator)
    {
        std::cerr << "[WARN] No path calculator set!\n";
        return;
    }

    auto newPosition = pathCalculator->calculatePath(follower, *target);
    follower.moveTo(newPosition);

    if (trackingMode == "heat")
    {
        float distance = computeDistance();
        heatSignatureLevel = 250.0f / (1.0f + distance); // Stronger signal closer
        switchModeIfNecessary();
    }

    if (follower.getPosition3D() == target->getPosition3D())
    {
        std::cout << "[TRACKER] Follower has reached the target.\n";
        active = false;
    }
}

void Tracker::updateHeatSignature(float heatValue)
{
    heatSignatureLevel = heatValue;
}

bool Tracker::isTrackingActive() const
{
    return active;
}

float Tracker::computeDistance() const
{
    auto [fx, fy, fz] = follower.getPosition3D();
    auto [tx, ty, tz] = target->getPosition3D();

    float dx = static_cast<float>(tx - fx);
    float dy = static_cast<float>(ty - fy);
    float dz = static_cast<float>(tz - fz);

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

void Tracker::switchModeIfNecessary()
{
    if (heatSignatureLevel < MODE_SWITCH_THRESHOLD)
    {
        modeSwitchCounter++;
        if (modeSwitchCounter >= MODE_SWITCH_DELAY)
        {
            std::cout << "[TRACKER] Weak heat signal — switching to GPS.\n";
            setTrackingMode("gps");
            modeSwitchCounter = 0;
        }
    }
}

std::string Tracker::getCurrentMode() const
{
    return trackingMode;
}