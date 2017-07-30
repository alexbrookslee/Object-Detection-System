
#include <Arduino.h>
#include <IR_RF_S.h>
#include <OutDevice.h>
#include <DetectLayer.h>
#include<SAW.h>
#include <PEER.h>
#include<band.h>
#include<ObjDetect.h>
#include <InDevice.h>

PEER::PEER(int AL, int OGC, int OSC, long double CV, int RRC){
	
		Min = 150;
		ConversionValue = CV;
		proxAlert = false;
		active = false;
		
		readMotor = {0};
		readMotorCount = 1;
				
		readDevice  =  {0};
		readDeviceCount = 1;
		
		ArcLength = AL;
		readRegionCount = RRC;
		OutGroupCount = OGC;
		OutSectorCount = OSC;
		
		readMotor[0] = OGC * OSC;
		readDevice[0] = A0;
		UserToggle = (OGC * OSC) + 1;
		ConversionValue = CV;
		
		MADDR = new int*[OGC];
		for(int i = 0; i < OGC; i++){
			MADDR[i] = new int[OSC];
			for(int c = 0; c < OSC; c++){
				MADDR[i][c] = (i * OSC) + c;
			}	
		}
		
		
}

void PEER::init(){ // sets up connections, configures objects
		Serial.begin(9600);
		Serial.println("CONSTRUCTING COMPONENTS ");
		pinMode(UserToggle, INPUT_PULLUP);
		//attachInterrupt(digitalPinToInterrupt(UserToggle), &PEER::reset(this), FALLING);
	  
		ODL = new ObjDetect(ArcLength, readRegionCount, OutGroupCount, OutSectorCount);
		IND = new IR_RF_S(ArcLength, readRegionCount, readDevice[0], readMotor[0]);
		Serial.println("DONE");
	  
		Serial.println("CONFIGURING INPUT DEVICE");
		int checkID = IND->StartInput();
		Serial.print("SUCCESS =  ");
		Serial.println(checkID);
		
		Serial.println(" ");
		Serial.println("STARTING OUTPUT DEVICE");  
		int checkOD = ODL->beginDetect(MADDR);
		Serial.print("SUCCESS =  ");
		Serial.println(checkOD);
		
		for (int c = 0; c < OutGroupCount; c++) {

				    for (int r = 0; r < OutSectorCount; r++) {
				      Serial.print(" ");
				      Serial.print(MADDR[c][r]);
				      Serial.print(" ");

				    }
				    Serial.println(" ");
				  }
		
		 
}

void PEER::customInit(int AL, int RRC, int *rMotor, int RMC, int *rDevice, int RDC, int OGC, int OSC, int UT,int MinB, long double CV ){
			
		Min = MinB;
		ConversionValue = CV;
		proxAlert = false;
		active = false;
			
		ArcLength = AL;
		readRegionCount = RRC;
		OutGroupCount = OGC;
		OutSectorCount = OSC;
			
		readMotor = rMotor;
		readMotorCount = RMC;
		readDevice = rDevice;
		readDeviceCount = RDC;
		UserToggle = UT;
		
			
		MADDR = new int*[OGC];
		for(int i = 0; i < OGC; i++){
			MADDR[i] = new int[OSC];
			for(int c = 0; c < OSC; c++){
				MADDR[i][c] = (i * OSC) + c;
			}	
		}
		
		this->init();
}

void PEER::check(){
	if (!proxAlert) {

	    Serial.println(" ---------------------  ");
	    Serial.println(" ---------------------  ");
	    Serial.println("STANDARD INPUT AQUISITION:  ");

	    for (int i = 0; i <= ((readRegionCount) * 3); i ++) {
	      if (!active) {
	        Serial.println(" ---------------------  ");
	        Serial.println(" INACTIVE  ");
	        Serial.println(" ---------------------  ");
	        break;
	      }
	      Serial.println("RUNNING READ");
	      IND->RunRead();

	      Serial.print("HIT MAX: ");
	      Serial.println(IND->bound);

	      int temp = IND->prevDistance();// erases as read
	      Serial.print("Distance (RAW): ");
	      Serial.println(temp);

	      if (ODL->distToDegree(temp, ConversionValue) > Min ) { // distance on any axis closer than ~3 cm will cut out of loop.
	        Serial.println(" Hit Minimum Distance");
	        proxAlert = true;

	      }

	      Serial.print("Angle: ");
	      Serial.println(IND->prevAngle());

	      Serial.println("Storing Read");
	      ODL->storeRead(ODL->distToDegree(temp, ConversionValue), IND->prevAngle());

	    }


	  }
	  else {
	    for (int i = 0; i < 10; i++) {
	      Serial.println("~~~~~~~~~");
	      Serial.println("PROX");
	    }

	    active = false;
	  }

	  Serial.println(" DISPLAYING FINAL DATA  ");
	  ODL->displayData();

	  Serial.println("///////");
	  Serial.println("///////");
	  Serial.println(" WRITING FINAL DATA");
	  ODL->writeCurrent();

	
}
void PEER::reset(){
	  for (int i = 0; i < 10; i++) {
			Serial.println("~~~~~~~~~");
	  }
	  active = !(active);
	  proxAlert = false;
	  Serial.print("CHANGING STATE TO: ");
	  Serial.println(active);
	  Serial.println(" RESETTING SYSTEM  ");
	  ODL->resetTable();
	  Serial.println(" DISPLAYING INITIAL DATA  ");
	  ODL->displayData();
}