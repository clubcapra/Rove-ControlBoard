/**
 * @file comServo.cpp
 * @brief Implementation of the ComServo class.
 */

#include "../include/comServo.hpp"

/**
 * @brief Default constructor for the ComServo class.
 */
ComServo::ComServo()
{
    
}

/**
 * @brief Reads and writes data to the servo.
 * 
 * This function receives data from the servo using DMA and transmits data to the servo using DMA.
 * 
 * @param tx Pointer to the transmit buffer.
 * @param tx_size Size of the transmit buffer.
 * @param rx Pointer to the receive buffer.
 * @param rx_size Size of the receive buffer.
 */
void ComServo::readWriteServo(uint8_t *tx, uint16_t tx_size, uint8_t *rx, uint16_t rx_size)
{   
    HAL_UARTEx_ReceiveToIdle_DMA(huart2, rx, 16);   
    HAL_UART_Transmit_DMA(huart2, tx, tx_size);
}

/**
 * @brief Sets the UART handle for the ComServo class.
 * 
 * @param huart Pointer to the UART handle.
 */
void ComServo::setHuart(UART_HandleTypeDef *huart)
{
    huart2 = huart;
}


