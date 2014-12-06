/*
 * Shooter.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: Oomkes
 */

#include "Shooter.h"
#include "UserInterface.h"
#include <taskLib.h>


static void ShootBall(Shooter *s) 
{
while(1){
 		int startTime =GetFPGATime();
 		double length = 0.130;
 		length = SmartDashboard::GetNumber("something");
 		if (!(0.0 < length < 0.2))
 			length = 0.130;
 					 
		switch(s->GetShotType())
		{
		case HIGH_SHOT:
			printf("High Shot ----> On:%lu  ",GetFPGATime()-startTime);
			SmartDashboard::PutString("shotType","That was a high shot");
			s->SolenoidOn(true);
			Wait(0.2);
			printf("Off:%lu \n\n",GetFPGATime()-startTime);
			s->SolenoidOn(false);
			break;
		case TRUSS_SHOT:
			printf("Truss Shot ----> On:%lu   ",GetFPGATime()-startTime);
			SmartDashboard::PutString("shotType","That was a truss shot");
			s->SolenoidOn(true);
			Wait(length);
			printf("Off:%lu\n\n",GetFPGATime()-startTime);
			s->SolenoidOn(false);	
			break;
		case BUMP_SHOT:
			printf("Bump Shot ----> On:%lu  ",GetFPGATime()-startTime);
			SmartDashboard::PutString("shotType","That was a bump shot");
			s->SolenoidOn(true);
			Wait(0.01);
			printf("Off:%lu   ",GetFPGATime()-startTime);		
			s->SolenoidOn(false);
			Wait(0.08);
			printf("On:%lu   ",GetFPGATime()-startTime);		
			s->SolenoidOn(true);
			Wait(0.06);
			printf("Off:%lu\n\n",GetFPGATime()-startTime);		
			s->SolenoidOn(false);
			break;
		case CUSTOM_SHOT:
			printf("Custom Shot ----> On:%lu  ",GetFPGATime()-startTime);
			SmartDashboard::PutString("shotType","That was a custom shot");
			s->SolenoidOn(true);
			Wait(SmartDashboard::GetNumber("firstShot"));
			printf("Off:%lu   ",GetFPGATime()-startTime);		
			s->SolenoidOn(false);
			Wait(SmartDashboard::GetNumber("delay"));
			printf("On:%lu   ",GetFPGATime()-startTime);		
			s->SolenoidOn(true);
			Wait(SmartDashboard::GetNumber("secondShot"));
			printf("Off:%lu\n\n",GetFPGATime()-startTime);		
			s->SolenoidOn(false);
			break;
		case NO_SHOT:
			break;
		}
	
		Wait(0.1); // hog prevention
		s->Complete();
}
/*	while(1){
		printf("myTask\n");
		s->SolenoidOn(true);
		Wait(0.5);
		s->SolenoidOn(false);
		Wait(0.5);
	}
 */
			
		
}

Shooter::Shooter(): // Constructor
	pow(1,PNUEM_SOL), // init list
	m_task ("Shooter", (FUNCPTR)ShootBall),
	m_shotType(NO_SHOT),
	m_state(0),
    m_recordButton(0),
    m_loopCount(0)
{
	//m_task.Start();
	//m_task.Suspend();
}

Shooter::~Shooter() // Destructor
{	
}
void Shooter::SolenoidOn(bool value){
	pow.Set(value);
	return;
}

void Shooter::Init(void){
	m_task.Start((int32_t)this);
	
}

void Shooter::HighShot()
	{
	pow.Set(true);
	Wait(0.3);
	pow.Set(false);
	return;
	}

void Shooter::Run(TaterUserInput *tui){
	static int  lastButton1 = 0,lastButton2 = 0,lastButton3 = 0;
	static int  currButton1 = 0,currButton2 = 0,currButton3 = 0;
	//shooter code
	currButton1 = (tui->highShot);
	currButton2 = (tui->trussShot);
	currButton3 = (tui->bumpShot);
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
			if (m_recordButton == 3){
				Wait(0.01);
				pow.Set(false);
				m_state = 3;
			}
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
	lastButton1 = currButton1;
	lastButton2 = currButton2;
	lastButton3 = currButton3;

}

void Shooter::Run2(TaterUserInput *tui){
	static int  lastHighShot = 0,lastTrussShot = 0,lastBumpShot = 0;
	//shooter code -- let the rest of the code starve while performing shots.
	int startTime =GetFPGATime();
	if (tui->highShot && !lastHighShot) 
		{
		printf("High Shot ----> On:%lu  ",GetFPGATime()-startTime);
		pow.Set(true);
		Wait(0.2);
		printf("Off:%lu \n\n",GetFPGATime()-startTime);
		pow.Set(false);
		}
	else if (tui->trussShot && !lastTrussShot) 
		{
		printf("Truss Shot ----> On:%lu   ",GetFPGATime()-startTime);
		pow.Set(true);
		Wait(0.13);
		printf("Off:%lu\n\n",GetFPGATime()-startTime);
		pow.Set(false);		
		}
	else if (tui->bumpShot && !lastBumpShot) 
		{
		printf("Bump Shot ----> On:%lu  ",GetFPGATime()-startTime);		
		pow.Set(true);
		Wait(0.01);
		printf("Off:%lu   ",GetFPGATime()-startTime);		
		pow.Set(false);
		Wait(0.08);
		printf("On:%lu   ",GetFPGATime()-startTime);		
		pow.Set(true);
		Wait(0.06);
		printf("Off:%lu\n\n",GetFPGATime()-startTime);		
		pow.Set(false);
		}
	
	lastHighShot = tui->highShot;
	lastTrussShot = tui->trussShot;
	lastBumpShot = tui->bumpShot;

}

void Shooter::Run3(TaterUserInput *tui){
	static int  lastHighShot = 0,lastTrussShot = 0,lastBumpShot = 0, lastCustomShot = 0;
	//shooter code -- let the rest of the code starve while performing shots.
	if(m_shotType == NO_SHOT){
		if (tui->highShot && !lastHighShot){
			m_shotType = HIGH_SHOT;			
			m_task.Resume();
			}
		else if (tui->trussShot && !lastTrussShot){ 
			m_shotType = TRUSS_SHOT;
			m_task.Resume();			
			}
		else if (tui->bumpShot && !lastBumpShot){
			m_shotType = BUMP_SHOT;
			m_task.Resume();
			}
		else if (tui->customShot && !lastCustomShot) {
			m_shotType = CUSTOM_SHOT;
			m_task.Resume();
		}
	}

	lastHighShot = tui->highShot;
	lastTrussShot = tui->trussShot;
	lastBumpShot = tui->bumpShot;
	lastCustomShot = tui->customShot;
}
void Shooter::Complete(void){
	m_shotType = NO_SHOT;
	//m_task.Suspend();
	m_task.Suspend();
}
ShotType Shooter::GetShotType(void){
	return m_shotType;
	
}

