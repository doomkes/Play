/*
 * Test.h
 *
 *  Created on: Oct 10, 2014
 *      Author: Studio-shop
 */

/*
 * Description:
 */
class TrapezoidalMoveProfile {
	float m_accel, m_decel, m_max_speed, m_distance;
	float m_t1, m_t2, m_t3;
public:
	TrapezoidalMoveProfile();
	TrapezoidalMoveProfile(float Accel, float Decel, float MaxSpeed, float Distance);
	void SetAccel(float);		// feet per second per second
	void SetDecel(float);		// feet per second per second
	void SetMaxSpeed(float);	// feet per second
	void SetDistance(float);
	void CalcParams(void);
	float Position(float time);

};
