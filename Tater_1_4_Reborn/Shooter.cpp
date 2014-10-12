/*
 * Shooter.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: Oomkes
 */
#include <cstdio>
#include "Shooter.h"

/*
 * Default constructor
 */
Shooter::Shooter():
			pow(1,3) // init list
{

}

/*
 * Default destructor
 */

Shooter::~Shooter()
{
	
}

void Shooter::Init(Joystick * joy){
	shooter_stick = joy;
}

void Shooter::Run(){
	if (shooter_stick->GetRawButton(1))
		{
		printf("Shoot Man!!!!");
		pow.Set(true);
		}
	else
		pow.Set(false);
	
}

