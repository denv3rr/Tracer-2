#include "../include/scenario.h"
#include "../include/Tracker.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <random> // For test target placement and movement

// Travel coordinates
int travelMinX = 0, travelMaxX = 0;
int travelMinY = 0, travelMaxY = 0;
int travelMinZ = 0, travelMaxZ = 0;

void updateTravelBounds(const Object &follower, const Object &target)
{
    auto [fx, fy, fz] = follower.getPosition3D();
    auto [tx, ty, tz] = target.getPosition3D();

    travelMinX = std::min({travelMinX, fx, tx});
    travelMaxX = std::max({travelMaxX, fx, tx});
    travelMinY = std::min({travelMinY, fy, ty});
    travelMaxY = std::max({travelMaxY, fy, ty});
    travelMinZ = std::min({travelMinZ, fz, tz});
    travelMaxZ = std::max({travelMaxZ, fz, tz});
}

void printHorizontalUI(const Object &follower, const Object &target, const std::string &mode,
                       bool modeChanged, const std::string &reason, int iteration, float heat, float distance)
{
    auto [fx, fy, fz] = follower.getPosition3D();
    auto [tx, ty, tz] = target.getPosition3D();

    std::string heatColor;
    if (heat > 200)
        heatColor = "\033[1;31m"; // 🔴 Red - at close proximity
    else if (heat > 100)
        heatColor = "\033[1;33m"; // 🟡 Yellow - mid proximity
    else
        heatColor = "\033[1;34m"; // 🔵 Blue - far but detectable

    std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top

    // Manual header alignment
    std::cout << std::left
              << "FOLLOWER STATUS              "
              << "MODE STATUS                  "
              << "SYSTEM LOG\n";

    std::ostringstream left, center, right;

    left << std::fixed << std::setprecision(2)
         << "Follower Pos : (" << fx << ", " << fy << ", " << fz << ")\n"
         << "Target Pos   : (" << tx << ", " << ty << ", " << tz << ")\n"
         << heatColor << "Heat Signal  : " << heat << "\033[0m\n";

    center << "Current Mode : " << mode << "\n"
           << "Mode Changed : " << (modeChanged ? "YES" : "NO") << "\n"
           << "Reason       : " << reason << "\n";

    right << "Iteration    : " << iteration << "\n"
          << "Distance     : " << distance << "\n"
          << "Status       : " << std::string(mode == "stopped" ? "Ended" : "Active") << "\n";

    // Print line by line
    std::istringstream l(left.str()), c(center.str()), r(right.str());
    std::string lLine, cLine, rLine;

    while (std::getline(l, lLine) && std::getline(c, cLine) && std::getline(r, rLine))
    {
        std::cout << std::left
                  << std::setw(32) << lLine
                  << std::setw(32) << cLine
                  << rLine << "\n";
    }

    std::cout << "\n\033[2;36mCURRENT MATRIX BOUNDS:\033[0m\n";
    std::cout << "X: [" << travelMinX << ", " << travelMaxX << "]   "
              << "Y: [" << travelMinY << ", " << travelMaxY << "]   "
              << "Z: [" << travelMinZ << ", " << travelMaxZ << "]\n";
}

void runScenarioMainMode(Object &follower, int speed, int iterations)
{
    Object target(1, "Target", {30, 30, 10});
    Tracker tracker(follower);
    tracker.setTarget(target);
    tracker.setTrackingMode("heat");

    int count = 0;
    std::string currentMode = tracker.getCurrentMode();
    bool modeChanged = false;
    std::string reason;

    std::default_random_engine rng(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> moveDelta(-1, 1); // Random small shifts

    while (tracker.isTrackingActive() && (iterations == 0 || count < iterations))
    {
        // FOR TESTING -- Simulate random movement of a target
        auto [tx, ty, tz] = target.getPosition3D();
        tx += moveDelta(rng);
        ty += moveDelta(rng);
        tz += moveDelta(rng);
        target.moveTo({tx, ty, tz});

        // Perform tracking update
        tracker.setTarget(target); // Update latest target position
        tracker.update();

        // Data
        float distance = tracker.computeDistance();
        float heat = 250.0f / (1.0f + distance);

        // Update bounds
        updateTravelBounds(follower, target);

        // Check if mode changed
        if (tracker.getCurrentMode() != currentMode)
        {
            modeChanged = true;
            reason = "Switched due to low heat";
            currentMode = tracker.getCurrentMode();
        }
        else
        {
            modeChanged = false;
            reason = "OK";
        }

        // Print current horizontal UI output
        printHorizontalUI(follower, target, currentMode, modeChanged, reason, count, heat, distance);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / speed));
        ++count;
    }

    std::cout << "\n\033[1;31mTarget reached.\033[0m\n";
}
