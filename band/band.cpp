#include <Arduino.h>
#include <band.h>
#include <OutDevice.h>
#include <Servo.h>

    
    
    band::band(int *ML, int NM, int addr,  int VR, int HR){
       Serial.println("PRIMARY CONSTRUCTOR");
       MotorList = ML;
       NumMotors = NM;
       SourceList[0] = addr;
       SourceList[1] = VR; // VR is Vertical Range
       MotorInterval = HR; // HR is Horizontal Range
       WriteList =  new long double[NM];
       Serial.print("Motor Ports: ");
       for(int k = 0; k < NM; k++){
    	   Serial.print(ML[k]);
    	   Serial.print(" ");
        }
        for(int c = 0; c < NM; c++){
            WriteList[c] = 0.0;
        }
        
        Serial.println("MAKING MOTORS");
        motors = new Servo[NM];
        for(int i = 0; i < NM; i++){
            Serial.print("VALID: ");
            Serial.println(!motors[i].attached());  
        }
      
    }
     band::band(){
           Serial.println("DEFAULT CONSTRUCTOR");
           
    }
     band::~band(){
    	   delete WriteList;
    	   for (int i = 0; i < NumMotors; i++ ) {

    	           delete motors[i];

    	         }
    	   delete motors;
     }
    void band::configureBand(int *ML, int NM, int addr,  int VR, int HR){
         MotorList = ML;
         NumMotors = NM;
         SourceList[0] = addr;
         SourceList[1] = VR;
         MotorInterval = HR;
         WriteList =  new long double[NM];
         motors = new Servo[NM];
  
    
    }

    int band::connectMotors(){ // attaches servos objects to ports
        int cnt = 0;
        for(int i = 0; i < NumMotors; i++){
        	Serial.print("connecting port: ");
        	Serial.println(MotorList[i]);
             motors[i].attach(MotorList[i]);
             if(motors[i].attached()){
                cnt++;
                motors[i].write(0); // forcing common initial state.
                Serial.println(" Valid ");
             }
             else{
            	 Serial.print(" InValid, ");
            	 Serial.println(" Status:  ");
            	 Serial.print(" Connected?  ");
            	 Serial.println(motors[i].attached());
            	 motors[i].attach(MotorList[i]);
            	 Serial.print(" Connected?  ");
            	 Serial.println(motors[i].attached());
            	 Serial.print(" angle?  ");
            	 Serial.println(motors[i].read());
            	 Serial.print(" Changable? (->180)");
            	 motors[i].write(180);
            	 Serial.println(motors[i].read());
            	 
            	 
             }
             
            	 
         }
          
        
          return cnt;
    }
    void band::update(long  double **DataTable){ // selects new values from the associated portion of DataTable and stores them in the band's internal copy
        Serial.println("MADE IT TO UPDATE IN BAND");
        for(int i = 0; i < NumMotors; i++){
          Serial.print("ZEROING OUT POSITION: ");
          Serial.println(i);
          Serial.print("WAS: ");
          Serial.println((double)WriteList[i]);
          WriteList[i] = 0;
          Serial.print("NOW: ");
          Serial.println((double)WriteList[i]);
         
        }
        for(int c = 0; c < NumMotors; c++){
          
            long double local = 0;
            Serial.print("SETTING WRITE LIST POS : ");
            Serial.println(c);
            
            Serial.print("  WITH VALUE: ");
            Serial.println((double)calcRelativeValue(DataTable[(int)SourceList[0]][c])); 
            
            Serial.print("  Derived From: ");
            Serial.println((double)DataTable[(int)SourceList[0]][c]);
            
            local = DataTable[(int)SourceList[0]][c];
            WriteList[c] = calcRelativeValue(local);
            
            Serial.print("Write List Set To  : ");
            Serial.println((double)WriteList[c]);
        }
    
    }

    void band::apply(){ // sets motor positions to values in WriteList, based on matched index values 
      Serial.println("MADE IT TO APPLY IN BAND");
      for(int i = 0; i < NumMotors; i++){
         Serial.print("Setting Motor: ");
         Serial.print(i);
         Serial.print("  To: ");
         Serial.println((double)WriteList[i]);
          motors[i].write((double)WriteList[i]);
          delay(15);
      }
    }
    bool band::valid(){ // returns false if any motor in the set is not bound to a port
      int cnt = 0;
      for(int i = 0; i < NumMotors; i++){
        if(motors[i].attached()){
          cnt++;
          Serial.print("Motor: ");
          Serial.print(i);
          Serial.println(" is active");
        }
        Serial.print("readable? ");
        Serial.println(motors[i].read());
      }
      return cnt == NumMotors;
      
    }

 long double band::calcRelativeValue(long double raw){ // takes the true value of distance on 0 - 180 and scales it so that the values fall on that same range but represent how close the raw number is to the next distance level.
                Serial.println("CALCULATING RELATIVE VALUE ");
                Serial.print("RAW VALUE: ");
                Serial.println((double)raw);
                Serial.print("VERT RANGE SIZE: ");
                Serial.println((double)SourceList[1]);
                Serial.print("INDEX OF THIS BAND: ");
                Serial.println((double)SourceList[0]);
        
                long double localValue = (raw/SourceList[1]) - SourceList[0];
                Serial.print("RATIO: ");
                Serial.println((double)localValue);
                if(localValue > 0){
                    localValue *= 180.0;
                    Serial.print("LOCAL EXPRESSION: ");
                    Serial.println((double)localValue);
                    return localValue;
                }
                return 0;
    }
