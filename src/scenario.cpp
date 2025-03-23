#include "../include/scenario.h"
#include "../include/Tracker.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

void printStaticModeColumns(const std::string &mode, int iteration, bool modeChanged, const std::string &reason)
{
    // Fixed output columns (center and right)
    std::cout << "\033[2;1H"; // Top-left (row 2, col 1)
    std::cout << "\033[1;33m+==== MODE STATUS ====+\033[0m\n";
    std::cout << "Mode         : " << mode << "\n";
    std::cout << "Mode Changed : " << (modeChanged ? "YES" : "NO") << "\n";
    std::cout << "Reason       : " << reason << "\n";

    std::cout << "\n\033[1;36m+==== SYSTEM LOG ====+\033[0m\n";
    std::cout << "Iteration    : " << iteration << "\n";
    std::cout << "Tracking     : " << (mode == "stopped" ? "No" : "Active") << "\n";
}

void printFollowerStatus(const Object &follower, const Object &target, float heatLevel, float distance)
{
    std::cout << "\033[0;0H"; // Move cursor to top-left corner
    std::cout << "\033[2J";   // Clear screen

    auto [fx, fy, fz] = follower.getPosition3D();
    auto [tx, ty, tz] = target.getPosition3D();

    std::cout << "\033[1;32m+==== FOLLOWER STATUS ====+\033[0m\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Follower Pos : (" << fx << ", " << fy << ", " << fz << ")\n";
    std::cout << "Target Pos   : (" << tx << ", " << ty << ", " << tz << ")\n";
    std::cout << "Distance     : " << distance << "\n";
    std::cout << "Heat Signal  : " << heatLevel << "\n";
}

void runScenarioMainMode(Object &follower, int speed, int iterations)
{
    Object target(1, "Target", {30, 30, 0});
    Tracker tracker(follower);
    tracker.setTarget(target);
    tracker.setTrackingMode("heat");

    int count = 0;
    std::string currentMode = "heat";
    bool modeChanged = false;

    while (tracker.isTrackingActive() && (iterations == 0 || count < iterations))
    {
        tracker.update();

        float distance = tracker.computeDistance(); // Assuming public access (optional promotion)
        float heat = 250.0f / (1.0f + distance);
        std::string reason;

        // If switched mode, update center
        if (tracker.isTrackingActive() && currentMode != tracker.getCurrentMode())
        {
            modeChanged = true;
            reason = "low heat signal";
            currentMode = tracker.getCurrentMode();
        }
        else
        {
            modeChanged = false;
        }

        printFollowerStatus(follower, target, heat, distance);
        printStaticModeColumns(currentMode, count, modeChanged, reason);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / speed));
        ++count;
    }

    std::cout << "\n\033[1;31mSimulation complete.\033[0m\n";
}
