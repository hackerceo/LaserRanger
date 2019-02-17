/*
  LaserRangerODC.h - Library for running Hi-AT Laser Rangefinders over UART.
  Created by Nick "HackerCEO" Benik, November 24, 2018.
  Copyright 2018, All Rights Reserved.
*/

#include "Arduino.h"
#include "LaserRangerODC.h"


// ---------------------------------------
LaserRangerODC::LaserRangerODC(uint8_t rx, uint8_t tx):
  LaserPort(rx, tx)
{
  laserState = false;
  waitingResponse = false;
  distance = 0;
  variance = 0;
  error = 0;
}

// ---------------------------------------
LaserRangerODC::~LaserRangerODC() 
{
  if (laserState) {
    laserPointerOff();
    LaserPort.flush();
  }
}



// ---------------------------------------
void LaserRangerODC::beginSerial(Stream &serial) {
  LaserPort = serial;	
}



// ---------------------------------------
void LaserRangerODC::laserPointerOn() 
{
  // turn laser pointer on
  laserState = true;
  waitingResponse = true;
  LaserPort.write('O');
  LaserPort.flush();
}

// ---------------------------------------
void LaserRangerODC::laserPointerOff() 
{
  // turn laser pointer off
  laserState = false;
  waitingResponse = true;
  LaserPort.write('C');
  LaserPort.flush();
}

// ---------------------------------------
void LaserRangerODC::findDistanceFast() 
{
  // shortest sample time
  waitingResponse = true;
  LaserPort.write('F');
  LaserPort.flush();
}

// ---------------------------------------
void LaserRangerODC::findDistanceExact() 
{  
  // longest sample time
  waitingResponse = true;
  LaserPort.write('M');      
  LaserPort.flush();
}

// ---------------------------------------
void LaserRangerODC::findDistance() 
{
  // dynamically adjusted sample time
  waitingResponse = true;
  LaserPort.write('D');
  LaserPort.flush();
}

// ---------------------------------------
bool LaserRangerODC::doProcess() 
{  
  // this function gets called in the loop function to poll for answer in serial buffer
  if (LaserPort.available()) {
	if (laserState) {
      LaserPort.write('O');
	}
	waitingResponse = false;
    String result = LaserPort.readString();
    // turn the laser back on if its state is on
    if (laserState && !result.startsWith("O,OK!")) {
      laserPointerOn();
    }
//Serial.println(result.c_str());
// Serial.write(String(result.indexOf("m,")).c_str());
    int meterPos = result.indexOf("m,");
    if (meterPos != -1) {
	  // save error magnitude 
	  variance = int(result.substring(meterPos+2, meterPos+6).toInt());
	  // save the distance measure (in millimeters)
	  result = result.substring(meterPos-6, meterPos);
	  result.trim();
	  meterPos = result.indexOf(".");
	  result.remove(meterPos, 1);
	  distance = result.toInt();
	  // not an error
	  error = 0;
	  return true;
    }
  }
  return false;
}

// ---------------------------------------
long LaserRangerODC::getMeasureDistance() 
{
	return distance;
}

// ---------------------------------------
int LaserRangerODC::getMeasureVariance() 
{
	return variance;
}

// ---------------------------------------
int LaserRangerODC::getMeasureError() 
{
	return error;
}

