using namespace vex;

extern brain Brain;
extern controller Controller;
extern inertial Inertial;

extern motor driveFL;
extern motor driveML;
extern motor driveBL;

extern motor_group leftDrive;

extern motor driveFR;
extern motor driveMR;
extern motor driveBR;

extern motor_group rightDrive;

extern motor flyWheelTop;
extern motor flyWheelBottom;

extern motor intakeLeft;
extern motor intakeRight;

extern rotation ForwardTrackingWheel;
extern rotation SideTrackingWheel;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
