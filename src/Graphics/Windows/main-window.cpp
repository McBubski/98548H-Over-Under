#include "Graphics/graphics.h"
#include "Autonomous/odometry.h"
#include "Autonomous/autonomous.h"
#include "vex.h"
#include <functional>

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

Button calibrateInertialButton(125, 15, 140, 30, "Calibrate", "#03a1fc"); // Inertial Calibration Button
Button odometryWindowButton(125, 55, 140, 30, "Odometry", "#fcb603"); // Button To Switch To Odometry Window
Button motorWindowButton(125, 95, 140, 30, "Motors", "#1cc94d"); // Button To Switch To Motor Window
Button robotInfoWindowButton(125, 135, 140, 30, "Robot Info", "#cf5df5"); // Button To Switch To Robot Info Window
Button returnToMainWindowButton(5, 10, 80, 30, "Return", "#ff0f0f"); // Return Button, Shown On All Menus (excluding Main)
Button autonLeft(275, 95, 20, 30, "<", "#E93030");
Button autonRight(453, 95, 20, 30, ">", "#E93030");

void CalibrateInertial() {
    Inertial.calibrate();
}
void SwitchToOdometryWindow() {
    if (screenWindow == "Main") {
        screenWindow = "Odometry";
    }
}
void SwitchToMotorWindow() {
    if (screenWindow == "Main") {
        screenWindow = "Motors";
    }
}
void SwitchToRobotInfoWindow() {
    if (screenWindow == "Main") {
        screenWindow = "RobotInfo";
    }
}
void ReturnToMainWindow() {
    if (screenWindow == "Odometry" || screenWindow == "Motors" || screenWindow == "RobotInfo") {
        screenWindow = "Main";
    }
}
void switchAutonBackward() {
    autonPath--;
    if (autonPath < 1) {
        autonPath = 3;
    }
}
void switchAutonForward() {
  autonPath++;
    if (autonPath > 3) {
        autonPath = 1;
    }  
}

void drawMainWindow() {
    odometryWindowButton.setOnClick(SwitchToOdometryWindow);
    motorWindowButton.setOnClick(SwitchToMotorWindow);
    robotInfoWindowButton.setOnClick(SwitchToRobotInfoWindow);
    returnToMainWindowButton.setOnClick(ReturnToMainWindow);
    autonLeft.setOnClick(switchAutonBackward);
    autonRight.setOnClick(switchAutonForward);

    // Team Logo

            drawText("Revamped", 290, 55, mono40, "#000000", "#ffffff");
            drawText("98548H", 340, 30, mono20, "#000000", "#ffffff");

            Brain.Screen.drawLine(290, 60, 460, 60);

            // Odometry

            Brain.Screen.printAt(5, 15, "Odometry: ");

            Brain.Screen.setPenColor("#FFBE0F");
            Brain.Screen.printAt(15, 35, "X: %.1f", globalXPos);
            Brain.Screen.printAt(15, 55, "Y: %.1f", globalYPos);

            // Inertial

            Brain.Screen.setFont(mono20);
            Brain.Screen.setPenColor(white);
            Brain.Screen.printAt(5, 85, "Inertial:");

            Brain.Screen.setFont(mono20);
            if (Inertial.installed()) {
                if (Inertial.isCalibrating()) {
                    Brain.Screen.setPenColor(yellow);
                    Brain.Screen.printAt(15, 105, "Clbrtng...");
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
            Brain.Screen.printAt(15, 155, "%.1f°", (absoluteOrientation * 180 / M_PI));

            // Suggestions

            Brain.Screen.setPenColor(white);
            Brain.Screen.printAt(5, 185, "Suggestions:");
            Brain.Screen.setPenColor(green);
            Brain.Screen.printAt(15, 205, "None Found");

            calibrateInertialButton.display();
            odometryWindowButton.display();
            motorWindowButton.display();
            robotInfoWindowButton.display();
            autonLeft.display();
            autonRight.display();

            // Auton Selector

            Brain.Screen.setFillColor(black);
            Brain.Screen.setPenColor(white);

            Brain.Screen.setFont(mono20);
            
            Brain.Screen.printAt(320, 90, "Autonomous:");

            Brain.Screen.setFont(mono30);
            if (autonPath == 1) {
                Brain.Screen.printAt(305, 117, "Home Side");
            } else if (autonPath == 2) {
                Brain.Screen.printAt(305, 117, "Away Side");
            } else if (autonPath == 3) {
                Brain.Screen.printAt(330, 117, "Skills");
            }
            Brain.Screen.drawLine(305, 122, 443, 122);

            if (Brain.Screen.pressing()) {
            if (screenDebounce == false) {
                screenDebounce = true;
                int x = Brain.Screen.xPosition();
                int y = Brain.Screen.yPosition();

                calibrateInertialButton.checkClick(x, y);
                odometryWindowButton.checkClick(x, y);
                motorWindowButton.checkClick(x, y);
                robotInfoWindowButton.checkClick(x, y);
                returnToMainWindowButton.checkClick(x, y);

                autonLeft.checkClick(x, y);
                autonRight.checkClick(x, y);
            }
        } else {
            screenDebounce = false;
        }
}