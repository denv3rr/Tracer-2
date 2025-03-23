#include "../include/scenario.h"
#include "../include/Tracker.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
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
    constexpr int COL_WIDTH = 30;

    std::cout << std::left << std::setw(COL_WIDTH) << "\033[1;32mFOLLOWER STATUS\033[0m"
              << std::setw(COL_WIDTH) << "\033[1;33mMODE STATUS\033[0m"
              << "\033[1;36mSYSTEM LOG\033[0m\n";

    std::cout << std::fixed << std::setprecision(2);

    std::cout << std::left << std::setw(COL_WIDTH)
              << ("Follower Pos : (" + std::to_string(fx) + ", " + std::to_string(fy) + ", " + std::to_string(fz) + ")")
              << std::setw(COL_WIDTH)
              << ("Current Mode : " + mode)
              << ("Iteration    : " + std::to_string(iteration)) << "\n";

    std::cout << std::left << std::setw(COL_WIDTH)
              << ("Target Pos   : (" + std::to_string(tx) + ", " + std::to_string(ty) + ", " + std::to_string(tz) + ")")
              << std::setw(COL_WIDTH)
              << ("Mode Changed : " + std::string(modeChanged ? "YES" : "NO"))
              << ("Distance     : " + std::to_string(distance)) << "\n";

    std::cout << std::left << std::setw(COL_WIDTH)
              << (heatColor + "Heat Signal  : " + std::to_string(heat) + "\033[0m")
              << std::setw(COL_WIDTH)
              << ("Reason       : " + reason)
              << ("Status       : " + std::string(mode == "stopped" ? "Ended" : "Active")) << "\n";

    std::cout << "\n\033[2;36mCURRENT MATRIX BOUNDS:\033[0m\n";
    std::cout << std::left << std::setw(COL_WIDTH)
              << ("X: [" + std::to_string(travelMinX) + ", " + std::to_string(travelMaxX) + "]")
              << std::setw(COL_WIDTH)
              << ("Y: [" + std::to_string(travelMinY) + ", " + std::to_string(travelMaxY) + "]")
              << ("Z: [" + std::to_string(travelMinZ) + ", " + std::to_string(travelMaxZ) + "]") << "\n";
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

    std::cout << "\n\033[1;31mSimulation complete.\033[0m\n";
}
