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
	static int  lastButton1 = 0,lastButton2 = 0,lastButton3 = 0;
	static int  currButton1 = 0,currButton2 = 0,currButton3 = 0;
	//shooter code
	currButton1 = (shooter_stick->GetRawButton(1));
	currButton2 = (shooter_stick->GetRawButton(2));
	currButton3 = (shooter_stick->GetRawButton(3));
		switch (m_state){

			case 0://idle
			//printf ("we are in case 0 \n" );
				m_recordButton = 0;
				m_loopCount = 0;
				if (currButton1 && !lastButton1) {	//full shot
					m_recordButton = 1;
					}
				else if (currButton2 && !lastButton2) {	//truss shot
					m_recordButton = 2;
					}
				else if (currButton3 && !lastButton3) {	//bump shot
					m_recordButton = 3;
					}
				if (m_recordButton){
					m_state = 1;
					m_loopCount = 0;
					}
				break;

			case 1://initial shot
				printf ("we are in case 1 \n" );
				pow.Set(true);
				m_state = 2;
				m_loopCount = 0;
				break;
			case 2://initial shot delay
				printf ("we are in case 2 \n" );
				if ((m_loopCount == 10)&&(m_recordButton == 1)){
					m_state = 3;
				}
				if ((m_loopCount == 7)&&(m_recordButton == 2)){
					m_state = 3;
				}
				if ((m_loopCount == 1)&&(m_recordButton == 3)){
					m_state = 3;
				}
				break;

			case 3://off
				printf ("we are in case 3 \n" );
				pow.Set(false);
				m_state = 4;
				m_loopCount = 0;
				break;

			case 4://off delay
				printf ("we are in case 4 \n" );
				if (m_recordButton == 1){
					m_state = 0;
				}
				if (m_recordButton == 2){
					m_state = 0;
				}
				if ((m_loopCount == 4)&&(m_recordButton == 3)){
					m_state = 5;
				}

				break;

			case 5://second shot (for bump shot only)
				printf ("we are in case 5 \n" );
				pow.Set(true);
				m_state = 6;
				m_loopCount = 0;
				break;

			case 6://second shot delay
				printf ("we are in case 6 \n" );
				if (m_loopCount == 3){
				pow.Set(false);
				m_state = 0;
				} 
				break;
		}
		m_loopCount++;
		currButton1 = lastButton1;
		currButton2 = lastButton2;
		currButton3 = lastButton3;
}
