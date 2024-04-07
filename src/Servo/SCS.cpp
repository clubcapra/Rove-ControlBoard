/*
 * SCS.cpp
 * communication layer for serial bus servo
 * date: 2023.6.28
 */

#include <stddef.h>
#include "../include/Servo/SCS.h"
#include "stm32f4xx_hal.h"


SCS::SCS()
{
	Level = 1; // all commands except broadcast command return response
	Error = 0;
}

SCS::SCS(u8 End)
{
	Level = 1;
	this->End = End;
	Error = 0;
}

SCS::SCS(u8 End, u8 Level)
{
	this->Level = Level;
	this->End = End;
	Error = 0;
}


/**
 * @brief Converts host data to SCS data.
 *
 * This function takes the host data and converts it to SCS data format.
 *
 * @param DataL Pointer to the lower byte of the SCS data.
 * @param DataH Pointer to the higher byte of the SCS data.
 * @param Data The host data to be converted.
 */
void SCS::Host2SCS(u8 *DataL, u8* DataH, u16 Data)
{
	if(End){
		*DataL = (Data>>8);
		*DataH = (Data&0xff);
	}else{
		*DataH = (Data>>8);
		*DataL = (Data&0xff);
	}
}


/**
 * Converts the given low and high data bytes to a 16-bit unsigned integer and returns it.
 *
 * @param DataL The low byte of the data.
 * @param DataH The high byte of the data.
 * @return The converted 16-bit unsigned integer.
 */
u16 SCS::SCS2Host(u8 DataL, u8 DataH)
{
	u16 Data;
	if(End){
		Data = DataL;
		Data<<=8;
		Data |= DataH;
	}else{
		Data = DataH;
		Data<<=8;
		Data |= DataL;
	}
	return Data;
}


/**
 * Writes data to the specified memory address of a servo.
 *
 * @param ID The ID of the servo.
 * @param MemAddr The memory address to write the data to.
 * @param nDat Pointer to the data buffer.
 * @param nLen The length of the data buffer.
 * @param Fun The function code.
 */
void SCS::writeBuf(u8 ID, u8 MemAddr, u8 *nDat, u8 nLen, u8 Fun)
{
	u8 msgLen = 2;
	u8 bBuf[6]={0};
	u8 CheckSum = 0;
	bBuf[0] = 0xff;
	bBuf[1] = 0xff;
	bBuf[2] = ID;
	bBuf[4] = Fun;
	if(nDat){
		msgLen += nLen + 1;
		bBuf[3] = msgLen;
		bBuf[5] = MemAddr;
		writeSCS(bBuf, 6);
		
	}else{
		bBuf[3] = msgLen;
		writeSCS(bBuf, 5);
	}
	
	CheckSum = ID + msgLen + Fun + MemAddr;
	u8 i = 0;
	if(nDat){
		for(i=0; i<nLen; i++){
			CheckSum += nDat[i];
		}
		writeSCS(nDat, nLen);
	}
	writeSCS(~CheckSum);
}


/**
 * Writes data to the specified memory address of a servo.
 *
 * @param ID The ID of the servo.
 * @param MemAddr The memory address to write the data to.
 * @param nDat Pointer to the data to be written.
 * @param nLen The length of the data to be written.
 * @return Returns 1 if ack received, otherwise returns 0.
 */
int SCS::genWrite(u8 ID, u8 MemAddr, u8 *nDat, u8 nLen)
{
	rFlushSCS();
	writeBuf(ID, MemAddr, nDat, nLen, INST_WRITE);
	wFlushSCS();
	return Ack(ID);
}



/**
 * Writes data to the specified memory address of a servo register.
 *
 * @param ID The ID of the servo.
 * @param MemAddr The memory address of the servo register.
 * @param nDat Pointer to the data to be written.
 * @param nLen The length of the data to be written.
 * @return Returns 1 if ack received, otherwise returns 0.
 */
int SCS::regWrite(u8 ID, u8 MemAddr, u8 *nDat, u8 nLen)
{
	rFlushSCS();
	writeBuf(ID, MemAddr, nDat, nLen, INST_REG_WRITE);
	wFlushSCS();
	return Ack(ID);
}


/**
 * Writes a register action for the specified ID.
 *
 * @param ID The ID of the register action to write.
 * @return Returns 1 if ack received, otherwise returns 0.
 */
int SCS::RegWriteAction(u8 ID)
{
	rFlushSCS();
	writeBuf(ID, 0, NULL, 0, INST_REG_ACTION);
	wFlushSCS();
	return Ack(ID);
}



/**
 * @brief Writes data to multiple servos simultaneously.
 *
 * This function is used to write data to multiple servos simultaneously using the Sync Write instruction.
 *
 * @param ID An array of servo IDs to write data to.
 * @param IDN The number of servos to write data to.
 * @param MemAddr The memory address to write the data to.
 * @param nDat A pointer to the data to be written.
 * @param nLen The length of the data to be written.
 */
void SCS::syncWrite(u8 ID[], u8 IDN, u8 MemAddr, u8 *nDat, u8 nLen)
{
	//rFlushSCS();
	u8 mesLen = ((nLen+1)*IDN+4);
	u8 Sum = 0;
	u8 bBuf[7]={0};
	bBuf[0] = 0xff;
	bBuf[1] = 0xff;
	bBuf[2] = 0xfe;
	bBuf[3] = mesLen;
	bBuf[4] = INST_SYNC_WRITE;
	bBuf[5] = MemAddr;
	bBuf[6] = nLen;
	writeSCS(bBuf, 7);

	Sum = 0xfe + mesLen + INST_SYNC_WRITE + MemAddr + nLen;
	u8 i, j;
	for(i=0; i<IDN; i++){
		writeSCS(ID[i]);
		writeSCS(nDat+i*nLen, nLen);
		Sum += ID[i];
		for(j=0; j<nLen; j++){
			Sum += nDat[i*nLen+j];
		}
	}
	writeSCS(~Sum);
	wFlushSCS();
}


/**
 * @brief Writes a byte of data to a specific memory address of a servo.
 * 
 * @param ID The ID of the servo.
 * @param MemAddr The memory address to write the data to.
 * @param bDat The byte of data to write.
 * @return Returns 1 if ack received, otherwise returns 0.
 */
int SCS::writeByte(u8 ID, u8 MemAddr, u8 bDat)
{
	rFlushSCS();
	writeBuf(ID, MemAddr, &bDat, 1, INST_WRITE);
	wFlushSCS();
	return Ack(ID);
}


/**
 * Writes a word (16-bit value) to the specified memory address of a servo with the given ID.
 *
 * @param ID The ID of the servo.
 * @param MemAddr The memory address to write the word to.
 * @param wDat The word (16-bit value) to write.
 * @return Returns 1 if ack received, otherwise returns 0.
 */
int SCS::writeWord(u8 ID, u8 MemAddr, u16 wDat)
{
	u8 bBuf[2]={0};
	Host2SCS(bBuf+0, bBuf+1, wDat);
	rFlushSCS();
	writeBuf(ID, MemAddr, bBuf, 2, INST_WRITE);
	wFlushSCS();
	return Ack(ID);
}



/**
 * Reads data from the specified memory address of a servo with the given ID.
 *
 * @param ID The ID of the servo.
 * @param MemAddr The memory address to read from.
 * @param nData Pointer to the buffer where the read data will be stored.
 * @param nLen The number of bytes to read.
 * @return size of data read, 0 if failed.
 */
int SCS::Read(u8 ID, u8 MemAddr, u8 *nData, u8 nLen)
{

	u8 bBuf[24]={0};
	writeBuf(ID, MemAddr, &nLen, 1, INST_READ);
	wFlushSCS();
	
	rFlushSCS();
	int size =readSCS(bBuf, nLen+6)-6;
	if(!checkHead(bBuf)){
		return -1;
	}
	Error = 0;
	/*
	if(readSCS(bBuf, 3)!=3){
		return 0;
	}*/
	if(size!=nLen){
		return 0;
	}
	for(u8 i=0; i<size; i++){
		nData[i] = bBuf[i+5];
	}
	/*
	if(bBuf[5]!=1){
		return 0;
	}*/
	u8 calSum = bBuf[2]+bBuf[3]+bBuf[4];
	/*
	u8 calSum=0;
	for (u8 i = 2; i < nLen; i++)
	{
		calSum += bBuf[i];
	}*/
	
	u8 i;
	for(i=0; i<size; i++){
		calSum += nData[i];
	}
	calSum = ~calSum;
	
	
	if(calSum!=bBuf[(nLen+6)-1]){
		return 0;
	}
	Error = bBuf[4];
	return size;
}


/**
 * Reads a byte from the specified memory address of the servo with the given ID.
 *
 * @param ID The ID of the servo.
 * @param MemAddr The memory address to read from.
 * @return The byte value read from the specified memory address and -1 when error.
 */
int SCS::readByte(u8 ID, u8 MemAddr)
{
	u8 bDat;
	int Size = Read(ID, MemAddr, &bDat, 1);
	if(Size!=1){
		return -1;
	}else{
		return bDat;
	}
}

// read 2 byte from servo, return -1 when timeout

/**
 * Reads a word (2 bytes) from the specified memory address of the servo with the given ID.
 *
 * @param ID The ID of the servo.
 * @param MemAddr The memory address to read from.
 * @return The value read from the memory address in 16 bit.
 */
int SCS::readWord(u8 ID, u8 MemAddr)
{	
	u8 nDat[2];
	int Size;
	u16 wDat;
	Size = Read(ID, MemAddr, nDat, 2);
	if(Size!=2)
		return -1;
	wDat = SCS2Host(nDat[0], nDat[1]);
	return wDat;
}



/**
 * Sends a ping command to the specified servo ID.
 *
 * @param ID The ID of the servo to ping.
 * @return Returns an integer value indicating the status of the ping command.
 */
int	SCS::Ping(u8 ID)
{
	

	rFlushSCS();
	writeBuf(ID, 0, NULL, 0, INST_PING);
	wFlushSCS();
	Error = 0;

	u8 bBuf[6]={0};
	readSCS(bBuf, 6);

	if(!checkHead(bBuf)){
		return -1;
	}
	/*
	u8 bBuf[4];
	if(readSCS(bBuf, 4)!=4){
		return -1;
	}*/
	if(bBuf[2]!=ID && ID!=0xfe){
		return -1;
	}
	if(bBuf[3]!=2){
		return -1;
	}
	u8 calSum = ~(bBuf[2]+bBuf[3]+bBuf[4]);
	if(calSum!=bBuf[5]){
		return -1;			
	}
	Error = bBuf[4];
	return bBuf[2];

	
}

/**
 * @brief Checks the head of the data.
 *
 * This function checks the head of the data represented by the given unsigned character array.
 *
 * @param nDat The unsigned character array representing the data.
 * @return 1 if the head is correct, otherwise 0.
 */
int SCS::checkHead(unsigned char *nDat)
{
	
	if(nDat[0]!=0xff && nDat[1]!=0xff){
		return 0;
	}
	
	return 1;
}

/**
 * @brief Sends an acknowledgment to the specified ID.
 *
 * This function sends an acknowledgment to the specified ID.
 *
 * @param ID The ID to send the acknowledgment to.
 * @return An integer representing the status of the acknowledgment.
 */
int	SCS::Ack(u8 ID)
{
	Error = 0;
	if(ID!=0xfe && Level){ 
		u8 bBuf[6]={0};
		readSCS(bBuf, 6);
		if(!checkHead(bBuf)){
			return 0;
		}
		/*
		if(readSCS(bBuf, 4)!=4){
			return 0;
		}*/
		
		if(bBuf[2]!=ID){
			return 0;
		}
		if(bBuf[3]!=2){
			return 0;
		}
		u8 calSum = ~(bBuf[2]+bBuf[3]+bBuf[4]);
		if(calSum!=bBuf[5]){
			return 0;			
		}
		Error = bBuf[4];
	}
	return 1;
}


/**
 * @brief Sends a synchronous read packet to multiple servo motors.
 * 
 * NOT TESTED
 * 
 * This function sends a synchronous read packet to multiple servo motors
 * specified by the given array of servo IDs. The packet requests the data
 * from the specified memory address with the given length.
 *
 * @param ID An array of servo IDs to send the packet to.
 * @param IDN The number of servo IDs in the array.
 * @param MemAddr The memory address to read from.
 * @param nLen The length of the data to read.
 * @return An integer representing the success or failure of the operation.
 */
int	SCS::syncReadPacketTx(u8 ID[], u8 IDN, u8 MemAddr, u8 nLen)
{
	syncReadRxPacketLen = nLen;
	u8 checkSum = (4+0xfe)+IDN+MemAddr+nLen+INST_SYNC_READ;
	u8 i;
	writeSCS(0xff);
	writeSCS(0xff);
	writeSCS(0xfe);
	writeSCS(IDN+4);
	writeSCS(INST_SYNC_READ);
	writeSCS(MemAddr);
	writeSCS(nLen);
	for(i=0; i<IDN; i++){
		writeSCS(ID[i]);
		checkSum += ID[i];
	}
	checkSum = ~checkSum;
	writeSCS(checkSum);
	return nLen;
}


/**
 * @brief Reads a packet from the specified servo ID using synchronous communication.
 * 
 * NOT TESTED
 * 
 * This function reads a packet from the servo with the given ID using synchronous communication.
 * The received data is stored in the provided buffer `nDat`.
 * 
 * @param ID The ID of the servo to read from.
 * @param nDat Pointer to the buffer where the received data will be stored.
 * 
 * @return An integer value indicating the success or failure of the operation.
 *         A return value of 0 indicates success, while a non-zero value indicates failure.
 */
int SCS::syncReadPacketRx(u8 ID, u8 *nDat)
{
	syncReadRxPacket = nDat;
	syncReadRxPacketIndex = 0;
	u8 bBuf[6]={0};
	readSCS(bBuf, 6);
	if(!checkHead(bBuf)){
		return 0;
	}
	/*
	if(readSCS(bBuf, 3)!=3){
		return 0;
	}*/
	if(bBuf[2]!=ID){
		return 0;
	}
	if(bBuf[3]!=(syncReadRxPacketLen+2)){
		return 0;
	}
	Error = bBuf[4];
	/*
	if(readSCS(nDat, syncReadRxPacketLen)!=syncReadRxPacketLen){
		return 0;
	}*/
	return syncReadRxPacketLen;
}


/**
 * @brief Reads a byte from the received packet during synchronous read operation.
 * 
 * NOT TESTED
 * 
 * This function is used to read a single byte from the received packet during a synchronous read operation.
 * It is typically called within a loop to read all the bytes of the packet.
 * 
 * @return The byte read from the received packet.
 */
int SCS::syncReadRxPacketToByte()
{
	if(syncReadRxPacketIndex>=syncReadRxPacketLen){
		return -1;
	}
	return syncReadRxPacket[syncReadRxPacketIndex++];
}


/**
 * @brief Reads the received packet and converts it to a word value.
 * 
 * NOT TESTED
 * 
 * This function reads the received packet and converts it to a word value.
 * The `negBit` parameter specifies the negative bit value to be used in the conversion.
 * 
 * @param negBit The negative bit value to be used in the conversion.
 * @return The converted word value.
 */
int SCS::syncReadRxPacketToWrod(u8 negBit)
{
	if((syncReadRxPacketIndex+1)>=syncReadRxPacketLen){
		return -1;
	}
	int Word = SCS2Host(syncReadRxPacket[syncReadRxPacketIndex], syncReadRxPacket[syncReadRxPacketIndex+1]);
	syncReadRxPacketIndex += 2;
	if(negBit){
		if(Word&(1<<negBit)){
			Word = -(Word & ~(1<<negBit));
		}
	}
	return Word;
}
