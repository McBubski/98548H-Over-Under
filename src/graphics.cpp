#include "graphics.h"
#include "odometry.h"
#include "autonomous.h"
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

void switchAuton() {
    
}

int updateScreen() {
    Brain.Screen.render();

    Button calibrateInertialButton(125, 15, 140, 30, "Calibrate", "#03a1fc"); // Inertial Calibration Button
    calibrateInertialButton.setOnClick(CalibrateInertial);

    Button odometryWindowButton(125, 55, 140, 30, "Odometry", "#fcb603"); // Button To Switch To Odometry Window
    odometryWindowButton.setOnClick(SwitchToOdometryWindow);

    Button motorWindowButton(125, 95, 140, 30, "Motors", "#1cc94d"); // Button To Switch To Motor Window
    motorWindowButton.setOnClick(SwitchToMotorWindow);

    Button robotInfoWindowButton(125, 135, 140, 30, "Robot Info", "#cf5df5"); // Button To Switch To Robot Info Window
    robotInfoWindowButton.setOnClick(SwitchToRobotInfoWindow);

    Button returnToMainWindowButton(5, 10, 80, 30, "Return", "#ff0f0f"); // Return Button, Shown On All Menus (excluding Main)
    returnToMainWindowButton.setOnClick(ReturnToMainWindow);


    Button autonLeft(275, 95, 20, 30, "<", "#E93030");
    autonLeft.setOnClick(switchAuton);

    Button autonRight(453, 95, 20, 30, ">", "#E93030");
    autonRight.setOnClick(switchAuton);

    while (true) {
        if (screenWindow == "Main") {
            Brain.Screen.setFillColor(black);
            Brain.Screen.setPenColor(white);

            // Team Logo
            
            Brain.Screen.setFont(mono40);
            Brain.Screen.printAt(290, 55, "Revamped");

            Brain.Screen.setFont(mono20);
            Brain.Screen.printAt(340, 30, "98548H");

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
            Brain.Screen.printAt(305, 117, "Home Side");
            Brain.Screen.drawLine(305, 122, 443, 122);

        } else if (screenWindow == "Odometry") {
            float robotSize = 10;

            Brain.Screen.setFillColor("#c4c4c4");
            Brain.Screen.drawRectangle(262, 22, 194, 194);
            Brain.Screen.drawImageFromFile("field.png", 260, 20);

            float XOnBrainScreen = 360 + (1.39 * globalXPos);
            float YOnbrainScreen = 120 + (-1.39 * globalYPos);

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

            Brain.Screen.setPenColor(red);
            Brain.Screen.setPenWidth(4);
            Brain.Screen.drawLine(XOnBrainScreen, YOnbrainScreen, XOnBrainScreen + (headingX * 15), YOnbrainScreen + (headingY * 15));

            Brain.Screen.setFillColor(black);
            Brain.Screen.setPenColor(white);
            Brain.Screen.printAt(5, 60, "Position: ");

            Brain.Screen.setPenColor("#FFBE0F");
            Brain.Screen.printAt(15, 80, "X: %.1f", globalXPos);
            Brain.Screen.printAt(15, 100, "Y: %.1f", globalYPos);

            Brain.Screen.setFont(mono20);
            Brain.Screen.setPenColor(white);
            Brain.Screen.printAt(5, 120, "Heading: ");

            Brain.Screen.setPenColor("#0FFFED");
            Brain.Screen.printAt(15, 140, "%.1f°", (absoluteOrientation * 180 / M_PI));

            Brain.Screen.printAt(15, 160, "F: %.1f", ForwardTrackingWheel.position(degrees));

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
        
        Brain.Screen.render();

        wait(20, msec);

        Brain.Screen.clearScreen();
    }

    return 0;
}