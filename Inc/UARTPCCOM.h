/*
 * UARTPCCOM.h
 *
 *  Created on: May 10, 2019
 *      Author: rvbc-
 */

#ifndef UARTPCCOM_H_
#define UARTPCCOM_H_

#include "stm32f1xx_hal.h"
#include "Robot.h"

//#define AMOUNTS_OF_SERVO 4
#define DATA_FRAME_TX_SIZE 10
#define DATA_FRAME_RX_SIZE 10

#define START_CODE 0x40
#define END_CODE 0x80


struct dataFrameRX{
	uint8_t start_code;
	uint16_t servo[Robot::AMOUNT_OF_SERVO];
	uint8_t end_code;
} __attribute__ ((__packed__));

class UART_PC_COM {


private:
	struct dataFrameTX{
		uint8_t start_code;
		uint16_t servo[Robot::AMOUNT_OF_SERVO];
		uint8_t end_code;
	} __attribute__ ((__packed__));


	union{
		dataFrameRX data;
		uint8_t bytes[DATA_FRAME_RX_SIZE];
	}frameRX;

	union{
		dataFrameTX data;
		uint8_t bytes[DATA_FRAME_TX_SIZE];
	}frameTX;


	UART_HandleTypeDef * uart_handler;

	Robot *robot;

	void init(UART_HandleTypeDef *uart_handler, Robot *robot);
public:

	uint8_t bytes[DATA_FRAME_RX_SIZE];

	UART_PC_COM(UART_HandleTypeDef *uart_handler, Robot *robot);
	virtual ~UART_PC_COM();

	void sendData();
	void recieveNextData();
	uint8_t recieveData();
	dataFrameRX *getRecievedData();

	UART_HandleTypeDef * getUartHandler();


};

#endif /* UARTPCCOM_H_ */
