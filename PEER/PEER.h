/*
 * Purpose: 
 * 				Implements a basic SAW. Based on Infrared detection system and servos.
 * 						Uses a single input device on single axis of motion, allows for any number of
 * 						output groups and sectors/servos   	
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


#ifndef PEER_H
#define PEER_H
#include <Arduino.h>
#include <IR_RF_S.h>
#include <OutDevice.h>
#include <DetectLayer.h>
#include<SAW.h>
#include<band.h>
#include<ObjDetect.h>
#include <InDevice.h>

class PEER: public SAW{
	
public:
	
	/*
	 * 
	 * CONFIGURES SAW BASED OBJECT FOR SINGLE INPUT MODE, SETS FUNCTIONAL DATA AND GENERATES PORTS BASED ON GIVEN DATA
	 * @param AL the Arc Length traversed by the input sensor
	 * @param OGC "Output Group Count" the number of distinct output controllers in use 
	 * @param OSC "Output Sector Count" the number of sectors (output motors) on each controller
	 * @param CV "Conversion Value" Maximum valid read value, maps to 180 degrees, sets conversion ratio
	 * @param RRC "Read Region Count" number of reads taken across the Arc Length
	 * 
	 */
	PEER(int AL, int OGC, int OSC, long double CV, int RRC);
	/*
	 * 
	 * 	USES DATA AND PORTS DEFINED IN THE CONSTRUCTOR TO GENERATE INPUT, OUTPUT AND SERVO SYSTEMS
	 * 	
	 */
	void init(); 
	/*
	 * 
	 * 	USES GIVEN DATA TO GENERATE INPUT, OUTPUT AND SERVO SYSTEMS. ALLOWS FOR ALL SETTINGS TO BE ALTERED
	 * 	NO AUTOMATIC PORT CREATION
	 * 	
	 * 	@param AL the Arc Length traversed by the input sensor
	 * 	@param RRC "Read Region Count" number of reads taken across the Arc Length
	 * 	@param rMotor read motor port array
	 * 	@param RMC "Read Motor Count" the number of reader positioning motors
	 * 	@param rDevice read device port array
	 * 	@param RDC "Read Device Count" number of read devices
	 * 	@param OGC "Output Group Count" the number of distinct output controllers in use 
	 *  @param OSC "Output Sector Count" the number of sectors (output motors) on each controller
	 *  @param UT "User Toggle" user control device port
	 *  @param MinB "Minimum Bound" Largest value allowed before triggering the proximety alert 
	 *  @param CV "Conversion Value" Maximum valid read value, maps to 180 degrees, sets conversion ratio
	 *  
	 */
	void customInit(int AL, int RRC, int *rMotor, int RMC, int *rDevice, int RDC, int OGC, int OSC, int UT,int MinB, long double CV );
	/*
	 * 
	 * RUNS A SINGLE INPUT SWEEP AND OUTPUT UPDATE. BEHAVIOR IS BASED ON THE ProxAlert AND active FIELDS
	 * 		WHEN ProxAlert IS TRUE OR active IS FALSE, NO READS WILL BE TAKEN OR DATA WRITTEN TO OUTPUT GROUPS
	 * 		WHEN ProxAlert IS FALSE AND  active IS TRUE, A  SWEEP BACK AND FORTH ALONG ARC LENGTH WILL OCCUR
	 * 		THIS WILL RESULT IN 3 * #READ REGIONS READS WHICH WILL BE PASSED TO THE DATA LAYER AS THEY ARE MADE
	 * 		  
	 */
	void check(); // runs a single operational cycle
	/*
	 * ACTS AS AN INTRRUPT SERVICE ROUTINE, CALLED BY A HIGHER METHOD ATTATCHED TO THE INTERRUPT HANDLER
	 * AT THE FALLING EDGE OF INPUT FROM UT, reset WILL ZERO OUT THE DATA LAYER, SET ProxAlert TO FALSE, AND NEGATE active
	 *  
	 */
	void reset(); // IRS for the user toggle, resets various control values 
	
	
	
};




#endif