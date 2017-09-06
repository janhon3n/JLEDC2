#include <RH_ASK.h>
#include <SPI.h>

int dataPin = 10;

RH_ASK driver(2000, 12, dataPin, 14, false);


void setup() {
  Serial.begin(9600);
  if(!driver.init())
    Serial.println("init failed");
}

void loop() {
  const uint8_t testData[6] = {0,100,100,100};
  driver.send(testData, 4);
  driver.waitPacketSent();
  delay(1000);
}
