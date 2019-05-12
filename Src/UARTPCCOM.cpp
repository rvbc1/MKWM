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

	HAL_UART_Receive_DMA(uart_handler, frameRX.bytes, DATA_FRAME_RX_SIZE);
}

UART_PC_COM::UART_PC_COM(UART_HandleTypeDef *uart_handler, Robot *robot) {
	init(uart_handler, robot);
}

UART_PC_COM::~UART_PC_COM() {
	// TODO Auto-generated destructor stub
}

void UART_PC_COM::sendData(){
	HAL_UART_Transmit_DMA(uart_handler, frameRX.bytes, DATA_FRAME_RX_SIZE);


	//char buffer [20];
	//HAL_UART_Transmit(uart_handler, (uint8_t*)buffer,
	//sprintf(buffer, "%d\n%d\n%d\n%d\n", frameTX.data.servo1, frameTX.data.servo2, frameTX.data.servo3, frameTX.data.servo4), HAL_MAX_DELAY);
}

void UART_PC_COM::recieveNextData(){
	if((frameRX.data.start_code == START_CODE) && (frameRX.data.end_code == END_CODE)){
		robot->updatedData(this->getRecievedData());
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_UART_Receive_DMA(uart_handler, frameRX.bytes, DATA_FRAME_RX_SIZE);
	} else {
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

uint8_t UART_PC_COM::recieveData(){
	if((frameRX.data.start_code == START_CODE) && (frameRX.data.end_code == END_CODE)){
		//		sendData();
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

