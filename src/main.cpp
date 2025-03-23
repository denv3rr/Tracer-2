#include "../include/main.h"

int main()
{
    std::cout << "\n\033[1;32mTracer-2\033[0m\n";
    Object follower(2, "Follower", {0, 0, 0});
    showMainMenu(follower);
    return 0;
}
