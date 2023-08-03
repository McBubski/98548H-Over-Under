#include "graphics.h"
#include "vex.h"

using namespace vex;

class button {
    public:
        int x;
        int y;
        int sizeX;
        int sizeY;
        bool pressing = false;
        const char * col;
        const char * text;
        
        void initialize(int startX, int startY, int startSizeX, int startSizeY, const char * buttonCol, const char * buttonText) {
            x = startX;
            y = startY;
            sizeX = startSizeX;
            sizeY = startSizeY;
            col = buttonCol;
            text = buttonText;
        }

        void display() {
            Brain.Screen.setFillColor(col);
            Brain.Screen.setPenWidth(3);

            if (pressing) {
                Brain.Screen.setPenColor("#FFBB00");
            } else {
                Brain.Screen.setPenColor(white);
            }
            
            Brain.Screen.drawRectangle(x, y, sizeX, sizeY);
            Brain.Screen.setPenColor(white);

            Brain.Screen.printAt(
                x + (sizeX/2) - (Brain.Screen.getStringWidth(text) / 2), 
                y + (sizeY/2) + (Brain.Screen.getStringHeight(text) / 4), 
                text
            );
        }

        void checkPress() {
            int pressX = Brain.Screen.xPosition();
            int pressY = Brain.Screen.yPosition();

            if (Brain.Screen.pressing()) {
                if (pressX >= x && pressX <= x + sizeX) {
                    if (pressY >= y && pressY <= y + sizeY) {
                        pressing = true;
                    }
                }   
            } else {
                pressing = false;
            }
        }
};

const int numOfButtons = 1;
button buttons[numOfButtons];

button calibrateButton;

int updateScreen() {
    Brain.Screen.render();

    calibrateButton.initialize(130, 15, 120, 40, "#84C1E3", "Calibrate");
    buttons[0] = calibrateButton;

    while (true) {
        Brain.Screen.setFillColor(black);
        Brain.Screen.setPenColor(white);

        // Team Logo

        Brain.Screen.setFont(mono40);
        Brain.Screen.printAt(300, 45, "Revamped");

        Brain.Screen.setFont(mono20);
        Brain.Screen.printAt(350, 20, "98548H");

        // Odometry

        Brain.Screen.printAt(5, 15, "Odometry: ");

        Brain.Screen.setPenColor("#FFBE0F");
        Brain.Screen.printAt(15, 35, "X: ???");
        Brain.Screen.printAt(15, 55, "Y: ???");
        
        // Inertial

        Brain.Screen.setFont(mono20);
        Brain.Screen.setPenColor(white);
        Brain.Screen.printAt(5, 85, "Inertial:");

        Brain.Screen.setFont(mono20);
        if (Inertial.installed()) {
            Brain.Screen.setPenColor(green);
            Brain.Screen.printAt(5, 105, "Ready");
        } else {
            Brain.Screen.setPenColor(red);
            Brain.Screen.printAt(5, 105, "Not Found");
        }

        // Heading

        Brain.Screen.setFont(mono20);
        Brain.Screen.setPenColor(white);
        Brain.Screen.printAt(5, 135, "Heading: ");

        Brain.Screen.setPenColor("#0FFFED");
        Brain.Screen.printAt(15, 155, "%.2f°", absoluteOrientation);

        // Suggestions

        Brain.Screen.setPenColor(white);
        Brain.Screen.printAt(5, 185, "Suggestions:");
        Brain.Screen.setPenColor(green);
        Brain.Screen.printAt(5, 205, "None Found");

        for (int i = 0; i < numOfButtons; i++) {
            buttons[i].display();
            buttons[i].checkPress();
        }

        Brain.Screen.render();

        wait(20, msec);

        Brain.Screen.clearScreen();
    }

    return 0;
}