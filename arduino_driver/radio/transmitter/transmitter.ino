#include <RH_ASK.h>
#include <SPI.h>

int dataPin = 10;

RH_ASK driver(2000, 12, dataPin, 14, false);

uint8_t dataBuffer[17];
int dataBufferLength = 17;
bool newData = false;



void setup() {
  Serial.begin(9600);
  if(!driver.init())
    Serial.println("init failed");
}

void loop() {
  newData = false;
  
  //clear outdated useless data. Only last 9+8 bytes matter
  while(Serial.available() > 17){
    Serial.read();
  }
  int newDataLength = Serial.available();
  if(newDataLength > 0){
    memcpy(dataBuffer, &dataBuffer[newDataLength], dataBufferLength - newDataLength); //shift existing data to the beginning of the array
    
    
    for(int i = 0; i < newDataLength; i++){
        dataBuffer[dataBufferLength - newDataLength + i] = Serial.read(); // add new data to array
    }
    newData = true;
  }
  
  if(newData){
    Serial.write(dataBuffer, sizeof(dataBuffer));
    uint8_t endIndex = 255;
    uint8_t startIndex = 255;
    for(int i = dataBufferLength - 1; i >= 0; i--){   //go through data array from end towards start
      if(dataBuffer[i] == 0xff && endIndex == 255){   //if byte is the last end byte
        endIndex = i;
      }
      if(dataBuffer[i] == 0xfe && endIndex != 255){  // if byte is the last start byte before the found end byte
        startIndex = i;
      }
    }
    Serial.write(startIndex);
    Serial.write(endIndex);

    if(endIndex != 255 && startIndex != 255 && endIndex > startIndex){
      //EXECUTE CMD AND SEND DATA
        uint8_t data[endIndex - startIndex - 1];
        for(int i = 0; i < endIndex - startIndex - 1; i++){
          data[i] = dataBuffer[startIndex + 1 + i];
        }
        Serial.write(data, sizeof(data));
        transmitCommand(data);

        
        //Delete used data
        for(int i = endIndex; i >= 0; i--){
          dataBuffer[i] = 0;
        }
    }
  }
}


/*
 * Transmits the given data over RF
 */
void transmitCommand(uint8_t d[]){
  driver.waitPacketSent();
  driver.send(d, sizeof(d));
}




// State variables for executing the command
int cmd = 0;
long startTime = 0;
uint8_t color1[3];
uint8_t color2[3];
uint16_t tdata = 0;

/*
 * inits the given command
 */
void executeCommand(uint8_t cmd, uint8_t d[]){

}


/*
 *   updates the led values according to the command and time spent
 */
void updateCommand(){

}
