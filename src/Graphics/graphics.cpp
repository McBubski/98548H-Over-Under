#include "Graphics/graphics.h"
#include "Graphics/Windows/main-window.h"

#include "Autonomous/odometry.h"
#include "Autonomous/autonomous.h"
#include "Autonomous/auton-functions.h"

#include "vex.h"
#include <functional>

using namespace vex;
using ButtonCallback = std::function<void()>;

class Button {
    public:
        Button(int x_, int y_, int width_, int height_, const char* label_, const char* color_) {
            x = x_;
            y = y_;
            width = width_;
            height = height_;
            label = label_;
            color = color_;
        }

        void setOnClick(ButtonCallback onClick_) {
            onClick = onClick_;
        }

        void checkClick(int touchX, int touchY) {
            if (touchX >= x && touchX <= x + width) {
                if (touchY >= y && touchY <= y + height) {
                    if (onClick) {
                        onClick();
                    }
                }
            }
        }

        void display() {
            Brain.Screen.setPenColor(black);
            Brain.Screen.setFillColor(color);
            Brain.Screen.drawRectangle(x, y, width, height);

            Brain.Screen.setPenColor(white);
            Brain.Screen.printAt(
                x + (width/2) - (Brain.Screen.getStringWidth(label)/2),
                y + (height/2) + (Brain.Screen.getStringHeight(label)/4),
                label
            );
        }
    private:
        int x, y;
        int width, height;
        const char* label;
        const char* color;
        ButtonCallback onClick;
};

bool screenDebounce = false;
char * screenWindow = "Main";

void drawText(const char* text, int x, int y, fontType font, const char* fillColor, const char* textColor) {
    Brain.Screen.setFillColor(fillColor);
    Brain.Screen.setPenColor(textColor);
    Brain.Screen.setFont(font);
    Brain.Screen.printAt(x, y, text);
}

void ReturnToMainWindow() {
    if (screenWindow == "Odometry" || screenWindow == "Motors" || screenWindow == "RobotInfo") {
        screenWindow = "Main";
    }
}

int updateScreen() {
    Brain.Screen.render();

    Button returnToMainWindowButton(5, 10, 80, 30, "Return", "#ff0f0f"); // Return Button, Shown On All Menus (excluding Main)
    returnToMainWindowButton.setOnClick(ReturnToMainWindow);

    while (true) {
        if (screenWindow == "Main") {
            drawMainWindow();

        } else if (screenWindow == "Odometry") {
            float robotSize = 10;

            Brain.Screen.setFillColor("#c4c4c4");
            Brain.Screen.drawRectangle(262, 22, 194, 194);
            Brain.Screen.drawImageFromFile("field.png", 260, 20);

            float XOnBrainScreen = 360 + (1.39 * globalXPos);
            float YOnbrainScreen = 120 + (-1.39 * globalYPos);

            Brain.Screen.setFillColor(red);
            Brain.Screen.setPenWidth(0);
            Brain.Screen.drawCircle(360 + 1.39 * targetX, 120 -1.39 * targetY, 4);

            float lineOffset1 = sqrt(2) * robotSize * cos(-absoluteOrientation + M_PI_4);
            float lineOffset2 = sqrt(2) * robotSize * cos(-absoluteOrientation - M_PI_4);

            float headingX = cos(absoluteOrientation - M_PI_2);
            float headingY = sin(absoluteOrientation - M_PI_2);

            Brain.Screen.setPenColor(cyan);
            Brain.Screen.setPenWidth(robotSize * 2 - 2);
            Brain.Screen.drawLine(XOnBrainScreen - (headingX * 8), YOnbrainScreen - (headingY * 8), XOnBrainScreen + (headingX * 8), YOnbrainScreen + (headingY * 8));

            Brain.Screen.setPenColor(black);
            Brain.Screen.setPenWidth(4);

            Brain.Screen.drawLine(XOnBrainScreen + lineOffset1, YOnbrainScreen - lineOffset2, XOnBrainScreen + lineOffset2, YOnbrainScreen + lineOffset1);
            Brain.Screen.drawLine(XOnBrainScreen + lineOffset2, YOnbrainScreen + lineOffset1, XOnBrainScreen - lineOffset1, YOnbrainScreen + lineOffset2);
            Brain.Screen.drawLine(XOnBrainScreen - lineOffset1, YOnbrainScreen + lineOffset2, XOnBrainScreen - lineOffset2, YOnbrainScreen - lineOffset1);
            Brain.Screen.drawLine(XOnBrainScreen - lineOffset2, YOnbrainScreen - lineOffset1, XOnBrainScreen + lineOffset1, YOnbrainScreen - lineOffset2);

            Brain.Screen.setFont(mono20);

            Brain.Screen.setFillColor(black);
            Brain.Screen.setPenColor(white);
            Brain.Screen.printAt(5, 60, "Position: ");

            Brain.Screen.setPenColor("#FFBE0F");
            Brain.Screen.printAt(15, 80, "X: %.1f", globalXPos);
            Brain.Screen.printAt(15, 100, "Y: %.1f", globalYPos);

            Brain.Screen.setFillColor(black);
            Brain.Screen.setPenColor(white);
            Brain.Screen.printAt(5, 120, "Target: ");

            Brain.Screen.setPenColor("#3099E9");
            Brain.Screen.printAt(15, 140, "X: %.1f", targetX);
            Brain.Screen.printAt(15, 160, "Y: %.1f", targetY);

            Brain.Screen.setFillColor(black);
            Brain.Screen.setPenColor(white);
            Brain.Screen.printAt(5, 180, "Error: ");

            Brain.Screen.setPenColor("#EE2A2A");
            Brain.Screen.printAt(15, 200, "X: %.1f", targetX - globalXPos);
            Brain.Screen.printAt(15, 220, "Y: %.1f", targetY - globalYPos);

            Brain.Screen.setPenColor("#3099E9");
            Brain.Screen.printAt(15, 240, "T Head.: %.1f", targetOrientation * (180/M_PI));

            
            Brain.Screen.setPenColor(red);
            Brain.Screen.setPenWidth(4);
            Brain.Screen.drawLine(XOnBrainScreen, YOnbrainScreen, XOnBrainScreen + (headingX * 15), YOnbrainScreen + (headingY * 15));

            returnToMainWindowButton.display();
        } else if (screenWindow == "Motors") {
            Brain.Screen.setFillColor(black);
            Brain.Screen.setPenColor(white);
            
            returnToMainWindowButton.display();
        } else if (screenWindow == "RobotInfo") {
            Brain.Screen.setFillColor(black);
            Brain.Screen.setPenColor(white);

            Brain.Screen.printAt(10, 60, "MicroSD Card: ");

            if (Brain.SDcard.isInserted()) {
                Brain.Screen.setPenColor(green);
                Brain.Screen.printAt(20, 80, "Installed");
            } else {
                Brain.Screen.setPenColor(red);
                Brain.Screen.printAt(20, 80, "Not Found");
            }

            Brain.Screen.setPenColor(white);
            Brain.Screen.printAt(10, 120, "Battery: ");

            int batteryCapacity = Brain.Battery.capacity(percent);
            if (batteryCapacity > 50) {
                Brain.Screen.setPenColor(green);
            } else if (batteryCapacity > 20 && batteryCapacity < 50) {
                Brain.Screen.setPenColor(yellow);
            } else if (batteryCapacity < 20) {
                Brain.Screen.setPenColor(red);
            }
            
            Brain.Screen.printAt(20, 140, "%d%c", batteryCapacity, '%');

            Brain.Screen.setPenColor(white);
            Brain.Screen.printAt(10, 180, "Voltage: ");
            Brain.Screen.setPenColor(green);
            Brain.Screen.printAt(20, 200, "%.1f%c", Brain.Battery.voltage(volt), 'V');

            returnToMainWindowButton.display();
        } else {
            Brain.Screen.setFillColor(black);
            Brain.Screen.printAt(5, 15, "Woah! Something very bad happened.");
            Brain.Screen.printAt(5, 35, "If you read this, please reset robot.");
        }
        
        Brain.Screen.render();

        wait(20, msec);

        Brain.Screen.clearScreen();
    }

    return 0;
}