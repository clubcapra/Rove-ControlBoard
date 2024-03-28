/*
 * SCSerial.h
 * hardware interface layer for waveshare serial bus servo
 * date: 2023.6.28
 */


#include "../include/Servo/SCSerial.h"

SCSerial::SCSerial()
{
	pSerial = NULL;
}

SCSerial::SCSerial(u8 End):SCS(End)
{
	pSerial = NULL;
}

SCSerial::SCSerial(u8 End, u8 Level):SCS(End, Level)
{
	pSerial = NULL;
}

int SCSerial::readSCS(unsigned char *nDat, int nLen)
{
	//HAL_UART_Receive_DMA(pSerial, nDat, nLen);
	//HAL_UARTEx_ReceiveToIdle_DMA(pSerial, nDat, nLen);
	//HAL_UART_Receive(pSerial, nDat, nLen,100);

	/*
	int Size = 0;
	int ComData;
	unsigned long t_begin = millis();
	unsigned long t_user;
	while(1){
		ComData = pSerial->read();
		if(ComData!=-1){
			if(nDat){
				nDat[Size] = ComData;
			}
			Size++;
			t_begin = millis();
		}
		if(Size>=nLen){
			break;
		}
		t_user = millis() - t_begin;
		if(t_user>IOTimeOut){
			break;
		}
	}*/
	/*
	if(HAL_UART_Receive(pSerial, nDat, nLen, 200)==HAL_OK){
		return nLen;
	}else{
		return 0;
	}*/
	
	if(HAL_UART_Receive_DMA(pSerial, nDat, nLen)==HAL_OK){
		return nLen;
	}else{
		return 0;
	}

	//return nLen;
}

int SCSerial::writeSCS(unsigned char *nDat, int nLen)
{
	/*
	if(nDat==NULL){
		return 0;
	}
	HAL_UART_Transmit_IT(pSerial, nDat, nLen);
	return nLen;*/
	while(nLen--){
		if(wLen<sizeof(wBuf)){
			wBuf[wLen] = *nDat;
			wLen++;
			nDat++;
		}
	}
	return wLen;
}

int SCSerial::writeSCS(unsigned char bDat)
{
	if(wLen<sizeof(wBuf)){
		wBuf[wLen] = bDat;
		wLen++;
	}
	return wLen;
	/*
	HAL_UART_Transmit_IT(pSerial, &bDat, 1);
	return 1;*/
}

void SCSerial::rFlushSCS()
{
	uint16_t i = 500;
	while(i--);
	
}

void SCSerial::wFlushSCS()
{
	if(wLen){
		HAL_UART_Transmit_IT(pSerial,wBuf, wLen);
		wLen = 0;
	}
}