/*
 * Purpose: 
 * 			Implements a simple OutDevice based on servos 
 * 					
 * 					Servos are placed in left to right order, this ordering is constant across the motors, WriteList, and DataTable arrays.
 * 					 (See base class OutDevice for more detail)
 * 				
 * 					
 *
 * Guidelines: 
 * 				ALWAYS:
 * 						- KEEP OUTPUT ON [0, 180]
 * 						- AVOID ALTERING CONFIGURATION MID OPERATION
 * 							
 * 				PREFERED:
 * 						- WRAP WITHIN A DATA PARSING/CONTROL CLASS 
 * 						
 * 				CONSIDER:
 * 						- ONLY USE PRIMARY CONSTRUCTOR WHEN CREATING SINGLE BANDS, OTHERWISE USE DEFAULT - configureBand TO ENSURE ALLOCATION 
 * 				
 * 		
 * 		@author Alex Lee
 * 		@version 7.1 7/19/17
 *
 * */

#ifndef band_H
#define band_H

#include <Arduino.h>
#include <OutDevice.h>
#include <Servo.h>

  class band: public OutDevice {  
    
      
    public:
	  Servo *motors; // ARRAY OF LOCAL MOTOR HANDLERS
	  /*
	  	 * 
	  	 * CONSTRUCTS BAND BASED ON GIVEN PARAMETER
	  	 * 
	  	 * @param ML Motor List, the array of motor ports to be used 
	  	 * @param NM Number of Motors, the size of ML
	  	 * @param addr address of the sub array in the master program associated with this output group
	  	 * @param VR Vertical Range, the size of the interval between output groups
	  	 * @param HR Horizontal Range, the size of the interval between motors along the output group 
	  	 * 
	  	 */
      band(int *ML, int NM, int addr,  int VR, int HR);
     
      /*
       * 
       * DEFAULT CONSTRUCTOR, LEAVES ALL PARAMETERS EMPTY, GENERATES NO INTERNAL CONTROL OBJECTS
       * 
       */
      band();
      /*
       * 
       * DESTRUCTOR
       * 
       */
      ~band();
      
      
      /*
       * 
       * ALTERS OPERATIONAL DATA, RECONFIGURES HARDWARE
       * 
       * @param ML Motor List, the array of motor ports to be used 
       * @param NM Number of Motors, the size of ML
       * @param addr address of the sub array in the master program associated with this output group
       * @param VR Vertical Range, the size of the interval between output groups
       * @param HR Horizontal Range, the size of the interval between motors along the output group 
       * 
       */
      void configureBand(int *ML, int NM, int addr,  int VR, int HR);
     
      /*
      	 * 
      	 * LINKS PORTS TO SERVO HANDLERS STORED IN motors ARRAY
      	 * 
      	 */
      int connectMotors();
      
      /*
      	 *
      	 * READS DATA STORED IN THE SUB ARRAY ASSOCIATED WITH THIS BAND (BASED ON ADDRESS GIVEN @ CONFIGURATION) AND INTEGRATES INTO LOCAL CONTEXT
      	 * ALWAYS STORES RESULTS IN THE  WriteList ARRAY
      	 * 		-> ALWAYS RESETS WriteList BEFORE STORING NEW DATA
      	 * 		-> USES calcRelativeValue TO RESCALE GIVEN VALUES TO LOCAL OUTPUT RANGE
      	 * 		-> NEVER DIRECTLY INTERACTS WITH MOTORS
      	 * 		
      	 * @param DataTable pointer to the set of arrays describing current input data set
      	 * 
      	 */
      void update(long  double **DataTable);
      
      /*
       * 
       * PARSES THROUGH WriteList AND SETS MOTOR POSITIONS TO ASSOCIATED VALUES. 
       * ASSOCIATION IS BASED ON INDEX MATCHING BETWEEN motors AND WriteList
       * 
       */
      void apply();
     
      /*
      	 * 
      	 * CHECKS SERVO HANDLERS TO ENSURE THEY ARE ASSOCIATED WITH A PORT. 
      	 * RETURNS TRUE ONLY IF ALL OBJECTS IN motors ARE ATTACHED CORRECTLY 
      	 * 
      	 */
      bool valid();

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
      	 * (USES METHODOLOGY GIVEN IN OutDevice's GUIDELINES)
      	 * 
      	 */
      long double calcRelativeValue(long double raw);




  };

#endif