/*
 * Servo.h
 *
 *  Created on: Apr 7, 2019
 *      Author: rvbc-
 */

#ifndef SERVO_H_
#define SERVO_H_

#define MIN_PULSE_WIDTH 	650
#define MAX_PULSE_WIDTH 	2600
#define DEFAULT_MIN_ANGLE 	0
#define DEFAULT_MAX_ANGLE 	180

#include "stm32f1xx_hal.h"

class Servo {
private:
	volatile uint32_t* PWM_Register;
	uint8_t min_angle;
	uint8_t max_angle;
public:
	void setPulse(uint16_t pulse);
	void setAngle(uint8_t angle);
	void setMinAngle(uint8_t min_angle);
	void setMaxAngle(uint8_t max_angle);
	Servo(volatile uint32_t* PWM_Register);
	virtual ~Servo();
};

#endif /* SERVO_H_ */
