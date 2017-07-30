/*
 * Purpose: 
 * 			Base class for operating and repositioning range finding hardware
 * 				Allows for any number of readers and devices to change their positions.
 * 				Manages the podition of at least one input device along at least one axis.
 * 				Position tracking and manipulation are internally controlled, reversing 
 * 				direction when the motor reaches an edge of the valid range.
 * 				
 * 				
 * 				
 * 				
 * 				
 *
 * Guidelines: 
 * 				ALWAYS:
 * 						- USE startInput TO CONFIGURE HARDWARE USING DATA SET IN THE CONSTRUCTOR
 * 								- USING CONSTRUCTOR FOR BOTH TASKS MAY RESULT IN THE USE OF INCORRECT DATA
 * 							
 * 				PREFERED:
 * 						- SPLIT RANGE OF MOTION INTO INDENTICALLY SIZED REGIONS BASED ON NUMBER OF READS
 * 						- MOVE DIRECTLY BETWEEN READ POINTS
 * 						- DO NOT ALLOW READER TO MOVE OFF RANGE, TRACK POSITION AND FORCE A CHANGE IN DIRECTION AT BOUNDS
 * 				
 * 				CONSIDER:
 * 						- BY DEFAULT, ALL InDevices USE ZERO AS THE START POINT FOR RANGE OF MOTION
 * 								- CHANGING THIS WILL REQUIRE SOME REWORKING OF THE OUTPUT
 * 								  RECCOMENDED METHOD IS TO MAP THE ACTUAL RANGE TO ONE STARTING AT ZERO WHEN RETURING DATA 
 * 						- READERS DO NOT ALL NEED TO BE OF THE SAME TYPE
 * 						- MOTORS DO NOT NEED TO BE OF THE SAME TYPE
 * 						- REPRESENTING ADDITIONAL AXES BY ADDING THEIR LENGTHS TOGETHER IN PrevAngle's OUTPUT
 * 								  
 * 						
 * 					
 * 				
 * 		
 * 		@author Alex Lee
 * 		@version 7.1 7/11/17
 *
 * */

#ifndef InDevice_H
#define InDevice_H

class InDevice {


  public:
    int range; // ARC LENGTH OF MOTION IN DEGREES
    int steps; // NUMBER OF READS TAKEN ALONG RANGE
    int *devPorts; // ARRAY OF PORTS USED BY INPUT DEVICES
    int *motPorts; // ARRAY OF PORTS USED BY POSITIONING MOTORS

    int NDP; // NUMBER OF DEVICE PORTS
    int NMP; // NUMBER OF MOTOR PORTS

    int curAngle; // CURRENT POSITON OF DEVICE ALONG RANGE OF MOTION IN DEGREES
    int curDistance; // RAW RETURN VALUE OF PREVIOUS INPUT 
    boolean bound; // TRUE WHEN curAngle == range FORCES DIRECTION TO CHANGE UNTIL curAngle == 0
    
    
    /*
     * 
     * CONFIGURES THE POSITIONING MOTORS AND INPUT DEVICES BASED ON PORTS GIVEN IN CONSTRUCTOR
     * 
     */
   virtual int StartInput(); 
    /*
     * OPERATES RANGE FINDER AND POSITIONING MOTORS, SETS DIRECTION BASED ON CURRENT POSITION AND THE LAST BOUND REACHED
     * 
     */
   virtual void RunRead(); 
    /*
     * 
     * RETURNS THE RAW VALUE GIVEN BY THE RANGE FINDER DURING THE LAST CALL TO RunRead
     * 
     */
   virtual int prevDistance(); 
    /*
     * 
     * RETURNS THE ANGLE IN DEGREES OF THE INPUT DEVICE DURING THE LAST CALL TO RunRead
     * RESETS THE GLOBAL VALUE TO ZERO AFTER EACH READ
     * 
     */
   virtual int prevAngle(); 
  private:
    /*
     * MANAGES COMMUNICATION WITH THE POSITONING MOTOR, SETS ITS POSITION AS RunRead DICTATES
     */
   virtual void runMotor(); 


};

#endif