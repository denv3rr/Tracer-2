#ifndef SCENARIO_H
#define SCENARIO_H

#include "Object.h"
#include <vector>
#include <string>

// Interface
void runScenarioMode(Object &follower, int gpsTimeout, int heatTimeout);
void runScenarioMainMode(Object &follower, int speed, int iterations);

// Utility
std::vector<Object> generateTargets();
float calculateHeatSignature(const Object &follower, const Object &target);
void logDiagnostics(const Object &follower, const std::vector<Object> &targets);

#endif // SCENARIO_H
