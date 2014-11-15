/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef TATERDRIVE_H_
#define TATERDRIVE_H_

#include "ErrorBase.h"
#include <stdlib.h>
#include <vxWorks.h>
#include "MotorSafety.h"
#include "MotorSafetyHelper.h"
#include "PIDOutput.h"


class SpeedController;
class GenericHID;

/**
 * Utility class for handling Robot drive based on a definition of the motor configuration.
 * The robot drive class handles basic driving for a robot. Currently, 2 and 4 motor standard
 * drive trains are supported. In the future other drive types like swerve and meccanum might
 * be implemented. Motor channel numbers are passed supplied on creation of the class. Those are
 * used for either the Drive function (intended for hand created drive code, such as autonomous)
 * or with the Tank/Arcade functions intended to be used for Operator Control driving.
 */
class TaterDrive: public MotorSafety, public ErrorBase, public PIDOutput
{
public:

	TaterDrive(uint32_t MotorChannel);
	virtual ~TaterDrive();
	void PIDWrite (float output);
	void TankDrive(float Value, bool squaredInputs = true);
	virtual void SetMotorOutput(float Output);
	void SetInvertedMotor(bool isInverted);

	void SetExpiration(float timeout);
	float GetExpiration();
	bool IsAlive();
	void StopMotor();
	bool IsSafetyEnabled();
	void SetSafetyEnabled(bool enabled);
	void GetDescription(char *desc);

protected:
	void InitTaterDrive();
	float Limit(float num);

	int32_t m_invertedMotor;
	float m_sensitivity;
	double m_maxOutput;
	bool m_deleteSpeedControllers;
	SpeedController *m_Motor;
	MotorSafetyHelper *m_safetyHelper;

	DISALLOW_COPY_AND_ASSIGN(TaterDrive);
};

#endif

