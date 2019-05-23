/*
 * UARTPCCOM.cpp
 *
 *  Created on: May 10, 2019
 *      Author: rvbc-
 */

#include "UARTPCCOM.h"

void UART_PC_COM::init(UART_HandleTypeDef *uart_handler, Robot *robot){
	this->uart_handler = uart_handler;

	this->robot = robot;
	robot->addPC(this);

	initFrameTX();
	startUpdatingData();

	HAL_UART_Receive_DMA(uart_handler, frameRX.bytes, DATA_FRAME_RX_SIZE);
	sendData();
}

UART_PC_COM::UART_PC_COM(UART_HandleTypeDef *uart_handler, Robot *robot) {
	init(uart_handler, robot);
}

UART_PC_COM::~UART_PC_COM() {
	// TODO Auto-generated destructor stub
}

void UART_PC_COM::sendData(){
	updateFrameTX();
	HAL_UART_Transmit_DMA(uart_handler, frameTX.bytes, DATA_FRAME_TX_SIZE);
}

void UART_PC_COM::recieveNextData(){
	if((frameRX.data.start_code == START_CODE) && (frameRX.data.end_code == END_CODE)){
		goodDataLoad();
		HAL_UART_Receive_DMA(uart_handler, frameRX.bytes, DATA_FRAME_RX_SIZE);
	} else {
		badDataLoad();
		for(int i = 1; i < DATA_FRAME_RX_SIZE; i++){
			if(frameRX.bytes[i] == START_CODE){
				for(int j = 0; j < DATA_FRAME_RX_SIZE - i; j++){
					frameRX.bytes[j] = frameRX.bytes[j + i];
				}
				HAL_UART_Receive_DMA(uart_handler, frameRX.bytes + DATA_FRAME_RX_SIZE - i, i);
				goto loop_end;
			}
		}
		HAL_UART_Receive_DMA(uart_handler, frameRX.bytes, DATA_FRAME_RX_SIZE);
	}
	loop_end:;
}

void UART_PC_COM::goodDataLoad(){
	if (getRecievedData()->pc_mode == PC_MODE_ON){
		robot->setPCPointsMode();
		if(is_updating_data) robot->updatedData(this->getRecievedData()->servo);
	}
	else if (getRecievedData()->pc_mode == PC_MODE_OFF)	robot->resetPCPointsMode();
}
void UART_PC_COM::badDataLoad(){

}

uint8_t UART_PC_COM::isRecieveDataCorrect(){
	if((frameRX.data.start_code == START_CODE) && (frameRX.data.end_code == END_CODE)){
		return true;
	}
	return false;
}

dataFrameRX * UART_PC_COM::getRecievedData(){
	return &frameRX.data;
}

UART_HandleTypeDef * UART_PC_COM::getUartHandler(){
	return uart_handler;
}

void UART_PC_COM::initFrameTX(){
	frameTX.data.start_code = START_CODE;
	frameTX.data.end_code = END_CODE;
}

void UART_PC_COM::updateFrameTX(){
	for(uint16_t i = 0; i < AMOUNT_OF_SERVO; i++){
		frameTX.data.servo = robot->getCurrentServoData();
	}
}

void UART_PC_COM::startSendingData(){
	is_sending_data = true;
}

void UART_PC_COM::stopSendingData(){
	is_sending_data = false;
}

void UART_PC_COM::startUpdatingData(){
	is_updating_data = true;
}

void UART_PC_COM::stopUpdatingData(){
	is_updating_data = false;
}


