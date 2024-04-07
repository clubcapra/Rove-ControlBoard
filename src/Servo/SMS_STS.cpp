/*
 * SMS_STS.cpp
 * application layer for waveshare ST servos
 * date: 2023.6.17 
 */

#include "../include/Servo/SMS_STS.h"

/**
 * @brief Constructs a new instance of the SMS_STS class.
 */
SMS_STS::SMS_STS()
{
	End = 0;
}

/**
 * @brief Constructs a new SMS_STS object.
 *
 * @param End The end value.
 */
SMS_STS::SMS_STS(u8 End):SCSerial(End)
{
}

/**
 * @brief Constructs a new SMS_STS object.
 *
 * @param End The end parameter.
 * @param Level The level parameter.
 */
SMS_STS::SMS_STS(u8 End, u8 Level):SCSerial(End, Level)
{
}



/**
 * @brief Writes the position, speed, and acceleration for a servo motor.
 * 
 * @param ID The ID of the servo motor.
 * @param Position The desired position to set for the servo motor.
 * @param Speed The desired speed to set for the servo motor.
 * @param ACC The desired acceleration to set for the servo motor.
 * @return 1 if the ack was successful, 0 if the ack was unsuccessful.
 */
int SMS_STS::WritePosEx(u8 ID, s16 Position, u16 Speed, u8 ACC)
{
	if(Position<0){
		Position = -Position;
		Position |= (1<<15);
	}
	u8 bBuf[7]={0};
	bBuf[0] = ACC;
	Host2SCS(bBuf+1, bBuf+2, Position);
	Host2SCS(bBuf+3, bBuf+4, 0);
	Host2SCS(bBuf+5, bBuf+6, Speed);
	
	return genWrite(ID, SMS_STS_ACC, bBuf, 7);
}


/**
 * @brief Writes the position, time, and speed values to the specified servo.
 * 
 * ADDED BUT NOT TESTED
 * 
 * @param ID The ID of the servo.
 * @param Position The desired position value.
 * @param Time The desired time value.
 * @param Speed The desired speed value.
 * @return 1 if the ack was successful, 0 if the ack was unsuccessful.
 */
int SMS_STS::WritePos(u8 ID, u16 Position, u16 Time, u16 Speed)
{
	u8 bBuf[6]={0};
	Host2SCS(bBuf+0, bBuf+1, Position);
	Host2SCS(bBuf+2, bBuf+3, Time);
	Host2SCS(bBuf+4, bBuf+5, Speed);
	
	return genWrite(ID, SMS_STS_GOAL_POSITION_L, bBuf, 6);
}


/**
 * @brief Writes the position, speed, and acceleration values to the specified servo motor.
 * 
 * Same as WritePosEx, but the execution time is different and it needs RegWriteAction to execute.
 * 
 * @param ID The ID of the servo motor.
 * @param Position The desired position value to be written.
 * @param Speed The desired speed value to be written.
 * @param ACC The desired acceleration value to be written.
 * @return int Returns 0 if the write operation is successful, otherwise returns an error code.
 */
int SMS_STS::RegWritePosEx(u8 ID, s16 Position, u16 Speed, u8 ACC)
{
	if(Position<0){
		Position = -Position;
		Position |= (1<<15);
	}
	u8 bBuf[7]={0};
	bBuf[0] = ACC;
	Host2SCS(bBuf+1, bBuf+2, Position);
	Host2SCS(bBuf+3, bBuf+4, 0);
	Host2SCS(bBuf+5, bBuf+6, Speed);
	
	return regWrite(ID, SMS_STS_ACC, bBuf, 7);
}


/**
 * @brief Synchronously writes positions, speeds, and accelerations to multiple servo motors.
 *
 * This function allows you to synchronize the position, speed, and acceleration settings of multiple servo motors.
 *
 * @param ID An array of servo motor IDs.
 * @param IDN The number of servo motors to write to.
 * @param Position An array of positions to write to the servo motors.
 * @param Speed An array of speeds to write to the servo motors.
 * @param ACC An array of accelerations to write to the servo motors.
 */
void SMS_STS::SyncWritePosEx(u8 ID[], u8 IDN, s16 Position[], u16 Speed[], u8 ACC[])
{
    u8 offbuf[7*IDN];
    for(u8 i = 0; i<IDN; i++){
		if(Position[i]<0){
			Position[i] = -Position[i];
			Position[i] |= (1<<15);
		}
		u16 V;
		if(Speed){
			V = Speed[i];
		}else{
			V = 0;
		}
		if(ACC){
			offbuf[i*7] = ACC[i];
		}else{
			offbuf[i*7] = 0;
		}
        Host2SCS(offbuf+i*7+1, offbuf+i*7+2, Position[i]);
        Host2SCS(offbuf+i*7+3, offbuf+i*7+4, 0);
        Host2SCS(offbuf+i*7+5, offbuf+i*7+6, V);
    }
    syncWrite(ID, IDN, SMS_STS_ACC, offbuf, 7);
}


/**
 * Sets the wheel mode for a specific ID.
 *
 * @param ID The ID of the servo.
 * @param value The value indicating whether to enable or disable the wheel mode.
 * @return The result of setting the wheel mode.
 */
int SMS_STS::WheelMode(u8 ID,bool value)
{
	return writeByte(ID, SMS_STS_MODE, value);		
}


/**
 * @brief Writes the specified speed and acceleration to the servo with the given ID.
 * 
 * @param ID The ID of the servo.
 * @param Speed The desired speed to set for the servo.
 * @param ACC The desired acceleration to set for the servo.
 * @return Returns 1 if ack received, otherwise returns 0.
 */
int SMS_STS::WriteSpe(u8 ID, s16 Speed, u8 ACC)
{
	if(Speed<0){
		Speed = -Speed;
		Speed |= (1<<15);
	}
	u8 bBuf[2];
	bBuf[0] = ACC;
	genWrite(ID, SMS_STS_ACC, bBuf, 1);
	Host2SCS(bBuf+0, bBuf+1, Speed);
	
	return genWrite(ID, SMS_STS_GOAL_SPEED_L, bBuf, 2);
}


/**
 * Enables or disables torque for a specific ID.
 *
 * @param ID The ID of the servo.
 * @param Enable Flag indicating whether to enable (1) or disable (0) torque.
 * @return Returns 1 if ack received, otherwise returns 0.
 */
int SMS_STS::EnableTorque(u8 ID, u8 Enable)
{
	return writeByte(ID, SMS_STS_TORQUE_ENABLE, Enable);
}


/**
 * @brief Unlocks the EEPROM for writing.
 * 
 * This function unlocks the EEPROM for writing, allowing modifications to be made.
 * 
 * @param ID The ID of the servo.
 * @return int Returns 0 if the EEPROM is successfully unlocked, otherwise returns an error code.
 */
int SMS_STS::unLockEprom(u8 ID)
{
	return writeByte(ID, SMS_STS_LOCK, 0);
}


/**
 * @brief Locks the EEPROM for a specific ID.
 *
 * This function locks the EEPROM for a specific ID, preventing any further modifications to its contents.
 *
 * @param ID The ID of the device to lock the EEPROM for.
 * @return int Returns 0 if the EEPROM is successfully locked, otherwise returns an error code.
 */
int SMS_STS::LockEprom(u8 ID)
{
	return writeByte(ID, SMS_STS_LOCK, 1);
}


/**
 * @brief Set the servo to the middle position.
 * 
 * 
 * @param ID The ID of the servo.
 * @return Returns 1 if ack received, otherwise returns 0.
 */
int SMS_STS::CalibrationOfs(u8 ID)
{
	return writeByte(ID, SMS_STS_TORQUE_ENABLE, 128);
}


/**
 * @brief Retrieves the feedback for a specific ID.
 *
 * This function is used to retrieve the feedback for a specific ID.
 *
 * @param ID The ID of the feedback to retrieve.
 * @return The feedback value for the specified ID.
 */
int SMS_STS::FeedBack(int ID)
{
	int nLen = Read(ID, SMS_STS_PRESENT_POSITION_L, Mem, sizeof(Mem));
	if(nLen!=sizeof(Mem)){
		Err = 1;
		return -1;
	}
	Err = 0;
	return nLen;
}


/**
 * @brief Reads the position of a servo with the specified ID.
 * 
 * if ID is -1, read the position of the servo with the ID stored in Mem.
 * 
 * @param ID The ID of the servo.
 * @return The position of the servo.
 */
int SMS_STS::ReadPos(int ID)
{
	int Pos = -1;
	if(ID==-1){
		Pos = Mem[SMS_STS_PRESENT_POSITION_H-SMS_STS_PRESENT_POSITION_L];
		Pos <<= 8;
		Pos |= Mem[SMS_STS_PRESENT_POSITION_L-SMS_STS_PRESENT_POSITION_L];
	}else{
		Err = 0;
		Pos = readWord(ID, SMS_STS_PRESENT_POSITION_L);
		if(Pos==-1){
			Err = 1;
		}
	}
	if(!Err && (Pos&(1<<15))){
		Pos = -(Pos&~(1<<15));
	}
	
	return Pos;
}


/**
 * @brief Reads the speed of a servo with the specified ID.
 * 
 * If ID is -1, read the speed of the servo with the ID stored in Mem.
 *
 * @param ID The ID of the servo.
 * @return The speed of the servo.
 */
int SMS_STS::ReadSpeed(int ID)
{
	int Speed = -1;
	if(ID==-1){
		Speed = Mem[SMS_STS_PRESENT_SPEED_H-SMS_STS_PRESENT_POSITION_L];
		Speed <<= 8;
		Speed |= Mem[SMS_STS_PRESENT_SPEED_L-SMS_STS_PRESENT_POSITION_L];
	}else{
		Err = 0;
		Speed = readWord(ID, SMS_STS_PRESENT_SPEED_L);
		if(Speed==-1){
			Err = 1;
			return -1;
		}
	}
	if(!Err && (Speed&(1<<15))){
		Speed = -(Speed&~(1<<15));
	}	
	return Speed;
}


/**
 * Reads the load value for a specific ID.
 * 
 * if ID is -1, read the load of the servo with the ID stored in Mem.
 *
 * @param ID The ID of the load to read.
 * @return The load value for the specified ID.
 */
int SMS_STS::ReadLoad(int ID)
{
	int Load = -1;
	if(ID==-1){
		Load = Mem[SMS_STS_PRESENT_LOAD_H-SMS_STS_PRESENT_POSITION_L];
		Load <<= 8;
		Load |= Mem[SMS_STS_PRESENT_LOAD_L-SMS_STS_PRESENT_POSITION_L];
	}else{
		Err = 0;
		Load = readWord(ID, SMS_STS_PRESENT_LOAD_L);
		if(Load==-1){
			Err = 1;
		}
	}
	if(!Err && (Load&(1<<10))){
		Load = -(Load&~(1<<10));
	}
	return Load;
}


/**
 * Reads the voltage for a specific ID.
 * 
 * if ID is -1, read the voltage of the servo with the ID stored in Mem.
 *
 * @param ID The ID of the voltage to read.
 * @return The voltage value for the specified ID.
 */
int SMS_STS::ReadVoltage(int ID)
{	
	int Voltage = -1;
	if(ID==-1){
		Voltage = Mem[SMS_STS_PRESENT_VOLTAGE-SMS_STS_PRESENT_POSITION_L];	
	}else{
		Err = 0;
		Voltage = readByte(ID, SMS_STS_PRESENT_VOLTAGE);
		if(Voltage==-1){
			Err = 1;
		}
	}
	return Voltage;
}


/**
 * @brief Reads the temperature for a specific ID.
 * 
 * This function reads the temperature for the specified ID.
 * if ID is -1, read the temperature of the servo with the ID stored in Mem.
 * 
 * @param ID The ID of the temperature sensor.
 * @return The temperature value read from the sensor.
 */
int SMS_STS::ReadTemper(int ID)
{	
	int Temper = -1;
	if(ID==-1){
		Temper = Mem[SMS_STS_PRESENT_TEMPERATURE-SMS_STS_PRESENT_POSITION_L];	
	}else{
		Err = 0;
		Temper = readByte(ID, SMS_STS_PRESENT_TEMPERATURE);
		if(Temper==-1){
			Err = 1;
		}
	}
	return Temper;
}


/**
 * @brief Reads the move of a servo with the specified ID.
 * 
 * if ID is -1, read the move of the servo with the ID stored in Mem.
 *
 * @param ID The ID of the servo.
 * @return The move value of the servo.
 */
int SMS_STS::ReadMove(int ID)
{
	int Move = -1;
	if(ID==-1){
		Move = Mem[SMS_STS_MOVING-SMS_STS_PRESENT_POSITION_L];	
	}else{
		Err = 0;
		Move = readByte(ID, SMS_STS_MOVING);
		if(Move==-1){
			Err = 1;
		}
	}
	return Move;
}


/**
 * @brief Reads the mode of a specific ID.
 *
 * This function reads the mode of the specified ID and returns the result.
 * if ID is -1, read the mode of the servo with the ID stored in Mem.
 *
 * @param ID The ID of the mode to be read.
 * @return The mode of the specified ID.
 */
int SMS_STS::ReadMode(int ID)
{
	int Mode = -1;
	if(ID==-1){
		Mode = Mem[SMS_STS_MODE-SMS_STS_PRESENT_POSITION_L];	
	}else{
		Err = 0;
		Mode = readByte(ID, SMS_STS_MODE);
		if(Mode==-1){
			Err = 1;
		}
	}
	return Mode;
}


/**
 * @brief Reads the current value for a specific ID.
 *
 * This function reads the current value for the specified ID.
 * if ID is -1, read the current of the servo with the ID stored in Mem.
 *
 * @param ID The ID of the value to read.
 * @return The current value for the specified ID.
 */
int SMS_STS::ReadCurrent(int ID)
{
	int Current = -1;
	if(ID==-1){
		Current = Mem[SMS_STS_PRESENT_CURRENT_H-SMS_STS_PRESENT_POSITION_L];
		Current <<= 8;
		Current |= Mem[SMS_STS_PRESENT_CURRENT_L-SMS_STS_PRESENT_POSITION_L];
	}else{
		Err = 0;
		Current = readWord(ID, SMS_STS_PRESENT_CURRENT_L);
		if(Current==-1){
			Err = 1;
			return -1;
		}
	}
	if(!Err && (Current&(1<<15))){
		Current = -(Current&~(1<<15));
	}	
	return Current;
}

