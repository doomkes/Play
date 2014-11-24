/*
 * Shooter.h
 *
 *  Created on: Oct 11, 2014
 *      Author: Oomkes
 */
#include "WPILib.h"
#include "RobotMap.h"
#include "UserInterface.h"
#include <cstdio>
/*
 * Description:
 */
class Shooter {
	Solenoid pow;
	int m_state;
	int m_recordButton;
	int m_loopCount;
	
public:
	Shooter();
	~Shooter();
	void Init(void);
	void HighShot(void);
	void Run(TaterUserInput *);
	void Run2(TaterUserInput *);

};
