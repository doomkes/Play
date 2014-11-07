#ifndef ROBOTMAP_H_
#define ROBOTMAP_H_

/*
 * RobotMap.h
 *
 *  Created on: Oct 12, 2014
 *      Author: Oomkes
 */

/*
 * Description: Mapping file for all of the solenoid outputs and pwm channels
 */
// USB CHANNELS
#define LTANK_JOY_USB 1
#define RTANK_JOY_USB 2
#define SHOOTER_JOY_USB 3


// PWM OUTPUTS
#define RIGHT_MOTOR_PWM 1
#define PWM_2 2
#define PWM_3 3
#define PWM_4 4
#define PWM_5 5
#define PWM_6 6
#define PWM_7 7
#define PWM_8 8
#define PWM_9 9
#define LEFT_MOTOR_PWM 10

// SOLENOID CHANNELS
#define FORK_UP_SOL 1
#define FORK_DN_SOL 2
#define PNUEM_SOL 3
#define ARC_RCTR_SOL 4
#define SOLENOID_5
#define SOLENOID_5
#define SOLENOID_6
#define SOLENOID_7
#define SOLENOID_8

// RELAY CHANNELS
#define COMPRESSOR_RLY 1
#define RELAY_2 2
#define RELAY_3 3
#define RELAY_4 4
#define RELAY_5 5
#define RELAY_6 6
#define RELAY_7 7
#define RELAY_8 8

//DIGITAL IO
#define PRESS_SW_DIO 1
#define DIO_3
#define DIO_2
#define CODE_LT_A 4
#define CODE_LT_B 5
#define CODE_RT_A 6
#define CODE_RT_B 7
#define DIO_8
#define DIO_9
#define DIO_10
#define DIO_11
#define DIO_12
#define DIO_13
#define DIO_14

//MISC.
#define INCH_PER_CNT (12.566/1000) //wheel circumference devided by lines-per-revolution

#endif






