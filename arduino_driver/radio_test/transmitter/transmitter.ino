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
  const uint8_t testData[6] = {1,random(255),random(255),random(255)};
  driver.send(testData, 4);
  driver.waitPacketSent();
  Serial.print("Send array: ");
  Serial.print(testData[1]);
  Serial.print(",");
  Serial.print(testData[2]);
  Serial.print(",");
  Serial.println(testData[3]);
  delay(1000);
}
