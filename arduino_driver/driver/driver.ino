/*
 * Driver for a RGB led circuit.
 * Recieves data over serial connection and controls 3 analog outputs according to it.
 * 
 * Leds are controlled with analogWrite (PWM).
 */

int redPin = 6;
int greenPin = 3;
int bluePin = 9;

float maxValue = 250;

int dataLength = 25;
byte data[25];
int dataEnd = 0;

byte ledData[] = {0,0,0};
bool newData = false;

long timeSave;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  timeSave = millis();
}


void loop() {
  newData = false;
  while(Serial.available() > 9){
    Serial.read();
  }
  while(Serial.available() > 0 && dataEnd < dataLength){
      data[dataEnd++] = Serial.read();
      newData = true;
  }
  
  if(newData){
    for(int i = dataEnd - 1; i >= 4; i--){   //go through data array from end towards start
      if(data[i] == 0xff){                   //if byte is end byte
         if(data[i-4] == 0xfe){              //and byte 4 places earlier is a start byte  
          ledData[0] = data[i-3];            //save led data
          ledData[1] = data[i-2];
          ledData[2] = data[i-1];

          int dataLeftEnd = dataEnd - i - 1;  //delete used data
          for(int u = 0; u < dataLeftEnd; u++){ 
            data[u] = data[i + u + 1];
          }
          dataEnd = dataLeftEnd;
          analogWrite(redPin, ledData[0]);
          analogWrite(greenPin, ledData[1]);
          analogWrite(bluePin, ledData[2]);
          break;
        }
      }
    }
  }

  //Send info over serial
  Serial.println("Looptime: "+ String(millis() - timeSave) + "ms");
  timeSave = millis();
  Serial.println("Led status: "+ String(ledData[0]) + "R,"+ String(ledData[1]) + "G,"+ String(ledData[2]) + "B,");
}



