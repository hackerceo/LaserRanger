/*
  LaserRangerODC.h - Library for running Hi-AT Laser Rangefinders over UART.
  Created by Nick "HackerCEO" Benik, November 24, 2018.
  Copyright 2018, All Rights Reserved.
*/

#include "Arduino.h"
#include <SoftwareSerial.h>

#ifndef LaserRangerODC_h
#define LaserRangerODC_h

class LaserRangerODC 
{
  public:
    LaserRangerODC(uint8_t rx, uint8_t tx);
    ~LaserRangerODC();
  
	/**
	 * Use serial connection to communicate with the laser module
	 */
	void beginSerial(Stream &serial);
  
    void laserPointerOn();
    void laserPointerOff();
    void findDistanceFast();
    void findDistanceExact();
    void findDistance();
    bool doProcess();
    long getMeasureDistance();
    int getMeasureVariance();
    int getMeasureError();

  private:
    SoftwareSerial LaserPort;
    bool laserState; 
    bool waitingResponse;
    long distance;
    int variance;
    int error;
};

#endif
