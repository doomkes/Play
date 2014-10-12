/*
 * Shooter.h
 *
 *  Created on: Oct 11, 2014
 *      Author: Oomkes
 */
#include "WPILib.h"
/*
 * Description:
 */
class Shooter {
	Solenoid pow;
	Joystick * shooter_stick; // pointer to joystic for shooting control
	
public:
	Shooter();
	~Shooter();
	void Init(Joystick *joy);
	void Run();

};
