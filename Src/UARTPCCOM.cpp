/*
 * UARTPCCOM.cpp
 *
 *  Created on: May 10, 2019
 *      Author: rvbc-
 */

#include "UARTPCCOM.h"

void UART_PC_COM::init(UART_HandleTypeDef *uart_handler){
	this->uart_handler = uart_handler;

	HAL_UART_Receive_DMA(uart_handler, frameRX.bytes, DATA_FRAME_RX_SIZE);
}

UART_PC_COM::UART_PC_COM(UART_HandleTypeDef *uart_handler) {
	init(uart_handler);
}

UART_PC_COM::~UART_PC_COM() {
	// TODO Auto-generated destructor stub
}

void UART_PC_COM::sendData(){
	HAL_UART_Transmit(uart_handler, frameTX.bytes, DATA_FRAME_TX_SIZE, HAL_MAX_DELAY);


	//char buffer [20];
	//HAL_UART_Transmit(uart_handler, (uint8_t*)buffer,
	//sprintf(buffer, "%d\n%d\n%d\n%d\n", frameTX.data.servo1, frameTX.data.servo2, frameTX.data.servo3, frameTX.data.servo4), HAL_MAX_DELAY);
}

uint8_t UART_PC_COM::recieveData(){
	if((frameRX.data.start_code == START_CODE) && (frameRX.data.end_code == END_CODE)){
		sendData();
		return true;
	}
	return false;
}

UART_HandleTypeDef * UART_PC_COM::getUartHandler(){
	return uart_handler;
}

