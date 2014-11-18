#include "WPILib.h"
#include "Shooter.h"
#include "RobotMap.h"
//#include "TrapezoidalMove.h"


/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 
class RobotDemo : public IterativeRobot
{
	RobotDrive myRobot; 					// robot drive system
	Joystick lStick, rStick, pickStick; 	// only joystick
	Solenoid forkDown, forkUp, arcReactor;
	Compressor pump;
	Shooter shoot;
	bool m_autoFirst;
	//Encoder lCode, rCode;
	//TrapezoidalMoveProfile autoMove;

public:
	RobotDemo(): 									// list initialization
		myRobot(LEFT_MOTOR_PWM, RIGHT_MOTOR_PWM),	// these must be initialized in the same order
		lStick(LTANK_JOY_USB),						// as they are declared above.
		rStick(RTANK_JOY_USB),
		pickStick(SHOOTER_JOY_USB),
		forkDown(1, FORK_DN_SOL),
		forkUp(1, FORK_UP_SOL),
		arcReactor (1, ARC_RCTR_SOL),
	    pump(1, PRESS_SW_DIO, 1, COMPRESSOR_RLY),
	    shoot()
		//lCode (1, CODE_LT_A, 1, CODE_LT_B, false),
		//rCode (1, CODE_RT_A, 1, CODE_RT_B, true),
		//autoMove(0.1, 0.1, 2.0, 15*12.0)
	  
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
	m_autoFirst = true;
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
	//printf("Running \n");
	//forkUp.Set(true);						//keep forks up
	//myRobot.SetSafetyEnabled(false);
	//Wait(1.0);
	//myRobot.TankDrive(-0.85, -0.85, false);
	//Wait(1.5);
	//myRobot.TankDrive(0.0, 0.0, true);
	//shoot.HighShot();
	//printf("Finished \n");
	//Wait(10);
	//forkUp.Set(false);
	
	//potential two ball auto
	if (m_autoFirst){
		forkUp.Set(true); //hold forks up
		myRobot.SetSafetyEnabled(false);
		myRobot.TankDrive(-0.85, -0.85, false);	//drive forward for first shot
		Wait(0.7);
		myRobot.TankDrive(0.0, 0.0, true);	//stop
		shoot.HighShot();	//shoot
		forkUp.Set(false);	//release forks
		Wait(1.0);
		forkDown.Set(true);	//lower forks
		myRobot.TankDrive(0.85, 0.85, false);	//drive back for next ball
		Wait(0.7);
		myRobot.TankDrive(0.0, 0.0, true);	//stop
		Wait(1.0);
		forkDown.Set(false);	//release forks
		forkUp.Set(true);	//raise forks
		Wait(1.0);
		myRobot.TankDrive(-0.85, -0.85, false);	//drive forward for second shot
		Wait(0.7);
		myRobot.TankDrive(0.0, 0.0, true);	//stop
		shoot.HighShot();	//shoot
		}
	m_autoFirst = false;
}

/**
 * Initialization code for teleop mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters teleop mode.
 */
void RobotDemo::TeleopInit() {
	//lCode.Start();
	//rCode.Start();
	pump.Start();//turn on compressor
	printf("Teleop Init");
}

/**
 * Periodic code for teleop mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in teleop mode.
 */
void RobotDemo::TeleopPeriodic() {
	static bool frontDrive = true;
	static bool arcadeDrive = false;
	static int switchState = 0;
	static int counter = 0;
	shoot.Run();// check for button pushes and manange shots
	
	//switch between arcade/tank drive mode
	if ((lStick.GetRawAxis(3) == -1)&&(rStick.GetRawAxis(3) == 1)&&(lStick.GetRawButton(8))) {
		switch (switchState) {
			case 0:
				counter = 0;
				if (rStick.GetRawButton(6)) {
					switchState = 1;
				}
				break;
			case 1:
				counter++;
				if (!rStick.GetRawButton(6)) {
					if (counter <= 25) {
						switchState = 2;
						counter = 0;
					}
					else {
						switchState = 0;
					}
				}
				break;
			case 2:
				counter++;
				if (rStick.GetRawButton(11)) {
					if (counter <=25) {
						switchState = 3;
						counter = 0;
					}
					else {
						switchState = 0;
					}
				}
				break;
			case 3:
				if (!rStick.GetRawButton(11)) {
					if (counter <= 25) {
						if (arcadeDrive) {
							arcadeDrive = false;
						}
						else {
							arcadeDrive = true;
						}
						
					}
					switchState = 0;
				}
				break;
		}
	}
	else {
		switchState = 0;
	}
	
	if (!arcadeDrive) { //tank drive mode
		//choose front/back
		if (lStick.GetRawButton(3)) {
			frontDrive = true;
			arcReactor.Set(false);	//de-activate arc-reactor
		}
		if (lStick.GetRawButton(2)) {
			frontDrive = false;
			arcReactor.Set(true);	//activate arc-reactor
		}
		
		if (frontDrive) {		//front is "forward"
			if (lStick.GetRawButton(1)) {
				myRobot.TankDrive(-lStick.GetY(), -rStick.GetY(), true);	//with turbo
			}
			else {
				myRobot.TankDrive(-lStick.GetY()*0.85, -rStick.GetY()*0.85, true);	//without turbo
			}
		}
		if (!frontDrive) {		//rear is "forward"
			if (lStick.GetRawButton(1)) {	
				myRobot.TankDrive(rStick.GetY(), lStick.GetY(), true);	//with turbo
			}
			else {
				myRobot.TankDrive(rStick.GetY()*0.85, lStick.GetY()*0.85, true);	//without turbo
			}
		}
		//pickup forks for tank mode
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
	else { //arcade drive mode
		myRobot.ArcadeDrive(-rStick.GetY(),-rStick.GetX(), true);
		
		//pickup forks for arcade mode
		if (lStick.GetRawButton(2)){
			forkDown.Set(true);
		}
		else {
			forkDown.Set(false);
		}

		if (lStick.GetRawButton(3)){
				forkUp.Set(true);
		}
		else {

			forkUp.Set(false);
		}
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

