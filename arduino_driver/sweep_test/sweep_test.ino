
int redPin = 6;
int greenPin = 3;
int bluePin = 9;

int maxValue = 255;

void setup() {
  // put your setup code here, to run once:
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop() {      
    for(int i = 255; i >= 0; i--){
      analogWrite(bluePin, i);
      delay(10);
    }
    for(int i = 255; i >= 0; i--){
      analogWrite(redPin, i);
      delay(10);
    }
    for(int i = 255; i >= 0; i--){
      analogWrite(greenPin, i);
      delay(10);
    }
    for(int i = 255; i >= 0; i--){
      analogWrite(bluePin, i);
      analogWrite(redPin, i);
      analogWrite(greenPin, i);
      delay(10);
    }
}
