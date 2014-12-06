/*
 * UI.cpp
 *
 *  Created on: Nov 23, 2014
 *      Author: Studio-shop
 */
#include "UserInterface.h"
/*
 * Default constructor
 */
UserInterface::UserInterface():
		joy1(1),						// as they are declared above.
		joy2(2),
		joy3(3),
		m_paradeDrive(true)
{
}

/*
 * Default destructor
 */
UserInterface::~UserInterface()
{
}
void UserInterface::Init(){
	m_paradeDrive = DriverStation::GetInstance()->GetDigitalIn(2);
	if (m_paradeDrive) printf("Parade Drive it is!!!\n");
}

// When in parade mode, The gamepad show up on Port 2
// When in normla mode, Joy1 is the left stick, Joy2 is the right stick,
// Joy3 is the fork control stick


void  UserInterface::GetData(TaterUserInput *tui){
	
	if(m_paradeDrive){
		tui->frontDrive = joy1.GetRawButton(3);
		tui->revDrive 	= joy1.GetRawButton(2);
		tui->turbo		= joy1.GetRawButton(11) | joy1.GetRawButton(12);
		tui->leftVal	= joy1.GetY();
		tui->rightVal	= joy1.GetRawAxis(4);
		
		tui->highShot 	= joy1.GetRawButton(7);
		tui->trussShot 	= joy1.GetRawButton(5);
		tui->bumpShot 	= joy1.GetRawButton(9);
		tui->customShot = joy1.GetRawButton(10);
	
		tui->forkDown	= joy1.GetRawButton(8);	
		tui->forkUp		= joy1.GetRawButton(6);	  
	 }
	 else{

		tui->frontDrive = joy1.GetRawButton(3);
		tui->revDrive 	= joy1.GetRawButton(2);
		tui->turbo		= joy2.GetRawButton(1);	
		tui->leftVal	= joy1.GetY();
		tui->rightVal	= joy2.GetY();
		
		tui->highShot 	= joy2.GetRawButton(1);
		tui->trussShot 	= joy2.GetRawButton(2);
		tui->bumpShot 	= joy2.GetRawButton(3);
		
		tui->forkDown	= joy3.GetY() < -0.1;	
		tui->forkUp		= joy3.GetY() > 0.1;	
		tui->customShot = joy1.GetRawButton(5);
	 }		
}
