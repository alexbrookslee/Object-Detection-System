/*
 * Purpose: 
 * 			Base class for controlling a single output group.
 * 					Takes a set of ports, the address of the associated portion of the master data array,
 * 					the size of the interval between groups and between the motors within each.
 * 					Vertical refers to the axis connecting the bands to each other, Horizontal 
 * 					refers to the axis running along each band connecting its motors.
 * 					 
 * 				
 * 					
 *
 * Guidelines: 
 * 				ALWAYS:
 * 						- USE calcRelativeValue TO SCALE INPUT BASED ON VERTICAL BOUNDS. 
 * 						IF INPUT IS X, UPPER BOUND (VERTICAL INTERVAL * BAND POS(ADDR) + VERTICAL INTERVAL) IS Y, AND OUTPUT(RANGE [0, 1]) IS Z:
 * 							AS X -> Y, Z -> 1
 * 						- WHERE GENERATING SETS OF TYPE OutDevice OR ITS DERIVATIVES, USE DEFAULT CONSTRUCTOR AND configureBand TO INITIALIZE  
 * 							
 * 				PREFERED:
 * 						- SEPERATE MOTOR GENERATION AND PORT CONNECTION INTO configureBand (OR THE CONSTRUCTOR) AND  connectMotors
 * 						
 * 				CONSIDER:
 * 						- USING valid TO CONFIRM HARDWARE STATUS NOT DATA FIDELITY 
 * 						- AVERAGING (OR IN SOME WAY CONJOINING)DATA THAT IS WRITTEN TO THE OUTPUT GROUP BEFORE IT HAS BEEN APPLIED/RESET
 * 						- USING THE ADDRESS OF THE ASSOCIATED SUB ARRAY IN THE MASTER DATA TABLE TO SCALE INPUT VALUES 
 * 								IF A IS ADDRESS, X IS INPUT, V IS VERTICAL INTERVAL, S IS SCALED, AND R IS RESULT:
 * 									S = X/V - A
 * 								IF  S < 0, R = 0
 * 								ELSE R = S * MAXIMUM OUTPUT
 * 						 
 * 				
 * 		
 * 		@author Alex Lee
 * 		@version 7.1 7/19/17
 *
 * */


#ifndef OutDevice_H
#define OutDevice_H

class OutDevice{
	
public:
	
	int *MotorList; // MOTOR ADDRESS ARRAY
	int NumMotors; // NUMBER OF MOTORS IN ADDRESS ARRAY
	long  double SourceList[2]; //  STORES ADDRESS OF RELAVANT ARRAY IN MASTER LIST (@0), AND SIZE OF VERTICAL INTERVAL(@1)
	long  double *WriteList; // LOCAL COPY OF RESPONSE DATA
	int MotorInterval; // INTERVAL BETWEEN MOTORS - NOT ALWAYS REQUIRED
	
	
	/*
	 * 
	 * STORES OPERATIONAL DATA, CONFIGURES HARDWARE
	 * 
	 * @param ML Motor List, the array of motor ports to be used 
	 * @param NM Number of Motors, the size of ML
	 * @param addr address of the sub array in the master program associated with this output group
	 * @param VR Vertical Range, the size of the interval between output groups
	 * @param HR Horizontal Range, the size of the interval between motors along the output group 
	 * 
	 */
	virtual void configureBand(int *ML, int NM, int addr,  int VR, int HR);
	
	/*
	 * 
	 * LINKS PORTS TO MOTOR CONTROL OBJECTS. MOTOR OBJECTS STORED IN ARRAY SPECIFIED IN DERIVED CLASS DEFINITION
	 * 
	 */
	virtual int connectMotors();
	
	/*
	 *
	 * READS DATA STORED IN THE SUB ARRAY ASSOCIATED WITH THIS OutDevice (BASED ON ADDRESS GIVEN @ CONFIGURATION) AND INTEGRATES INTO LOCAL CONTEXT
	 * ALWAYS STORES RESULTS IN THE  WriteList ARRAY
	 * 		-> ALWAYS RESETS WriteList BEFORE STORING NEW DATA
	 * 		-> USES calcRelativeValue TO RESCALE GIVEN VALUES TO LOCAL OUTPUT RANGE
	 * 		-> NEVER DIRECTLY INTERACTS WITH MOTORS
	 * 		
	 * @param DataTable pointer to the set of arrays describing current input data set
	 * 
	 */
	virtual void update(long  double **DataTable);
	
	/*
	 * 
	 * PARSES THROUGH WriteList AND SETS MOTOR POSITIONS TO ASSOCIATED VALUES. 
	 * ASSOCIATION IS BASED ON INDEX MATCHING BETWEEN DERIVED MOTOR ARRAY AND WriteList
	 * 
	 */
	virtual void apply();
	
	/*
	 * 
	 * CHECKS DIRECT MOTOR CONTROLERS TO ENSURE THEY ARE ASSOCIATED WITH A PORT. 
	 * RETURNS TRUE ONLY IF ALL MOTORS IN THE SET ARE CONNECTED
	 * 
	 */
	virtual bool valid();

private:
	/*
	 * 
	 * CONVERTS A GIVEN VALUE ON THE RANGE [0, 180] AND MAPS IT TO A AN OUTPUT VALUE BASED ON LOCAL SCALE
	 * EACH OUTPUT GROUP IN A SET WILL BE ASSOCIATED WITH ONE OF A SET OF EQUALLY SIZED PORTIONS OF INPUT RANGE.
	 * MOTOR RESPONSE SHOULD BE BASED ON PROXIMITY TO THE BOUNDS OF THAT OUTPUT GROUP'S REGION, MOVING TOWARDS ITS 
	 * MAXIMUM RESPONSE AS THE UPPER BORDER IS REACHED
	 * 
	 * EXAMPLE: 
	 * 		AN OUTPUT GROUP ASSOCIATED WITH THE INPUT RANGE [0, 60], AND HAS AN OUTPUT RANGE OF [0,180]:
	 * 
	 * INPUT	OUTPUT
	 * 0	->	0
	 * 15	->	45
	 * 30	->	90
	 * 45	->	135
	 * 60	->	180
	 * 
	 * (SEE GUIDELINES FOR RECCOMENDED METHODOLOGY)
	 * 
	 */
	virtual long double calcRelativeValue(long double raw);
	
	
};



#endif