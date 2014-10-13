/*
 * Shooter.h
 *
 *  Created on: Oct 11, 2014
 *      Author: Oomkes
 */
#include "WPILib.h"
#include "RobotMap.h"
#include <cstdio>
/*
 * Description:
 */
class Shooter {
	Solenoid pow;
	Joystick * shooter_stick; // pointer to joystic for shooting control
	int m_state;
	int m_recordButton;
	int m_loopCount;
	
public:
	Shooter();
	~Shooter();
	void Init(Joystick *joy);
	void HighShot();
	void Run();

};
