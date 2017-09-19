#include <RH_ASK.h>
#include <SPI.h>

int dataPin = 10;
int redPin = 3;
int greenPin = 6;
int bluePin = 5;

RH_ASK driver(2000, dataPin, 13, 14, false);

/*
 * CHANNEL: first 4 bits of the first recieved byte (0-15)
 * COMMAND: 5-8 bits of first byte (0-15)
 * 
 * COMMAND TYPES:
 * 0: Hold - Holds the color given by bytes (2,3,4) (r,g,b)
 * 1: Sweep - Sweeps from the color given by bytes(2,3,4) to the color (5,6,7) over time span given by (8,9)
 */

void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  if(!driver.init())
    Serial.println("init failed");

  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);
  delay(5000);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}

void loop() {
  uint8_t buf[8];
  uint8_t buflen = sizeof(buf);
  if(driver.recv(buf, &buflen)){
    Serial.print("Recieved data length ");
    Serial.println(buflen);
    if(buflen == 4) { // message size must be 4 bytes (channel, r, g, b)
      uint8_t channel = buf[0] & 0xf0;
      uint8_t 
      uint8_t red = buf[1];
      uint8_t green = buf[2];
      uint8_t blue = buf[3];
      
      Serial.print(channel);
      Serial.print(": ");
      Serial.print(red);
      Serial.print(",");
      Serial.print(green);
      Serial.print(",");
      Serial.println(blue);

      
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);
    }
  }
}

