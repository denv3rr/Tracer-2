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

    std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top

    std::cout << std::left << std::setw(35) << "\033[1;32mFOLLOWER STATUS\033[0m"
              << std::setw(35) << "\033[1;33mMODE STATUS\033[0m"
              << "\033[1;36mSYSTEM LOG\033[0m\n";

    std::cout << std::left << std::setw(35)
              << "Follower Pos : (" + std::to_string(fx) + ", " + std::to_string(fy) + ", " + std::to_string(fz) + ")"
              << std::setw(35)
              << "Current Mode : " + mode
              << "Iteration    : " << iteration << "\n";

    std::cout << std::left << std::setw(35)
              << "Target Pos   : (" + std::to_string(tx) + ", " + std::to_string(ty) + ", " + std::to_string(tz) + ")"
              << std::setw(35)
              << "Mode Changed : " + std::string(modeChanged ? "YES" : "NO")
              << "Distance     : " << std::fixed << std::setprecision(2) << distance << "\n";

    std::cout << std::left << std::setw(35)
              << "Heat Signal  : " + std::to_string(heat)
              << std::setw(35)
              << "Reason       : " + reason
              << "Status       : " << (mode == "stopped" ? "Ended" : "Active") << "\n";

    // Display auto-expanding boundaries
    std::cout << "\n\033[2;36mCURRENT MATRIX BOUNDS:\033[0m\n";
    std::cout << std::left << std::setw(35)
              << "X: [" + std::to_string(travelMinX) + ", " + std::to_string(travelMaxX) + "]"
              << std::setw(35)
              << "Y: [" + std::to_string(travelMinY) + ", " + std::to_string(travelMaxY) + "]"
              << "Z: [" + std::to_string(travelMinZ) + ", " + std::to_string(travelMaxZ) + "]\n";
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
