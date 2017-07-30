#include <Arduino.h>
#include <DetectLayer.h>
#include <ObjDetect.h>
#include <OutDevice.h>
#include <band.h>




ObjDetect::ObjDetect(double AL, int nReads, int nBands, int nSectors) {
	  IV = 0.00;
      arcLength = AL;
      numReads = nReads;
      numBands = nBands;
      numSectors = nSectors;
      vertRange = 180.0 / numBands;
      horzRange = AL / numReads; // directly along double the secondary axis
      DataTable = new long double*[numBands];
      
     

}

ObjDetect::~ObjDetect() {
      for (int i = 0; i < numReads; i++ ) {

        delete DataTable[i];

      }
      delete DataTable;
      for (int c = 0; c < numBands; i++ ) {

              delete outSet[i];

            }
      delete outSet;

}

int ObjDetect::beginDetect( int **MAL) { // returns the number of valid bands created
	int created = 0;
      mAddrList = MAL;
      //IV = 0.00;
      Serial.println("reached begin detect");
      for (int i = 0; i < numSectors; i++ ) {
        Serial.print("read region: ");
        Serial.println(i);
        DataTable[i] = new long double [numSectors];
        for (int c = 0; c < numReads; c++) {
          DataTable[i][c] = IV;
          Serial.print("filling dataTable at: ");
          Serial.println(c);
          Serial.print("Values:(distance) ");
          Serial.print((double)DataTable[i][c]);
        }
      }

      Serial.println("Starting band construction ");
      outSet = new band[numBands];
      for (int j = 0; j < numBands; j++) {
        Serial.print("band ");
        Serial.println(j);
        Serial.print("Motor Ports: ");
        for(int k = 0; k < numSectors; k++){
        	
        	Serial.print(mAddrList[j][k]);
        	Serial.print(" ");	
        	
        }
        Serial.println(" ");	

       
        outSet[j].configureBand(mAddrList[j], numSectors, j, vertRange, horzRange);
        int q = outSet[j].connectMotors();

        if (q == numSectors) {
          created++;
        }
        else{
        	Serial.println(" ");
        	Serial.println("UNCONNECTED");
        	
        }
      
        
      }
      
    
      return created;
}

void ObjDetect::storeRead(long double dist, long double arc) { // takes the distance and arc of a read and stores data in the correct region of the table
      Serial.println("STORING NEW READ");
      int vertPos = vertSelect(dist);
      int horzPos = horzSelect(arc);

      if (DataTable[vertPos][horzPos] != IV) {
        DataTable[vertPos][horzPos] = (dist + DataTable[vertPos][horzPos]) / 2;
      }
      else {
        DataTable[vertPos][horzPos] = dist;
      }
      Serial.print("DISTANCE: ");
      Serial.println((double)DataTable[vertPos][horzPos]);


}

void ObjDetect::writeCurrent() { // allows the current table values to alter motor position
      this->distributeTable();

      for (int col = 0; col < numBands; col++) {
        Serial.print("Running Band ");
        Serial.println(col);

        outSet[col].apply();

      }
      this->resetTable();
}
void ObjDetect::resetTable() { // clears table contents, must be used after each write before new reads can occur
      for (int col = 0; col < numBands; col++) {

        Serial.print(" Resetting along double col ");
        Serial.println(col);
        for (int row = 0; row < numSectors; row++) {

          DataTable[col][row] = IV;

        }
        Serial. println("   ");
      }
}

void ObjDetect::displayData() { // writes out current table of values

      for (int col = 0; col < numBands; col++) {


        for (int row = 0; row < numReads; row++) {
          Serial. print(" ADDRESS:  ");
          Serial. print(row);
          Serial. print(" , ");
          Serial. print(col);
          Serial. println(": ");
          Serial. print((double)DataTable[row][col]);
          Serial.print("  , ");
          Serial.print( " -- ");
          Serial. println("   ");

        }
        Serial. println("   ");
      }
}

long double ObjDetect::distToDegree(long double RAW, long double MAX){ // for use in taking raw values to valid inputs, vald for any unit so long double as both inputs are given in the same form
      // added statement here to filter oversized inputs so that errors in addressing are unlikely to occur
      if (RAW >= MAX) {
        return 180.0;
      }
      else if (RAW < 0) {
        return 0.0;
      }
      return (RAW / MAX) * 180.0;
}

int ObjDetect::validateTable() { // checks that the table has no more than one non zero value per discrete range along double horizontal bands, returns -1 if the table is correct and the band of the first bad read otherwise
      int validator[numSectors];
      for (int v = 0; v < numSectors; v++) {
        validator[v] = 0.0;
      }

      for (int col = 0; col < numBands; col++) {

        Serial.print("along double col ");
        Serial.println(col);
        for (int row = 0; row < numSectors; row++) {
          if (DataTable[col][row] != 0.0) {
            if (validator == 0) {
              validator[row] = row;
            }
            else {
              return row;
            }
          }
        }
        Serial. println("   ");
      }
      return -1;

}
int ObjDetect::vertSelect(long double distance) { // selects the band to write the value to based on distance as degrees

      for (int vrt = 0; vrt < numBands; vrt ++) {

        if (distance - ((vrt + 1) * vertRange) <= 0) {
          return vrt;
        }
      }
      return -1;
    }

int ObjDetect::horzSelect(long double realAngle) { // selects the exact range along double read cone the given read falls under

      for (int hrz = 0; hrz < numReads; hrz++) {

        if (realAngle - ((hrz + 1) * horzRange) <= 0) {
          return hrz;
        }
      }
      return -1;

    }
void ObjDetect::distributeTable() { // feeds new data to each band on device
      Serial.println("MADE IT TO DISTRIBUTE TABLE IN OBJDETECT");
      for (int col = 0; col < numBands; col++) {
        Serial.print("update Band ");
        Serial.println(col);
        outSet[col].update(DataTable);
      }

}
