#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 
class RobotDemo : public IterativeRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick lStick, rStick; // only joystick
	Solenoid pneumChuck;
	Compressor thing;

public:
	RobotDemo():
		myRobot(1, 10),	// these must be initialized in the same order
		lStick(1),		// as they are declared above.
		rStick(2),
		pneumChuck(1, 3),
	    thing(1, 1, 1, 1)
	{
		myRobot.SetExpiration(0.1);
		this->SetPeriod(0); 	//Set update period to sync with robot control packets (20ms nominal)
	}
	
/**
 * Robot-wide initialization code should go here.
 * 
 * Use this method for default Robot-wide initialization which will
 * be called when the robot is first powered on.  It will be called exactly 1 time.
 */
void RobotDemo::RobotInit() {
}

/**
 * Initialization code for disabled mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters disabled mode. 
 */
void RobotDemo::DisabledInit() {
}

/**
 * Periodic code for disabled mode should go here.
 * 
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in disabled mode.
 */
void RobotDemo::DisabledPeriodic() {
}

/**
 * Initialization code for autonomous mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters autonomous mode.
 */
void RobotDemo::AutonomousInit() {
	myRobot.TankDrive((float)0, 0.0, true);
	pneumChuck.Set(false);
	thing.Start();
}

/**
 * Periodic code for autonomous mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in autonomous mode.
 * 
 */
void RobotDemo::AutonomousPeriodic() {
	static bool firstTime = true;
	if (firstTime == true) {
		printf("Doing it for the first time \n");
	    }
	else {
		printf("Restarting \n");
    	}   
	//add holding the arms up in autonomous
	myRobot.SetSafetyEnabled(false);
	Wait(1.0);
	myRobot.TankDrive(-0.85, -0.85, false);
	Wait(1.5);
	myRobot.TankDrive(0.0, 0.0, true);
	pneumChuck.Set(true);
	Wait(0.3);
	pneumChuck.Set(false);
	printf("I'm Done!!!!!!1 Aren't you happy?!?!?\n");
	Wait(10);
	firstTime = false;
	
}

/**
 * Initialization code for teleop mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters teleop mode.
 */
void RobotDemo::TeleopInit() {
}

/**
 * Periodic code for teleop mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in teleop mode.
 */
void RobotDemo::TeleopPeriodic() {
//	myRobot.ArcadeDrive(stick); // drive with arcade style 
	myRobot.TankDrive(lStick.GetY(), rStick.GetY(), true);
}

/**
 * Initialization code for test mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters test mode.
 */
void RobotDemo::TestInit() {
}

/**
 * Periodic code for test mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in test mode.
 */
void RobotDemo::TestPeriodic() {
}

};

START_ROBOT_CLASS(RobotDemo);

