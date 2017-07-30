/*
 * Purpose: 
 * 			Base class for organizing the interaction between InputDevices and Output Devices. 
 * 				input sources are handled externally to this class, while output devices are treated as internal objects.
 * 				
 * 				Input values may only be provided one at a time, which do not need to be in sequence. 
 * 				This information is mapped to an internal format based on a 180 degree arc.
 * 				Range values are entered into an array based on their explicit value and the (angular) position of the reader 
 * 				when the value is taken. Position in the array dictates which OutDevice and internal motor will respond to the reading
 * 				
 * 				
 * 			
 * 			 
 * 				
 * 					 
 * 				
 * 					
 *
 * Guidelines: 
 * 				ALWAYS:
 * 						- DIVIDE THE INPUT RANGE INTO EQUALLY SIZED SEGEMENTS 
 * 						- INTERNALLY, THE INPUT RANGE MAPS TO 180 DEGREE ARC
 * 						- HORIZONTAL RANGE IS THE ARC LENGTH OF THE INPUT MOTOR'S MOTION DIVIDED BY THE NUMBER OF READS TAKEN
 * 						- RESET TABLE AFTER EVERY WRITE
 * 						-  WHEN CREATING DERIVATIVES OF THIS CLASS, INCLUDE A LOCAL ARRAY OF THE SPECIFIC OUTPUT GROUP TYPE REQUIRED; NO BASIC STORAGE IS GIVEN IN THIS DEFINITION
 * 							
 * 				PREFERED:
 * 						- THE NUMBER OF READS TAKEN SHOULD >= NUMBER OF PHYSICAL MOTORS. 
 * 						- WHERE THE NUMBER OF READS EXCEEDS THE NUMBER OF MOTORS, SOME READINGS WILL OVERLAP
 * 							-> ALL VALUES TARGETED AT  PARTICULAR MOTOR WILL BE AVERAGED TOGETHER 
 * 						- vertSelect and HorzSelect SHOULD USE RANGES OF RESPECTIVE AXES TO FIND THE ASSOCIATED INDEX ON THE MASTER ARRAY 
 * 				CONSIDER:
 * 						-  USING RELATIONSHIP BETWEEN ENTRIES ON THE SAME VERTICAL AXIS ACROSS DIFFERENT BANDS TO DETERMINE TABLE VALIDITY
 * 							-> IF USING A RANGE FINDER ONLY ONE VALUE SHOULD EXIST PER AXIS
 * 						- USING DIFFERENT VALUES OF MAX IN distToDegree WILL ALLOW DIFFERNT INPUT DEVICES/SCALING
 * 							- ONCE MASKED, ALL INPUT VALUES WILL BE PERCENTAGES OF THEIR MAXIMUM VALUE, DISTORTING THE RELATIVE VALUES OF 
 * 								-> INPUT DEVICES WITH DIFFERENT RANGES
 * 			
 * 				
 * 		
 * 		@author Alex Lee
 * 		@version 7.1 7/18/17
 *
 * */



#ifndef DetectLayer_H
#define DetectLayer_H

#include <Arduino.h>
#include<OutDevice.h>
class DetectLayer{
	
	
	
public:
		long double **DataTable; // MASTER DATA TABLE; EACH COLUMN IS WATCHED BY A UNIQUE OutDevice
		double arcLength; // ANGLE MOVED ALONG BY THE INPUT DEVICE 
		int numReads; // NUMBER OF READS MADE AS THE InputDevice MOVES ALONG arcLength
		int numBands; // NUMBER OF OutDevices
		int numSectors; // NUMBER OF PHYSICAL OUTPUT REGIONS PER OutDevice
		int **mAddrList; //SET OF MOTOR ADDRESS ARRAYS, EACH COLUMN PROVIDES PORTS TO A UNIQUE OutDevice. OutDevices IN FARTHEST TO CLOSEST ORDER 				 list of motor sets, farthest to closest band along double D1, right to left along double D2
		long double IV; // INITIAL VALUE OF THE MASTER DATA TABLE. ON RESET THIS VALUE WILL BE USED TO OVERWRITE THE ENTIRE ARRAY
		long double vertRange; // THE SIZE OF THE SECTION OF THE INPUT RANGE EACH OUTPUT GROUP COVERS. CONSISTANT ACROSS OUTPUT GROUPS (180 / NUMBER OF BANDS)
		long double horzRange; // THE SIZE OF THE SECTION OF THE INPUT ARC LENGTH COVERED BY EACH SECTOR ON A GIVEN OUTPUT GROUP  (ARC LENGTH / NUMBER OF SECTORS)
		
		/*
		 * 
		 * CONFIGURES MOTOR CONTROLLERS AND MASTER DATA TABLE. RETURNS THE NUMBER OF OUTPUT GROUPS PROPERLY CREATED
		 * 
		 * @param MAL Motor Address List, a set of motor port lists. each motor set is responsible for a unique segement of the input range. higher index  = closer proximity to user
		 * 
		 */
		virtual int beginDetect( int **MAL);
		
		/*
		 * 
		 * USES THE DISTANCE (IN DEGREES) AND POSITION OF INPUT DEVICE ALONG ITS ARC TO SELECT 
		 * A STORAGE LOCATION IN THE MASTER ARRAY. IF THAT POSITON IS NOT AT THE DEFAULT VALUE, VALUES ARE AVERAGED TOGETHER.
		 * 
		 * @param dist distance (in degrees) reported by InDevice
		 * @param arc positon of InDevice along arc when reading was taken
		 * 
		 */
	    virtual void storeRead(long double dist, long double arc);
	    
	    /*
	     * INSTRUCTS EACH OUTPUT GROUP TO MOVE MOTORS TO THE POSTIONS CURRENTLY STORED IN THE MASTER ARRAY
	     *  
	     */
	    virtual void writeCurrent();
	    
	    /*
	     * 
	     * SETS  ALL VALUES IN MASTER DATA TABLE TO IV
	     * 
	     */
	    virtual void resetTable();
	    
	    /*
	     * 
	     * PRINTS ALL VALUES IN THE MASTER DATA TABLE VIA THE SERIAL MONITOR
	     * 
	     */
	    virtual void displayData();
	    
	    /*
	     * 
	     * CONVERTS THE RAW OUTPUT OF A SENSOR TO THE INTERNAL DEGREE FORMAT USING MAX, THE LARGEST POSSIBLE VALID READ.
	     * RETURNS VALUES [0, 180]
	     * 		IF RAW > MAX: RETURNS 180
	     * 		ELSE IF RAW < 0: RETURNS 0
	     *		ELSE: RETURNS RAW/MAX * 180 
	     * 
	     * @param RAW The current distance read 
	     * @param MAX The largest possible value  
	     * 
	     */
	    virtual long double distToDegree(long double RAW, long double MAX);
	   
	    /*
	     *
	     *CHECKS THE DATA INTEGRITY OF THE TABLE.
	     * RETURNS THE POSITION OF THE FIRST INVALID ENTRY IN THE MASTER TABLE
	     */
	    virtual int validateTable();
	    
private:
	    /*
	     * 
	     * FINDS ROW ADDRESS IN THE MASTER TABLE ASSOCIATED WITH distance.  
	     * RETURNS ROW ADDRESS ASSOCIATED WITH RANGE distance FALLS WITHIN
	     * 
	     * @param distance the distance (in degrees) of a given read
	     * 
	     */
	    virtual int vertSelect(long double distance);
	    
	    /*
	     * 
	     * FINDS COLUMN ADDRESS IN THE MASTER TABLE ASSOCIATED WITH realAngle.  
	     * RETURNS COLUMN ADDRESS ASSOCIATED WITH RANGE realAngle FALLS WITHIN
	     * 
	     * @param realAngle angular position of a given read
	     * 
	     */
	    virtual int horzSelect(long double realAngle);
	  
	    /*
	     * 
	     * PASSES EACH OUTPUT GROUP THE LATEST VERSION OF THE MASTER TABLE
	     * 
	     */
	    virtual void distributeTable();
	
};

#endif