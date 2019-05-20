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
//#include "UARTPCCOM.h"
#include <vector>

#define TIME_BETWEEN_POINTS 600
#define LONG_CLICK_TIME 1000

struct dataFrameRX;
class UART_PC_COM;

struct servoAngleData{
	uint16_t angle[AMOUNT_OF_SERVO];
} __attribute__ ((__packed__));


class Robot {
private:
	uint8_t saved_points_mode;
	uint16_t current_saved_point;


	TIM_HandleTypeDef *points_timer;
	volatile uint32_t *points_timer_speed_register;

	Servo *servo [AMOUNT_OF_SERVO];
	servoAngleData current_angle_servo;
	std::vector <servoAngleData> saved_points_vector;

	UART_PC_COM *pc;

	void setOnePulseMode(TIM_HandleTypeDef *htim);
	void resetOnePulseMode(TIM_HandleTypeDef *htim);
	void resetTimer(TIM_HandleTypeDef *htim);


	void init(volatile uint32_t* PWM_0, volatile uint32_t* PWM_1, volatile uint32_t* PWM_2,volatile uint32_t* PWM_3,TIM_HandleTypeDef *points_timer);
public:
	//CONST VALUES
	//static const unsigned int AMOUNT_OF_SERVO = 4;

	Robot(volatile uint32_t* PWM_0, volatile uint32_t* PWM_1, volatile uint32_t* PWM_2,volatile uint32_t* PWM_3,TIM_HandleTypeDef *points_timer);
	virtual ~Robot();

	void addPC(UART_PC_COM *pc_handler);

	void moveToNextPoint();
	void moveToSavedPoints();
	void addPoint(servoAngleData servoData);
	void updatedData(servoAngleData servoData);

	uint8_t isSavedPointsMode();
	void setSavedPointsMode();
	void resetSavedPointsMode();

	void shortClick();
	void longClick();
	void buttonIT(uint8_t button_state);
	void timerIT();

	servoAngleData getCurrentServoData();
};

#endif /* ROBOT_H_ */
