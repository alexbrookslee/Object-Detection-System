/*
 * Purpose: 
 * 			Base class for controlling Situational Awareness Wearables(SAW)
 * 				Contains the outline of basic data and methods required to
 * 				connect and run the primary components of the device.
 *
 * Guidelines: 
 * 				ALWAYS:
 * 						- USE INIT TO CONFIGURE HARDWARE AFTER BASIC DATA IS PROVIDED VIA CONSTRUCTOR
 * 						- USE THE RESET METHOD AS A INTERRUPT SERVICE ROUTINE
 * 								- WRAP IN A TOP LEVEL METHOD TO ALLOW THE INTERRUPT HANDLER TO FUNCTION
 * 								- BUNDLE EACH ACTIVE INSTANCE'S ISR IN THE SAME WRAPPER
 * 							
 * 				PREFERED:
 * 						- USE CONSTRUCTOR TO PROVIDE BASIC DATA REQUIRED FOR FUNCTION, NOT TO INITIALIZE HARDWARE
 * 						- PROVIDE A DEFUALT CONSTRUCTOR WITH REASONABLE PARAMETERS 
 * 						- ONLY USE CUSTOMINIT WHEN DEVIATING FROM THE AUTOMATICALLY GENERATED PORT NUMBERS
 * 				CONSIDER:
 * 						- TAKING AT LEAST AS MANY INPUT READS AS THERE ARE OUTPUT SECTORS AND 
 * 								- FEWER READS THAN SECTORS WILL RESULT WASTED WASTED HARDWARE
 * 						- AVERAGE READS THAT OVERLAP ON THE SAME OUTPUT SECTOR 
 * 						- BY DEFAULT, OUTPUT SECTOR PORTS START AT ZERO AND END AT (#SECTORS * #OUTPUTGROUPS) - 1
 * 								- USER TOGGLE AND INPUT POSITIONING DEVICES ARE ASSIGNED SEQUENTIAL PORTS FOLLOWING THESE
 * 								- USE CUSTOMINIT TO CHOOSE DIFFERENT PORTS 
 * 				
 * 		
 * 		@author Alex Lee
 * 		@version 7.1 7/11/17
 *
 * */
 


#ifndef SAW_H
#define SAW_H
#include <Arduino.h>
#include <IR_RF_S.h> // should swap this for InDevice
#include <OutDevice.h>
#include <DetectLayer.h>

class SAW{// Situational Awareness Wearable
public:
	
	double ArcLength; // ANGLE TRAVERSED BY THE INPUT DEVICE GIVEN IN DEGREES 
	int readRegionCount; // NUMBER OF READS TAKEN BY INPUT DEVICE AS IT TRAVERSES ARCLENGTH
	
	int *readMotor; // POINTER TO ARRAY OF PORT NUMBERS FOR DEVICES MOVING THE INPUT PATH
	int readMotorCount; // NUMBER OF DEVICES IN USE
	
	int *readDevice; // POINTER TO ARRAY OF INPUT DEVICE PORTS 
	int readDeviceCount; // NUMBER OF INPUT DEVICES IN USE 
	
	int OutGroupCount; // NUMBER OF OUTPUT DEVICES IN USE 
	int OutSectorCount; // NUMBER OF OUTPUT DEVICES IN EACH GROUP -- CONSTANT ACROSS OUTPUT GROUPS
	int UserToggle; // PORT FOR USER CONRTOL INTERFACE 
	
	int Min; // VALUES CLOSER TO THE USER THAN THIS ACTIVATE THE PROXIMETY ALERT
	long double ConversionValue; // LARGEST VALID READING FOR THE INPUT DEVICE, SETS THE CONVERSION RATIO 

	volatile boolean proxAlert; // SET TO TRUE WHEN ANY SINGLE READ PASSES MIN
	volatile boolean active; // SET TO TRUE WHEN THE DEVICE SHOULD BE READING 

	int ** MADDR; // MOTOR ADDRESS ARRAY
	DetectLayer *ODL; // OBJECT DETECTION LAYER
	InDevice *IND; // INPUT DEVICE 
	//OutDevice *OUD; // OUtput Device - this might be in wrong place
	/*
	 * DEFAULT CONFIGURATION BASED ON CONSTRUCTOR DATA 
	 */
	virtual void init(); 
	/*
	 * ALLOWS NON-STANDARD CONFIGURATION BASED ON GIVEN DATA
	 */
	virtual void customInit(int AL, int ReadRegC, int *rMotor, int ReadMotorC, int *rDevice, int readDevC, int OGC, int OSC, int UT,int Min, long double CV );
	
	/*
	 * RUNS A SINGLE SWEEP, INFORMATION PERSISTS UNTIL RESET
	 */
	virtual void check(); 
	/*
	 * ISR FOR THIS INSTANCE. RESETS FUNCTIONAL DATA AND CONTROL VARIABLES
	 */
	virtual void reset();  
	
	
};

#endif