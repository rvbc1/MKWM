/*
 * UARTPCCOM.h
 *
 *  Created on: May 10, 2019
 *      Author: rvbc-
 */

#ifndef UARTPCCOM_H_
#define UARTPCCOM_H_

#include "stm32f1xx_hal.h"

#define AMOUNT_OF_SERVO 4
#define DATA_FRAME_TX_SIZE 8
#define DATA_FRAME_RX_SIZE 10

#define START_CODE 0x40
#define END_CODE 0x80

class UART_PC_COM {

private:
	struct dataFrameTX{
		uint16_t servo[AMOUNT_OF_SERVO];
	} __attribute__ ((__packed__));

	struct dataFrameRX{
		uint8_t start_code;
		uint16_t servo[AMOUNT_OF_SERVO];
		uint8_t end_code;
	} __attribute__ ((__packed__));

	uint8_t Received[20];


	UART_HandleTypeDef * uart_handler;

	void init(UART_HandleTypeDef *uart_handler);
public:
	UART_PC_COM(UART_HandleTypeDef *uart_handler);
	virtual ~UART_PC_COM();

	void sendData();
	uint8_t recieveData();

	UART_HandleTypeDef * getUartHandler();

	union{
		dataFrameTX data;
		uint8_t bytes[DATA_FRAME_TX_SIZE];
	}frameTX;

	union{
		dataFrameRX data;
		uint8_t bytes[DATA_FRAME_RX_SIZE];
	}frameRX;
};

#endif /* UARTPCCOM_H_ */
