/*
 * Shooter.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: Oomkes
 */

#include "Shooter.h"


Shooter::Shooter(): // Constructor
			pow(1,PNUEM_SOL) // init list
{
}

Shooter::~Shooter() // Destructor
{	
}

void Shooter::Init(Joystick * joy){
	shooter_stick = joy;
	m_state = 0;
	m_recordButton = 0;
	m_loopCount = 0;
}

void Shooter::HighShot()
	{
	pow.Set(true);
	Wait(0.3);
	pow.Set(false);
	}

void Shooter::Run(){
	
	//shooter code
		switch (m_state){
			case 0://idle
				if (shooter_stick->GetRawButton(1)) {
					m_recordButton = 1;
					}
				else if (shooter_stick->GetRawButton(2)) {
					m_recordButton = 2;
					}
				else if (shooter_stick->GetRawButton(3)) {
					m_recordButton = 3;
					}
				if (m_recordButton){
					m_state = 1;
					m_loopCount = 0;
					}
				break;
			case 1://initial shot
				pow.Set(true);
				m_state = 2;
				break;
			case 2://delay
				m_loopCount++;
				if ((m_loopCount == 1)&&(m_recordButton == 3)){
					m_state = 3;
				}
				break;
			case 3://off
				break;
			case 4://second shot
				break;
		}
}

