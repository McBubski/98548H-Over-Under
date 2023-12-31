using namespace vex;

extern brain Brain;
extern controller Controller;
extern inertial Inertial;

extern motor driveFL;
extern motor driveTL;
extern motor driveBL;
extern motor_group leftDrive;

extern motor driveFR;
extern motor driveTR;
extern motor driveBR;
extern motor_group rightDrive;

extern motor liftLeft;
extern motor liftRight;
extern motor_group lift;

extern motor puncher;
extern motor triball_arm;

extern smartdrive drive;

extern rotation ForwardTrackingWheel;
extern rotation SidewaysTrackingWheel;
extern rotation LiftPotentiometer;

extern digital_out wingPneumatics;
extern digital_out ballArm;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
