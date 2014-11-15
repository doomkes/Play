/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "TaterDrive.h"
#include "CANJaguar.h"
#include "GenericHID.h"
#include "Joystick.h"
#include "Victor.h"
#include "NetworkCommunication/UsageReporting.h"
#include "Utility.h"
#include "WPIErrors.h"
#include <math.h>

#define max(x, y) (((x) > (y)) ? (x) : (y))

/*
 * Driving functions
 * These functions provide an interface to multiple motors that is used for C programming
 * The Drive(speed, direction) function is the main part of the set that makes it easy
 * to set speeds and direction independently in one call.
 */

/**
 * Common function to initialize all the robot drive constructors.
 * Create a motor safety object (the real reason for the common code) and
 * initialize all the motor assignments. The default timeout is set for the robot drive.
 */
void TaterDrive::InitTaterDrive() {
	m_Motor = NULL;
	m_sensitivity = 0.5;
	m_maxOutput = 1.0;
	m_safetyHelper = new MotorSafetyHelper(this);
	m_safetyHelper->SetSafetyEnabled(true);
}

/** Constructor for TaterDrive with 2 motors specified with channel numbers.
 * Set up parameters for a two wheel drive system where the
 * left and right motor pwm channels are specified in the call.
 * This call assumes Victors for controlling the motors.
 * @param leftMotorChannel The PWM channel number on the default digital module that drives the left motor.
 * @param rightMotorChannel The PWM channel number on the default digital module that drives the right motor.
 */
TaterDrive::TaterDrive(uint32_t MotorChannel)
{
	InitTaterDrive();
	m_Motor = new Victor(MotorChannel);
	m_invertedMotor = 1;
	SetMotorOutput(0.0);
	m_deleteSpeedControllers = true;
}

/**
 * TaterDrive destructor.
 * Deletes motor objects that were not passed in and created internally only.
 **/
TaterDrive::~TaterDrive()
{
	if (m_deleteSpeedControllers)
	{
		delete m_Motor;
	}
	delete m_safetyHelper;
}

/** Set the speed of the motor.
 * The motor is set to "Output"
 * and includes flipping the direction of one side for opposing motors.
 * @param Output The speed to send to the motor.
 */
void TaterDrive::SetMotorOutput(float Output)
{
	wpi_assert(m_Motor != NULL);

	uint8_t syncGroup = 0x80;

	if (m_Motor != NULL)
		m_Motor->Set(Limit(Output) * m_invertedMotor, syncGroup);

	m_safetyHelper->Feed();
}

/**
 * Limit motor values to the -1.0 to +1.0 range.
 */
float TaterDrive::Limit(float num)
{
	if (num > 1.0)
	{
		return 1.0;
	}
	if (num < -1.0)
	{
		return -1.0;
	}
	return num;
}

/*
 * Invert a motor direction.
 * @param isInverted True if the motor should be inverted when operated.
 */
void TaterDrive::SetInvertedMotor(bool isInverted)
{
	m_invertedMotor = isInverted ? -1 : 1;
}


void TaterDrive::PIDWrite (float output){
	SetMotorOutput(output); // forward the setting from the PID controller.
}

void TaterDrive::SetExpiration(float timeout)
{
	m_safetyHelper->SetExpiration(timeout);
}

float TaterDrive::GetExpiration()
{
	return m_safetyHelper->GetExpiration();
}

bool TaterDrive::IsAlive()
{
	return m_safetyHelper->IsAlive();
}

bool TaterDrive::IsSafetyEnabled()
{
	return m_safetyHelper->IsSafetyEnabled();
}

void TaterDrive::SetSafetyEnabled(bool enabled)
{
	m_safetyHelper->SetSafetyEnabled(enabled);
}

void TaterDrive::GetDescription(char *desc)
{
	sprintf(desc, "TaterDrive - One motor");
}

void TaterDrive::StopMotor()
{
	if (m_Motor != NULL) m_Motor->Disable();
}
