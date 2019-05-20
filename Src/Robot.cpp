/*
 * Robot.cpp
 *
 *  Created on: May 11, 2019
 *      Author: rvbc-
 */

#include "Robot.h"
#include "UARTPCCOM.h"

bool operator==(const servoAngleData& el_1, const servoAngleData& el_2){
	for(uint16_t i = 0; i < AMOUNT_OF_SERVO; i++){
		if (el_1.angle[i] != el_2.angle[i]) return false;
	}
	return true;
}

bool operator!=(const servoAngleData& el_1, const servoAngleData& el_2){
	for(uint16_t i = 0; i < AMOUNT_OF_SERVO; i++){
		if (el_1.angle[i] != el_2.angle[i]) return true;
	}
	return false;
}

void Robot::init(volatile uint32_t* PWM_0, volatile uint32_t* PWM_1, volatile uint32_t* PWM_2,volatile uint32_t* PWM_3, TIM_HandleTypeDef *points_timer){
	servo[0] = new Servo(PWM_0);
	servo[1] = new Servo(PWM_1);
	servo[2] = new Servo(PWM_2);
	servo[3] = new Servo(PWM_3);

	this->pc = NULL;
	this->points_timer = points_timer;
	this->points_timer_speed_register = &points_timer->Instance->ARR;

	//CLEAR ANGLE VALUE
	current_angle_servo.angle[0] = 0;
	current_angle_servo.angle[1] = 0;
	current_angle_servo.angle[2] = 0;
	current_angle_servo.angle[3] = 0;

	updatedData(current_angle_servo);

	current_saved_point = 0;
	resetSavedPointsMode();
	setOnePulseMode(points_timer);
	HAL_TIM_Base_Start_IT(points_timer);
	resetTimer(points_timer);
}

Robot::Robot(volatile uint32_t* PWM_0, volatile uint32_t* PWM_1, volatile uint32_t* PWM_2,volatile uint32_t* PWM_3, TIM_HandleTypeDef *points_timer) {
	init(PWM_0, PWM_1, PWM_2, PWM_3, points_timer);
}

Robot::~Robot() {
	// TODO Auto-generated destructor stub
}

void Robot::addPC(UART_PC_COM *pc_handler){
	this->pc = pc_handler;
}

void Robot::moveToNextPoint(){
	updatedData(saved_points_vector[current_saved_point]);
	current_saved_point++;
	if(current_saved_point == saved_points_vector.size()){
		HAL_TIM_Base_Stop_IT(points_timer);
	}
}

void Robot::moveToSavedPoints(){
	current_saved_point = 0;
	//	moveToNextPoint();
	//	HAL_TIM_Base_Start_IT(points_timer);
}

void Robot::addPoint(servoAngleData servoData){
	if(saved_points_vector.empty())
		saved_points_vector.push_back(servoData);
	else if(servoData != saved_points_vector.back())
		saved_points_vector.push_back(servoData);
}

void Robot::updatedData(servoAngleData servoData){
	current_angle_servo = servoData;
	for(unsigned int i = 0; i < AMOUNT_OF_SERVO; i++){
		servo[i]->setAngle(servoData.angle[i]);
	}
}

servoAngleData Robot::getCurrentServoData(){
	return current_angle_servo;
}

void Robot::setOnePulseMode(TIM_HandleTypeDef *htim){
	htim->Instance->CR1 |= TIM_OPMODE_SINGLE;
}

void Robot::resetOnePulseMode(TIM_HandleTypeDef *htim){
	htim->Instance->CR1 &= ~TIM_OPMODE_SINGLE;
}

void Robot::resetTimer(TIM_HandleTypeDef *htim){
	HAL_TIM_Base_Stop_IT(htim);
	__HAL_TIM_SET_COUNTER(htim, 0);
}

uint8_t Robot::isSavedPointsMode(){
	return saved_points_mode;
}

void Robot::setSavedPointsMode(){
	saved_points_mode = true;
}

void Robot::resetSavedPointsMode(){
	this->saved_points_mode = false;
}

void Robot::shortClick(){ //ADD POINT
	resetTimer(points_timer);
	addPoint(getCurrentServoData());
}

void Robot::longClick(){ //MOVE TO SAVED POINTS
	saved_points_mode = true;
	//robot->moveToSavedPoints();
	if(pc != NULL) pc->stopUpdatingData();
	resetOnePulseMode(points_timer);
	*points_timer_speed_register = TIME_BETWEEN_POINTS;
	HAL_TIM_Base_Start_IT(points_timer);
}

void Robot::buttonIT(uint8_t button_state){
	if(button_state == 0){
		*points_timer_speed_register = LONG_CLICK_TIME;
		HAL_TIM_Base_Start_IT(points_timer);
	} else {
		if(saved_points_mode == false){
			shortClick();
		}
	}
}

void Robot::timerIT(){
	if(isSavedPointsMode()){
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		updatedData(saved_points_vector[current_saved_point]);
		current_saved_point++;
		if(current_saved_point >= saved_points_vector.size()){
			resetTimer(points_timer);
			setOnePulseMode(points_timer);
			saved_points_mode = false;
			if(pc != NULL) pc->startUpdatingData();
			current_saved_point = 0;
		}
	} else if(	HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0){
		longClick();
	}
}


