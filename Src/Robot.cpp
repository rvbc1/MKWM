/*
 * Robot.cpp
 *
 *  Created on: May 11, 2019
 *      Author: rvbc-
 */

#include "Robot.h"
#include "UARTPCCOM.h"

void Robot::init(volatile uint32_t* PWM_0, volatile uint32_t* PWM_1, volatile uint32_t* PWM_2,volatile uint32_t* PWM_3){
	servo[0] = new Servo(PWM_0);
	servo[1] = new Servo(PWM_1);
	servo[2] = new Servo(PWM_2);
	servo[3] = new Servo(PWM_3);
}

Robot::Robot(volatile uint32_t* PWM_0, volatile uint32_t* PWM_1, volatile uint32_t* PWM_2,volatile uint32_t* PWM_3) {
	init(PWM_0, PWM_1, PWM_2, PWM_3);
}

Robot::~Robot() {
	// TODO Auto-generated destructor stub
}

void Robot::updatedData(dataFrameRX *dataBuffer){
	setServoFromDataBuffer(dataBuffer->servo);
}

void Robot::setServoFromDataBuffer(uint16_t *buffer){
	for(unsigned int i = 0; i < AMOUNT_OF_SERVO; i++){
		servo[i]->setAngle(buffer[i]);
	}
}

