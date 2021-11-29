/*
	FILE: 		ResetDetector.cpp
	VERSION: 	1.0.0
	PURPOSE: 	ESP8266 Library to detect how many resets was made
	LICENCE:	MIT
 */

#include "ResetDetector.h"

ResetDetector::ResetDetector(int timeout, int address) {
	this->timeout = timeout*1000;
	this->address = address;
	waitingForNextReset = false;
#ifdef _ResetDetectorDEBUG_	
	Serial.println("[ResetDetector] Timeout set to: "+String(this->timeout));
	Serial.println("[ResetDetector] Address set to: "+String(this->address));
#endif		
}

int ResetDetector::detectResetCount() {
	
	int retvalue = readResetFlag();
	if (retvalue<0){
		clearResetFlag();
		retvalue=0;
	}else{

#ifdef _ResetDetectorDEBUG_	
	Serial.print("[detectResetCount] Increasing resetcount to:"); Serial.println(String(retvalue+1));
#endif		
		
		setResetFlag(retvalue+1);
	};
	waitingForNextReset = true;
	
	return retvalue+1;
	
}
	
bool ResetDetector::isWaitingForNextReset(){
	return waitingForNextReset;
}



void ResetDetector::waitForDetect() {
	if (waitingForNextReset && millis() > this->timeout) stopDetecting();
}



void ResetDetector::stopDetecting() {
#ifdef _ResetDetectorDEBUG_	
	Serial.println("[stopDetecting] Stop detecting");
#endif	
	clearResetFlag();
	waitingForNextReset = false;	
}


uint32_t ResetDetector::convert2uint32_t(int value){
	String s_int = "0000"+String(value, HEX);
	String t_flag=String(RESETDETECTOR_SIG)+s_int.substring(s_int.length()-4);	
	uint32_t retvalue=(uint32_t)strtoul(t_flag.c_str(),NULL,16);

#ifdef _ResetDetectorDEBUG_	
	Serial.println("[convert2uint32_t] Original ResetDetectorFlag to write: "+String(value));
	Serial.print("[convert2uint32_t] Converted ResetDetectorFlag to write (HEX): "); Serial.println(t_flag.c_str());
	Serial.println("[convert2uint32_t] Converted to uint32_t ResetDetectorFlag to write: "+String(retvalue));	
#endif		

	return retvalue;
}

int ResetDetector::convert2int(uint32_t value){
	String t_flag=String(value,HEX);	
	int retvalue = -1; //function return -1 when can't recognize resetDetector signature
#ifdef _ResetDetectorDEBUG_	
	Serial.println("[convert2int] Converted to hex ResetDetectorFlag: "+t_flag);
	Serial.println("[convert2int] Calculated prefix: "+t_flag.substring(0,4));	
	Serial.println("[convert2int] ResetDetector prefix: "+String(RESETDETECTOR_SIG));	
#endif	

	if (t_flag.substring(0,4) == String(RESETDETECTOR_SIG)){
		retvalue=strtoul(t_flag.substring(t_flag.length()-4).c_str(),NULL,16);
	};
	
#ifdef _ResetDetectorDEBUG_	
	Serial.println("[convert2int] Returning value: "+String(retvalue));
#endif		

	return retvalue;
	
}


int ResetDetector::readResetFlag(){
	
	uint32_t t_uint32;
		
	ESP.rtcUserMemoryRead(address, &t_uint32, sizeof(t_uint32));
	
#ifdef _ResetDetectorDEBUG_	
	Serial.println("[readResetFlag] Read ResetDetectorFlag: "+String(t_uint32,HEX));
#endif	
	
	return convert2int(t_uint32);
	
}



void ResetDetector::setResetFlag(int flag) {
	
	uint32_t t_uint32;
	
#ifdef _ResetDetectorDEBUG_	
	Serial.println("[setResetFlag] ResetDetectorFlag original value: "+String(flag));
#endif	
	t_uint32=convert2uint32_t(flag);
#ifdef _ResetDetectorDEBUG_		
	Serial.println("[setResetFlag] ResetDetectorFlag to write: "+String(t_uint32));
	Serial.println("[setResetFlag] ResetDetectorFlag to write (HEX): "+String(t_uint32,HEX));
#endif	
	ESP.rtcUserMemoryWrite(address, &t_uint32, sizeof(t_uint32));
}

void ResetDetector::clearResetFlag(){

#ifdef _ResetDetectorDEBUG_	
	Serial.println("[clearResetFlag] is calling setResetFlag(0)");

#endif		
	
	setResetFlag(0);
}

// EOF
