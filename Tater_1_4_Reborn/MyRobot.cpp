#include "WPILib.h"
#include "Shooter.h"
#include "RobotMap.h"
/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 
class RobotDemo : public IterativeRobot
{
	RobotDrive myRobot; 					// robot drive system
	Joystick lStick, rStick, pickStick; 	// only joystick
	Solenoid forkDown, forkUp;
	Compressor pump;
	Shooter shoot;


	//stuff to see if it worked

public:
	RobotDemo(): 									// list initialization
		myRobot(LEFT_MOTOR_PMW, LEFT_MOTOR_PMW),	// these must be initialized in the same order
		lStick(LTANK_JOY_USB),						// as they are declared above.
		rStick(RTANK_JOY_USB),
		pickStick(SHOOTER_JOY_USB),
		forkDown(1, FORK_DN_SOL),
		forkUp(1, FORK_UP_SOL),
	    pump(1, PRESS_SW_DIO, 1, COMPRESSOR_RLY),
	    shoot()
	  
	{
		myRobot.SetExpiration(0.1);
		this->SetPeriod(0); 	//Set update period to sync with robot control packets (20ms nominal)
	}							//When set to zero, periodic tasks are called based on when DS packets come in
	
/**
 * Robot-wide initialization code should go here.
 * 
 * Use this method for default Robot-wide initialization which will
 * be called when the robot is first powered on.  It will be called exactly 1 time.
 */
void RobotDemo::RobotInit() {
	printf("Robot Init");
	shoot.Init(&rStick); // tell the shooter object which stick controls the shooter
}

/**
 * Initialization code for disabled mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters disabled mode. 
 */
void RobotDemo::DisabledInit() {
	printf("Disabled Init");
}

/**
 * Periodic code for disabled mode should go here.
 * 
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in disabled mode.
 */
void RobotDemo::DisabledPeriodic() {
	//static int count;
	//printf("Disabled Periodic %i\n", count++);
	//count--;
}

/**
 * Initialization code for autonomous mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters autonomous mode.
 */
void RobotDemo::AutonomousInit() {
	myRobot.TankDrive((float)0, 0.0, true);//make sure the robot is stopped
	pump.Start();//turn on compressor
	printf("Autonomous Init");
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
		printf("Running \n");
	    }
	else {
		printf("Restarting \n");
    	}   
	forkUp.Set(true);						//keep forks up
	myRobot.SetSafetyEnabled(false);
	Wait(1.0);
	myRobot.TankDrive(-0.85, -0.85, false);
	Wait(1.5);
	myRobot.TankDrive(0.0, 0.0, true);
	shoot.HighShot();
	printf("Finished \n");
	Wait(10);
	firstTime = false;
	forkUp.Set(false);
	
}

/**
 * Initialization code for teleop mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters teleop mode.
 */
void RobotDemo::TeleopInit() {
	printf("Teleop Init");
}

/**
 * Periodic code for teleop mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in teleop mode.
 */
void RobotDemo::TeleopPeriodic() {

	shoot.Run();						// check for button pushes and manange shots
	myRobot.TankDrive(lStick.GetY(), rStick.GetY(), true);
	if (pickStick.GetY() < -0.1){
		forkDown.Set(true);
		}
	else {
		forkDown.Set(false);
		}
	
	if (pickStick.GetY() > 0.1){
			forkUp.Set(true);
		}
	else {
		
		forkUp.Set(false);
		}
	
	
	
}

/**
 * Initialization code for test mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters test mode.
 */
void RobotDemo::TestInit() {
	printf("Test Init");
}

/**
 * Periodic code for test mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in test mode.
 */
void RobotDemo::TestPeriodic() {
	
	printf("Test Periodic");
}

};

START_ROBOT_CLASS(RobotDemo);

