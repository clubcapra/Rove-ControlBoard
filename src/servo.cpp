#include "servo.h"

#include <stdarg.h>
#include <stdbool.h>

//#include "logging.h"



//osEventFlagsId_t servo_received_flag;

/**
 * @brief Constructs a new instance of the CServo class.
 *
 * @param servoId The ID of the servo.
 * @param acc The acceleration value for the servo.
 * @param goalSpeed The goal speed for the servo.
 */
CServo::CServo(uint8_t servoId, uint8_t acc, uint16_t goalSpeed)
{
    
    // Initialize the servo object
    s.servoId = servoId;
    s.state.voltageV = 0;
    s.state.currentMa = 0;
    s.state.position = 0;
    s.state.speed = 0;
    s.state.load = 0;

    
    servoWriteMemAddr(SERVO_SRAM_ACC, acc);
    servoWriteMemAddrU16(SERVO_SRAM_GOAL_SPEED_L, goalSpeed);
    servoEnableWheelMode();
   
}



/**
 * @brief Checks if the received message checksum is valid.
 *
 * This function verifies the checksum of a received message by comparing it with the expected checksum.
 *
 * @param totalMsgLen The total length of the received message.
 * @return `true` if the checksum is valid, `false` otherwise.
 */
bool CServo::rxChecksumOk(const uint8_t totalMsgLen)
{
    uint8_t cc = 0;
    for (uint8_t i = 2; i < totalMsgLen - 1; i++)
    {
        cc += rxBuffer[i];
    }
    return (cc ^ 0xff) == rxBuffer[totalMsgLen - 1];
}

// ...




/**
 * Creates a servo command in the given buffer.
 *
 * Message protocol, the protocol is in principle I2C
 *
 * Host -> Servo
 * cmd   [0xff 0xff ID MSG_LEN 0x01 CMD      CHECKSUM          ]
 * read  [0xff 0xff ID MSG_LEN 0x02 MEMADDR  LEN      CHECKSUM ]
 * write [0xff 0xff ID MSG_LEN 0x03 MEMADDR  <DATA>   CHECKSUM ]
 *
 * Servo -> Host
 * ack   [0xff 0xff ID MSG_LEN ERR CHECKSUM          ]
 * resp  [0xff 0xff ID MSG_LEN ERR <DATA>   CHECKSUM ]
 *
 * Checksum is calculated as: ~(ID + MSG_LEN + <DATA>)
 *
 * @param data_bytes the number of data bytes
 * @param ...
 * @return size of command in bytes
 */
size_t CServo::constructMessage(size_t dataBytes, ...)
{
    va_list argPtr;
    uint8_t currentByte;

    // id + data + checksum
    uint8_t cc = s.servoId + dataBytes + 1;

    txBuffer[2] = s.servoId;
    txBuffer[3] = dataBytes + 1;

    va_start(argPtr, dataBytes);
    for (size_t j = 0; j < dataBytes; j++)
    {
        currentByte = va_arg(argPtr, int);
        txBuffer[4 + j] = currentByte;
        cc += currentByte;
    }
    va_end(argPtr);

    txBuffer[4 + dataBytes] = cc ^ 0xff;

    // 5 bytes of metadata + n bytes of data
    // - preamble: 2
    // - servo id: 1
    // - message length: 1
    // - data bytes: n
    // - checksum: 1
    return 5 + dataBytes;
}


/**
 * @brief Sends a command to the servo.
 *
 * This function sends a command to the servo specified by the `cmd` parameter.
 *
 * @param cmd The command to send to the servo.
 * @return `true` if the command was sent successfully, `false` otherwise.
 */
bool CServo::servoCommand(uint8_t cmd)
{

    size_t nBytes = constructMessage(cmd);

    comServo->readWriteServo(txBuffer, nBytes, rxBuffer, nBytes);

    if (!rxChecksumOk(nBytes))
    {
        return false;
    }

    return true;
}

// ...

/**
 * Enables the wheel mode for the servo.
 *
 * This function enables the wheel mode for the servo, allowing it to rotate continuously like a wheel.
 * Once the wheel mode is enabled, the servo will keep rotating until it is explicitly stopped.
 *
 * @return True if the wheel mode was successfully enabled, false otherwise.
 */
bool CServo::servoEnableWheelMode()
{
    bool res;

    res = servoWriteMemAddr(SERVO_SRAM_LOCK, 0);
    if (!res)
        return res;

    res = servoWriteMemAddr(SERVO_EEPROM_MODE, 1);
    if (!res)
        return res;

    res = servoWriteMemAddr(SERVO_SRAM_LOCK, 1);
    if (!res)
        return res;

    return true;
}

bool CServo::servoEnableServoMode()
{
    bool res;

    res = servoWriteMemAddr(SERVO_SRAM_LOCK, 0);
    if (!res)
        return res;

    res = servoWriteMemAddr(SERVO_EEPROM_MODE, 0);
    if (!res)
        return res;

    res = servoWriteMemAddr(SERVO_SRAM_LOCK, 1);
    if (!res)
        return res;

    return true;

}

// ...


/**
 * Writes a value to a memory address of the servo.
 *
 * @param addr The memory address to write to.
 * @param value The value to write.
 * @return True if the write operation was successful, false otherwise.
 */
bool CServo::servoWriteMemAddr(uint8_t addr, uint8_t value)
{
    bool res;

    size_t nBytes = constructMessage(3, SERVO_MEM_WRITE_CMD, addr, value);

    comServo->readWriteServo(txBuffer, nBytes, rxBuffer, 6);

    if (!rxChecksumOk(6))
        return false;

    return res;
}


/**
 * Writes a 16-bit value to a memory address of the servo.
 *
 * @param addr The memory address to write to.
 * @param value The 16-bit value to write.
 * @return True if the write operation was successful, false otherwise.
 */
bool CServo::servoWriteMemAddrU16(uint8_t addr, uint16_t value)
{
    bool res;

    size_t nBytes = constructMessage(4, SERVO_MEM_WRITE_CMD, addr, (value & 0xff), (value >> 8));

    comServo->readWriteServo(txBuffer, nBytes, rxBuffer, 6);

    if (!rxChecksumOk(6))
    {
        return false;
    }

    return res;
}
// ...


/**
 * Reads data from the specified memory address of the servo.
 *
 * @param addr The memory address to read from.
 * @param buf Pointer to the buffer where the read data will be stored.
 * @param memBytes The number of bytes to read from the memory address.
 * @return `true` if the read operation was successful, `false` otherwise.
 */
bool CServo::servoReadMemAddr(uint8_t addr, uint8_t *buf, size_t memBytes)
{
    size_t nBytes = constructMessage(3, SERVO_MEM_READ_CMD, addr, memBytes);

    comServo->readWriteServo(txBuffer, nBytes, rxBuffer, 6 + memBytes);

    if (!rxChecksumOk(6 + memBytes))
    {
        return false;
    }

    for (size_t i = 0; i < memBytes; i++)
    {
        buf[i] = rxBuffer[5 + i];
    }

    return true;
}



// ...

/**
 * Pings the servo to check if it is responsive.
 *
 * @return true if the servo responds, false otherwise.
 */
bool CServo::servoPing()
{
    return servoCommand(SERVO_PING_CMD);
}

// ...

/**
 * Sets the position of the servo motor.
 *
 * @param angleU16 The desired angle of the servo motor in degrees.
 * @return True if the servo position was set successfully, false otherwise.
 */
bool CServo::servoSetServoPosition(uint16_t angleU16)
{
    return servoWriteMemAddrU16(SERVO_SRAM_GOAL_POSITION_L, angleU16);
}

// ...

/**
 * @brief Sets the speed of the servo.
 *
 * This function sets the speed of the servo to the specified value.
 *
 * @param speedU16 The speed value to set for the servo.
 * @return True if the speed was set successfully, false otherwise.
 */
bool CServo::servoSetSpeed(uint16_t speedU16)
{
    return servoWriteMemAddrU16(SERVO_SRAM_GOAL_SPEED_L, speedU16);
}


/**
 * Reads the voltage of the servo.
 *
 * @return true if the voltage reading was successful, false otherwise.
 */
bool CServo::servoReadVoltage()
{
    bool res;
    uint8_t memBuff[1];

    res = servoReadMemAddr(SERVO_SRAM_PRESENT_VOLTAGE, memBuff, 1);
    if (!res)
        return res;

    s.state.voltageV = memBuff[0];

    return true;
}

/**
 * Reads the current of the servo.
 *
 * @return true if the current reading was successful, false otherwise.
 */
bool CServo::servoReadCurrent()
{
    bool res;
    uint8_t memBuff[2];

    res = servoReadMemAddr(SERVO_SRAM_PRESENT_CURRENT_L, memBuff, 2);
    if (!res)
        return res;

    s.state.currentMa = ((uint16_t)memBuff[1]) << 8 | ((uint16_t)memBuff[0]);

    return true;
}

/**
 * Reads the position of the servo.
 *
 * @return true if the position reading was successful, false otherwise.
 */
bool CServo::servoReadPosition()
{
    bool res;
    uint8_t memBuff[2];

    res = servoReadMemAddr(SERVO_SRAM_PRESENT_POSITION_L, memBuff, 2);
    if (!res)
        return res;

    s.state.position = ((uint16_t)memBuff[1]) << 8 | ((uint16_t)memBuff[0]);

    return true;
}

/**
 * Reads the speed of the servo.
 *
 * @return true if the speed reading was successful, false otherwise.
 */
bool CServo::servoReadSpeed()
{
    bool res;
    uint8_t memBuff[2];

    res = servoReadMemAddr(SERVO_SRAM_PRESENT_SPEED_L, memBuff, 2);
    if (!res)
        return res;

    s.state.speed = ((uint16_t)memBuff[1]) << 8 | ((uint16_t)memBuff[0]);

    return true;
}

/**
 * Reads the load of the servo.
 *
 * @return true if the load reading was successful, false otherwise.
 */
bool CServo::servoReadLoad()
{
    bool res;
    uint8_t memBuff[2];

    res = servoReadMemAddr(SERVO_SRAM_PRESENT_LOAD_L, memBuff, 2);
    if (!res)
        return res;

    s.state.load = ((uint16_t)memBuff[1]) << 8 | ((uint16_t)memBuff[0]);

    return true;
}

uint16_t CServo:: getPosition()
{
    return s.state.position;
}

uint16_t CServo::getSpeed()
{
    return s.state.speed;
}

uint16_t CServo::getLoad()
{
    return s.state.load;
}

uint8_t CServo::getVoltage()
{
    return s.state.voltageV;
}

uint16_t CServo::getCurrent()
{
    return s.state.currentMa;
}

/**
 * Returns the position of the servo in degrees.
 * 
 * In the Waveshare servo reference library, the position is represented as a 16-bit
 * sign-magnitude value. However, in reality, it's a 12-bit unsigned value, which is
 * typically what magnetic encoders output.
 * 
 * @return The position of the servo in degrees.
 */
float CServo::servoGetPositionDegrees()
{
    return (((float)s.state.position) / 4095.0f) * 360.0f;
}
