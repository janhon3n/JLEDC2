#include <SevenSegmentShiftDisplay.h>
#include <OneButton.h>

int channel = 0;

int dataPin = 2;
int clockPin = 14;
int latchPin = 15;

int buttonPin = A1;


SevenSegmentShiftDisplay sssd(dataPin, clockPin, latchPin);
OneButton button(buttonPin, 1);

void setup() {
  button.attachClick(clicked);
  button.attachLongPressStart(longPressed);
  // put your setup code here,to run once:
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(buttonPin, INPUT);
  Serial.begin(9600);

  sssd.SetNumber(channel);
}

void loop() {
  button.tick();
}

void clicked(){
  if(channel != 9){
    channel = channel + 1;
  } else {
    channel = 0;
  }
  sssd.SetNumber(channel);
}

void longPressed(){
  channel = 0;
  sssd.SetNumber(channel);
}


