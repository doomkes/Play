/*
 * UserInterface.h
 *
 *  Created on: Nov 23, 2014
 *      Author: Studio-shop
 */
#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include "WPILib.h"


/*
 * Description:
 */
struct TaterUserInput{
	bool highShot;
	bool trussShot;
	bool bumpShot;
	bool frontDrive;
	bool revDrive;
	float leftVal;
	float rightVal;
	bool turbo;
	bool forkUp;
	bool forkDown;	
};


class UserInterface {
private:
	Joystick joy1, joy2, joy3;
	bool m_paradeDrive;
	 
public:
	UserInterface();
	~UserInterface();
	void Init(void);
	void GetData(TaterUserInput *);

};

#endif




