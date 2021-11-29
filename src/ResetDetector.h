/*
	FILE: 		ResetDetector.h
	VERSION: 	1.0.0
	PURPOSE: 	ESP8266 Library to detect how many resets was made
	LICENCE:	MIT
 */

#ifndef ResetDetector_H__
#define ResetDetector_H__

//#define _ResetDetectorDEBUG_

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <stdlib.h>

#define RESETDETECTOR_SIG "d3d3"


class ResetDetector
{
public:
	ResetDetector(int timeout, int address);
	int detectResetCount();
	void waitForDetect();
	void stopDetecting();
	bool isWaitingForNextReset();
	
private:
	unsigned long timeout;
	int address;
	bool waitingForNextReset;
	int readResetFlag();
	void clearResetFlag();
	void setResetFlag(int flag);
	uint32_t convert2uint32_t(int value);
	int convert2int(uint32_t value);
};
#endif // ResetDetector_H__
