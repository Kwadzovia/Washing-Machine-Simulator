/*
 * MotorFunctions.h
 *
 *  Created on: Mar 18, 2020
 *      Author: Caleb
 */

#ifndef __MOTORFUNCTIONS_H__ // do not include more than once
#define __MOTORFUNCTIONS_H__

#define START 0x10
#define STOP 0x00

#define MOTOR_MAX 1.0f // Run at 100%
#define MOTOR_MID 0.5f // Run at 50%
#define MOTOR_MIN 0.2f // Run at 20%

void StartMotor(float duty_cycle);
void StopMotor();
void ChangeMotorDirection();
void PwmInit();

#endif
