/*
 * Purpose: 
 * 			Implements a basic DetectLayer object. uses band type OutDevice. 
 * 			(See base class for more details
 * 				
 * 					 
 * 				
 * 					
 *
 * Guidelines: 
 * 				ALWAYS:
 * 						- USE THE beginDetect METHOD TO CONFIGURE THE DEVICE DURING INITIAL SETUP
 * 						- CONSTRUCT GLOBALLY, INITAILIZE AT SETUP USING beginDetect
 * 							
 * 				PREFERED:
 * 						- THE NUMBER OF READS TAKEN SHOULD >= NUMBER OF PHYSICAL MOTORS. 
 * 						- WHERE THE NUMBER OF READS EXCEEDS THE NUMBER OF MOTORS, SOME READINGS WILL OVERLAP
 * 							-> ALL VALUES TARGETED AT A PARTICULAR MOTOR WILL BE AVERAGED TOGETHER
 * 						- FOR distToDegree MAX SHOULD BE THE LARGEST ANALOG RETURN OF THE INPUT DEVICE 
 * 				CONSIDER:
 * 						- MULTIPLE ENTRIES ALONG THE SAME VERTICAL AXIS WILL BE FLAGGED AS INVALID BY validateTable
 * 				
 * 		
 * 		@author Alex Lee
 * 		@version 7.1 7/18/17
 *
 * */



#ifndef ObjDetect_H
#define ObjDetect_H

#include <Arduino.h>
#include <DetectLayer.h>
#include <band.h>
#include<OutDevice.h>

class ObjDetect: public DetectLayer {
    
   
   
    
  public:
	 band *outSet; // OUTPUT GROUP ARRAY FOR DERIVED CLASS
	 
	 /*
	  * 
	  * CONSTRUCTOR, SETS OPERATIONAL DATA BASED ON PARAMETERS, ALLOCATES SPACE FOR THE MASTER DATA TABLE
	  * 
	  * @param AL Arc Length, the angle moved across by the InDevice 
	  * @param nReads Number of Reads taken as InDevice traverses AL
	  * @param nBands Number of Bands 
	  * @param nSectors Number of Sectors/motors per band
	  * 
	  */
    ObjDetect(double AL, int nReads, int nBands, int nSectors);
    
    /*
   	  * 
   	  * DESTRUCTOR
   	  */
    ~ObjDetect();
    
    /*
     * SEE BASE CLASS
     */
   int beginDetect(int **MAL);
   
   /*
    * SEE BASE CLASS
    */
    void storeRead(long double dist, long double arc);
    
    /*
     * SEE BASE CLASS
     */
    void writeCurrent();
    /*
     * SEE BASE CLASS
     */
    void resetTable();
    
    /*
     * SEE BASE CLASS
     */
    void displayData();

    /*
     * SEE BASE CLASS
     */
    long double distToDegree(long double RAW, long double MAX);
    
    /*
     * SEE BASE CLASS
     */
    int validateTable();
    
  private:
    
    /*
     * SEE BASE CLASS
     */
    int vertSelect(long double distance);
    
    /*
     * SEE BASE CLASS
     */
    int horzSelect(long double realAngle);
    
    /*
     * SEE BASE CLASS
     */
    void distributeTable();
    
};
#endif