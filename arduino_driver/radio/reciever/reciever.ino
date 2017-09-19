#include <RH_ASK.h>
#include <SPI.h>

int dataPin = 10;
int redPin = 3;
int greenPin = 6;
int bluePin = 5;

RH_ASK driver(2000, dataPin, 13, 14, false);

/*
 * byte 1, bits 1-4  CHANNEL: The channel the command is ordered to. Execute only if on that channel.
 * byte 1, bits 5-8  COMMAND: The type of the command
 * byte 2 RED VALUE
 * byte 3 GREEN VALUE
 * byte 4 BLUE VALUE
 * byte 5 RED VALUE 2
 * byte 6 GREEN VALUE 2
 * byte 7 BLUE VALUE 2
 * byte 8-9 EXTRA DATA (about time)
 * 
 * COMMAND TYPES:
 * 0: Hold - Holds the color given by bytes (2,3,4) (r,g,b)
 * 1: Sweep - Sweeps from the color given by bytes(2,3,4) to the color (5,6,7) over time span given by (8,9) in unit of 10ms
 * 2: Blink - Blinks between colors (2,3,4) and (5,6,7) with frequency 1/T where T is (8,9) in unit of 10ms
 */

void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  if(!driver.init())
    Serial.println("init failed");


  //BLINK TO KNOW IF WORKING
  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);
  delay(1000);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}

void loop() {
  uint8_t buf[9];
  uint8_t buflen = sizeof(buf);
  if(driver.recv(buf, &buflen)){
    Serial.print("Recieved data length ");
    Serial.println(buflen);
    uint8_t channel = (buf[0] & 0xf0) >> 4;
    uint8_t command = buf[0] & 0x0f;

    Serial.print(channel);
    Serial.print(":");
    Serial.println(command);
    uint8_t red = buf[1];
    uint8_t green = buf[2];
    uint8_t blue = buf[3];
    uint8_t red2 = buf[4];
    uint8_t green2 = buf[5];
    uint8_t blue2 = buf[6];
    uint16_t tdata= buf[7]*256 + buf[8];
      
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
  }
}

