/*
 * Robot.h
 *
 *  Created on: May 11, 2019
 *      Author: rvbc-
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#define AMOUNT_OF_SERVO 4

#include "Servo.h"

class Robot {
private:


	void init();
public:
	Robot();
	virtual ~Robot();

	Servo *servo [AMOUNT_OF_SERVO];
};

#endif /* ROBOT_H_ */
