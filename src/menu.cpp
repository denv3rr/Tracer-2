#include "../include/menu.h"
#include "../include/scenario.h"
#include "../include/inputValidation.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>

void showMainMenu(Object &follower)
{
    int choice = 0;

    while (choice != 3)
    {
        std::cout << "\n\033[1;34m=== MAIN MENU ===\033[0m\n";
        std::cout << "1. Run Scenario Mode (Auto)\n";
        std::cout << "2. Run Test Mode (Manual)\n";
        std::cout << "3. Exit\n";
        choice = getValidatedIntInput("Select an option: ", 1, 3);

        switch (choice)
        {
        case 1:
        {
            // ✅ Scenario runs immediately using defaults
            int defaultSpeed = 60;
            int defaultIterations = 0; // Run until complete
            std::cout << "\n\033[1;32m[SCENARIO MODE] Auto-launching with speed: "
                      << defaultSpeed << ", infinite tracking...\033[0m\n";
            runScenarioMainMode(follower, defaultSpeed, defaultIterations);
            break;
        }
        case 2:
        {
            std::cout << "\n\033[1;33m[TEST MODE] Manual configuration...\033[0m\n";
            int speed = getValidatedIntInput("Enter tracking speed (1–100): ", 1, 100);
            int iterations = getValidatedIntInput("Enter number of iterations (0 for infinite): ", 0, 10000);
            runScenarioMainMode(follower, speed, iterations);
            break;
        }
        case 3:
            std::cout << "Exiting Tracer-2...\n";
            break;
        }
    }
}