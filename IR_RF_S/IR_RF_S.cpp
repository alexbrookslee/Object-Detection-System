#include <Arduino.h>
#include <InDevice.h>
#include <Servo.h>
#include <IR_RF_S.h>

IR_RF_S::IR_RF_S(int RAL, int NR, int DP,  int MP) { 
      range = RAL;
      steps = NR;
      interval = RAL / NR;
      NDP = 1;
      NMP = 1;
      devPorts = new int{DP};
      motPorts = new int{MP};
      curAngle = 0;
      curDistance = 0;
      bound = false;
}

IR_RF_S::~IR_RF_S(){
		delete motor[motPorts[0]]
		delete motor;
		delete devPorts;
		delete motPorts;
		
}

int IR_RF_S::StartInput() {
      //interval = 0;
      Serial.println("Starting Motor");
      motor = Servo();
      motor.attach(motPorts[0]);
      Serial.println("Motors Connected");
      return motor.attached();
}

void IR_RF_S::RunRead() { // manages the motor positoning and data collection of the input device. stores angle and distance in a pair of global variables

      this->runMotor(); 
      curDistance = analogRead(devPorts[0]);
      delay(5);
      if (curAngle <= 0 ) {
        bound = false;
      }
      else if (curAngle >= range) {
        bound = true;
      }

      bound ? curAngle -= interval : curAngle += interval;

}

int IR_RF_S::prevDistance() { // resets distance and give distance of last read
      int local = curDistance;
      curDistance = 0;
      return local;
}

int IR_RF_S::prevAngle() { // gives angle of last read DOES NOT RESET
      
	if(curAngle - interval <= 0){
		return 0;
	}
	return curAngle - interval;
}

void IR_RF_S::runMotor() { // sets motor position and allows appropriate delay
      motor.write(curAngle);
      delay(15);
}

