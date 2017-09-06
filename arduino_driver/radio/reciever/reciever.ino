#include <RH_ASK.h>
#include <SPI.h>

int dataPin = 10;
int redPin = 3;
int greenPin = 6;
int bluePin = 9;

RH_ASK driver(2000, dataPin, 13, 14, false);


void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  if(!driver.init())
    Serial.println("init failed");
}

void loop() {
  uint8_t buf[12];
  uint8_t buflen = sizeof(buf);
  if(driver.recv(buf, &buflen)){
    if(buflen == 4) { // message size must be 4 bytes (channel, r, g, b)

      uint8_t channel = buf[0];
      uint8_t red = buf[1];
      uint8_t green = buf[2];
      uint8_t blue = buf[3];
      
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);
    }
  }
}

