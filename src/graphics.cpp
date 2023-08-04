#include "graphics.h"
#include "vex.h"

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

void CalibrateInertial() {
    Inertial.calibrate();
}

void OdometryWindow() {
    screenWindow = "Odometry";
}

void MotorWindow() {
    screenWindow = "Motors";
}

void ReturnToMainWindow() {
    screenWindow = "Main";
}

int updateScreen() {
    Brain.Screen.render();

    Button calibrateInertialButton(125, 15, 140, 30, "Calibrate", "#03a1fc"); // Inertial Calibration Button
    calibrateInertialButton.setOnClick(CalibrateInertial);

    Button odometryWindowButton(125, 55, 140, 30, "Odometry", "#fcb603"); // Button To Switch To Odometry Menu
    odometryWindowButton.setOnClick(OdometryWindow);

    Button motorWindowButton(125, 95, 140, 30, "Motors", "#1cc94d");
    motorWindowButton.setOnClick(MotorWindow);

    Button returnToMainWindowButton(5, 10, 80, 30, "Return", "#ff0f0f"); // Return Button, Shown On All Menus (excluding Main)
    returnToMainWindowButton.setOnClick(ReturnToMainWindow);

    while (true) {
        if (screenWindow == "Main") {
            Brain.Screen.setFillColor(black);
            Brain.Screen.setPenColor(white);

            // Team Logo

            Brain.Screen.setFont(mono40);
            Brain.Screen.printAt(300, 45, "Revamped");

            Brain.Screen.setFont(mono20);
            Brain.Screen.printAt(350, 20, "98548H");

            Brain.Screen.drawLine(300, 50, 460, 50);

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
                if (Inertial.isCalibrating()) {
                    Brain.Screen.setPenColor(yellow);
                    Brain.Screen.printAt(15, 105, "Calibrating...");
                } else {
                    Brain.Screen.setPenColor(green);
                    Brain.Screen.printAt(15, 105, "Ready");
                }
            } else {
                Brain.Screen.setPenColor(red);
                Brain.Screen.printAt(15, 105, "Not Found");
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
            Brain.Screen.printAt(15, 205, "None Found");

            calibrateInertialButton.display();
            odometryWindowButton.display();
            motorWindowButton.display();

        } else if (screenWindow == "Odometry") {
            Brain.Screen.setFillColor(black);
            Brain.Screen.printAt(10, 100, "Pretend a cool odometry menu is here :)");
            
            returnToMainWindowButton.display();
        } else if (screenWindow == "Motors") {
            Brain.Screen.setFillColor(black);
            Brain.Screen.printAt(10, 100, "Pretend a menu for motors is here 0_0");
            
            returnToMainWindowButton.display(); 
        }

        if (Brain.Screen.pressing()) {
            if (screenDebounce == false) {
                screenDebounce = true;
                int x = Brain.Screen.xPosition();
                int y = Brain.Screen.yPosition();

                if (screenWindow == "Main") {
                    calibrateInertialButton.checkClick(x, y);
                    odometryWindowButton.checkClick(x, y);
                    motorWindowButton.checkClick(x, y);
                    continue;
                } else if (screenWindow = "Odometry") {
                    returnToMainWindowButton.checkClick(x, y);
                    continue;
                } else if (screenWindow = "Motors") {
                    returnToMainWindowButton.checkClick(x, y);
                    continue;
                }
            }
        } else {
            screenDebounce = false;
        }
        
        Brain.Screen.render();

        wait(20, msec);

        Brain.Screen.clearScreen();
    }

    return 0;
}