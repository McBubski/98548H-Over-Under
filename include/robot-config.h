using namespace vex;

extern brain Brain;
extern controller Controller;
extern inertial Inertial;

extern motor driveFL;
extern motor driveBL;

extern motor_group leftDrive;

extern motor driveFR;
extern motor driveBR;

extern motor_group rightDrive;

extern motor liftLeft;
extern motor liftRight;

extern motor_group lift;

extern rotation ForwardTrackingWheel;
extern rotation SideTrackingWheel;

extern digital_out wingPneumatics;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
