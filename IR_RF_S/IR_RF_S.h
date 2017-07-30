/*
 * Purpose: 
 * 				Implements a basic InDevice. One motor and input device (Servo and Infrared Range Finder)
 * 					
 * 					
 * 				
 * 				
 * 				
 * 				
 *
 * Guidelines: 
 * 				PREFERED:
 * 							- STORE READ DATA IN THE MASTER DATA TABLE AS IT IS TAKEN
 * 							
 * 				CONSIDER:
 * 							- MEANT FOR USE WITH A SAW TYPE CONTROL SYSTEM, WHERE POSSIBLE USE ITS AUTOMATIC PORT SELECTION
 * 							
 * 		
 * 		@author Alex Lee
 * 		@version 7.1 7/20/17
 *
 * */


#ifndef IR_RF_S_H
#define IR_RF_S_H

#include <Arduino.h>
#include <InDevice.h>
#include <Servo.h>
class IR_RF_S: public InDevice { // INFRARED_RANGEFINDER_SERVO
    
	Servo motor; // MANAGES THE POSITIONING MOTOR 
    int interval; // SIZE OF GAP BETWEEN READ POSITIONS ON RANGE
public:
    /*
     * 
     * CONSTRUCTS A InDevice TYPE OBJECT WITH ONE AXIS OF MOTION AND A SINGLE INFRARED RANGE FINDER
     * USES GIVEN PARAMETERS TO DEFINE OPERATIONAL RANGE AND PORTS OF INTEREST 
     * 
     * @param RAL "Range of Arc Length" the range of motion for the device, given in degrees
     * @param NR  "Number of Reads" the number of points along the range where reads will be taken (evenly spaced)
     * @param DP  "Device Port" analog port of the infrared range finder
     * @param MP  "Motor Port" port to be used by the positioning servo
     * 
     */
	IR_RF_S(int RAL, int NR, int DP,  int MP);
	/*
	 * 
	 * DESTRUCTOR
	 * 
	 */
	 ~IR_RF_S();
	
	/*
	 * SEE BASE CLASS
	 * 
	 */
	int StartInput();
	/*
	 * SEE BASE CLASS
	 * 
	 */
	void RunRead();
	/*
	 * SEE BASE CLASS
	 * 
	 */
	int prevDistance();
	/*
	 * SEE BASE CLASS
	 * 
	 */
	int prevAngle();
private:
	/*
	 * SEE BASE CLASS
	 * 
	 */
     void runMotor();


};


#endif