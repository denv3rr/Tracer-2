#ifndef SCENARIO_H
#define SCENARIO_H

#include "Object.h"
#include <vector>
#include <string>

// Live travel boundaries (auto-expanding)
extern int travelMinX, travelMaxX;
extern int travelMinY, travelMaxY;
extern int travelMinZ, travelMaxZ;

// Interface
void runScenarioMainMode(Object &follower, int speed, int iterations);

// Utility
std::vector<Object> generateTargets();
float calculateHeatSignature(const Object &follower, const Object &target);
void logDiagnostics(const Object &follower, const std::vector<Object> &targets);
void updateTravelBounds(const Object &follower, const Object &target);

#endif // SCENARIO_H