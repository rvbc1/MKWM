/*
 * Robot.cpp
 *
 *  Created on: May 11, 2019
 *      Author: rvbc-
 */

#include "Robot.h"

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

