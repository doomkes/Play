#include "WPILib.h"
#include "Shooter.h"
#include "RobotMap.h"
#include "TrapezoidalMove.h"
#include "TaterDrive.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 
class RobotDemo : public IterativeRobot
{
	//RobotDrive myRobot; 					// robot drive system
	TaterDrive LeftDrive;
	TaterDrive RightDrive;
	Joystick lStick, rStick, pickStick; 	// only joystick
	Solenoid forkDown, forkUp, arcReactor;
	Compressor pump;
	Shooter shoot;
	Encoder lCode, rCode;
	TrapezoidalMoveProfile autoMove;
	PIDController lLoop, rLoop;
	double m_AutoStartTime;
	bool m_autoFirst;

public:
	RobotDemo(): 									// list initialization
	//	myRobot(LEFT_MOTOR_PWM, RIGHT_MOTOR_PWM),	// these must be initialized in the same order
		LeftDrive(LEFT_MOTOR_PWM),
		RightDrive(RIGHT_MOTOR_PWM),
		lStick(LTANK_JOY_USB),						// as they are declared above.
		rStick(RTANK_JOY_USB),
		pickStick(SHOOTER_JOY_USB),
		forkDown(1, FORK_DN_SOL),
		forkUp(1, FORK_UP_SOL),
		arcReactor (1, ARC_RCTR_SOL),
	    pump(1, PRESS_SW_DIO, 1, COMPRESSOR_RLY),
	    shoot(),
		lCode (1, CODE_LT_A, 1, CODE_LT_B, false),
		rCode (1, CODE_RT_A, 1, CODE_RT_B, true),
		autoMove(0.1, 0.1, 2.0, 15*12.0),
		lLoop(0.1, 0.001, 0.0, &lCode, &LeftDrive,0.05),
		rLoop(0.1, 0.001, 0.0, &rCode, &RightDrive,0.05)
	  
	{
		LeftDrive.SetExpiration(0.1);
		RightDrive.SetExpiration(0.1);
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
	LeftDrive.SetMotorOutput(0.0);
	RightDrive.SetMotorOutput(0.0);
	pump.Start();//turn on compressor
	lCode.Reset();
	rCode.Reset();
	lCode.SetDistancePerPulse(INCH_PER_CNT);
	rCode.SetDistancePerPulse(INCH_PER_CNT);
	lCode.Start();
	rCode.Start();
	lLoop.SetSetpoint(0.0);
	rLoop.SetSetpoint(0.0);
	lLoop.Enable(); // Turn on PID control loop
	rLoop.Enable();
	m_AutoStartTime = Timer::GetPPCTimestamp();
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
	
	double currentTime = Timer::GetPPCTimestamp();
	double desiredPos = autoMove.Position(currentTime);
	printf("Setting setpoint to %f inches\n",desiredPos);
	lLoop.SetSetpoint(desiredPos);
	rLoop.SetSetpoint(desiredPos);
}

/**
 * Initialization code for teleop mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters teleop mode.
 */
void RobotDemo::TeleopInit() {
	lCode.Start();	// Start Encoders if not already started
	rCode.Start();
	lLoop.Disable(); // Turn off PID system
	rLoop.Disable();
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
	shoot.Run();// check for button pushes and manange shots
	double lCodeVal = lCode.GetRaw()*INCH_PER_CNT;
	double rCodeVal = rCode.GetRaw()*INCH_PER_CNT;
	printf("Left = %f Right = %f\n",rCodeVal, lCodeVal);

	
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
			//myRobot.TankDrive(-lStick.GetY(), -rStick.GetY(), true);	//with turbo
			LeftDrive.SetMotorOutput(-lStick.GetY());
			RightDrive.SetMotorOutput(-rStick.GetY());
		}
		else {
			//myRobot.TankDrive(-lStick.GetY()*0.85, -rStick.GetY()*0.85, true);	//without turbo
			LeftDrive.SetMotorOutput(-lStick.GetY()*0.85);
			RightDrive.SetMotorOutput(-rStick.GetY()*0.85);
		}
	}
	if (!frontDrive) {		//rear is "forward"
		if (lStick.GetRawButton(1)) {	
			//myRobot.TankDrive(rStick.GetY(), lStick.GetY(), true);	//with turbo
			LeftDrive.SetMotorOutput(rStick.GetY()*0.85);
			RightDrive.SetMotorOutput(lStick.GetY()*0.85);
			
		}
		else {
			//myRobot.TankDrive(rStick.GetY()*0.85, lStick.GetY()*0.85, true);	//without turbo
			LeftDrive.SetMotorOutput(lStick.GetY()*0.85);
			RightDrive.SetMotorOutput(rStick.GetY()*0.85);
		}
	}
	
	
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

