/*
 * Robot.cpp
 *
 *  Created on: May 11, 2019
 *      Author: rvbc-
 */

#include "Robot.h"
#include "UARTPCCOM.h"

void Robot::init(){
	servo[0] = new Servo(&TIM4->CCR1);
	servo[1] = new Servo(&TIM4->CCR2);
	servo[2] = new Servo(&TIM4->CCR3);
	servo[3] = new Servo(&TIM4->CCR4);
}

Robot::Robot() {
	init();
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

