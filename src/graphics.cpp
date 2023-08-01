#include "vex.h"
#include "graphics.h"

using namespace vex;

int updateScreen() {
    Brain.Screen.render();
    while (true) {
        Brain.Screen.setFillColor(black);

        Brain.Screen.setFont(mono40);
        Brain.Screen.printAt(300, 45, "Revamped");

        Brain.Screen.setFont(mono20);
        Brain.Screen.printAt(350, 20, "98548H");
        
        Brain.Screen.render();

        wait(20, msec);

        Brain.Screen.clearScreen();
    }

    return 0;
}