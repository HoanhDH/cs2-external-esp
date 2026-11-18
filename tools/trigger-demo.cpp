#include "../src/gui/frontend/menu/practice/ClickPractice.hpp"
#include <cstdio>

int main() {
    if (click_practice::Run(nullptr)) return 0;
    std::fputs("Could not open the offline click practice window.\n", stderr);
    return 1;
}
