#ifndef TRACKER_H
#define TRACKER_H

#include "Object.h"
#include "PathAlgorithms.h"
#include <memory>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

class Tracker
{
public:
    explicit Tracker(Object &follower);
    void setTarget(const Object &target);
    void setTrackingMode(const std::string &mode);
    void startTracking(int iterations, int speed);
    void update();
    void updateHeatSignature(float heatValue);
    bool isTrackingActive() const;

    // Helpers
    float computeDistance() const;
    std::string getCurrentMode() const;

private:
    Object &follower;
    const Object *target;
    std::unique_ptr<PathCalculator> pathCalculator;
    std::string trackingMode;
    bool active;

    // Heat-specific data
    float heatSignatureLevel;
    int modeSwitchCounter;

    // Internal helpers
    void switchModeIfNecessary();
};

#endif // TRACKER_H