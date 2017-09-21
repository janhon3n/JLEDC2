#include <SevenSegmentShiftDisplay.h>
#include <OneButton.h>

int channel = 0;

int redPin = 3;
int greenPin = 6;
int bluePin = 5;

int dataPin = 2;
int clockPin = 14;
int latchPin = 15;

int buttonPin = A1;

uint8_t red;
uint8_t green;
uint8_t blue;


SevenSegmentShiftDisplay sssd(dataPin, clockPin, latchPin);
OneButton button(buttonPin, 1);

void setup() {
  button.attachClick(clicked);
  button.attachLongPressStart(longPressed);
  
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(buttonPin, INPUT);
  Serial.begin(9600);

  sssd.SetNumber(channel);
}

void loop() {
  button.tick();
  updateCommand();
}


//Button events
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




/* led commands functionality */
uint64_t _startTime;
int _currentCommand = -1;
int _cmd = -1;
uint8_t _fullHueCircles;

uint8_t _r1, _g1, _b1, _r2, _g2, _b2;
double _h1, _s1, _l1, _h2, _s2, _l2;

uint16_t _timeData;
bool _color1set = true;

void hold(uint8_t red, uint8_t green, uint8_t blue){
  _startTime = millis();
  _cmd = 0;
  _setColor(red, green, blue);
}

void sweep(uint8_t red, uint8_t green, uint8_t blue, uint8_t red2, uint8_t green2, uint8_t blue2, uint16_t timeSpan, int8_t fullHueCircles){
  _startTime = millis();
  _fullHueCircles = fullHueCircles;
  _timeData = timeSpan;
  _cmd = 1;
  _saveColors(red, green, blue, red2, green2, blue2);
  
  rgb2hsl(red, green, blue, _h1, _s1, _l2);
  rgb2hsl(red2, green2, blue2, _h2, _s2, _l2);
  
  _setColor(red, green, blue);
}

void blink(uint8_t red, uint8_t green, uint8_t blue, uint8_t red2, uint8_t green2, uint8_t blue2, uint16_t periodTime){
  _startTime = millis();
  _saveColors(red,green,blue,red2,green2,blue2);
  _timeData = periodTime;
  _cmd = 2;
  _setColor(red, green, blue);
  _color1set = true;
}


void updateCommand(){
  switch(_cmd){
    case 0:
      break;
    case 1:
      if(millis() < _startTime + _timeData){
        double deltaH = _h2 - _h1;
        if(deltaH > 0.5 || deltaH < -0.5){
          deltaH = deltaH * -1;
        }
        deltaH += _fullHueCircles;
        double deltaS = _s2 - _s1;
        double deltaL = _l2 - _l1;
        double timePassed = (millis() - _startTime);
        double H = _h1 + deltaH * (timePassed / _timeData);
        double S = _s1 + deltaS * (timePassed / _timeData);
        double L = _l1 + deltaL * (timePassed / _timeData);
        hsl2rgb(H, S, L, red, green, blue);
        _setColor(red, green, blue);
      }
      break;
    case 2:
      if(millis() > _startTime + _timeData){
        _startTime += _timeData;
        _toggleColor();
      }
      break;
  }
}

void _toggleColor(){
    if(_color1set){
      _setColor(_r2,_g2,_b2);
    } else {
      _setColor(_r1,_g1,_b1);
    }
    _color1set = !_color1set;
}

void _setColor(uint8_t r, uint8_t g, uint8_t b){
  red = r;
  green = g;
  blue = b;
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void _saveColors(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2){
  _r1 = r;
  _g1 = g;
  _b1 = b;
  _r2 = r2;
  _g2 = g2;
  _b2 = b2;
}









/* HSL TO RGB CONVERSIONS */

void rgb2hsl(uint8_t re, uint8_t gr, uint8_t bl, double &h, double &s, double &l)
{
    double min, max, delta;
    double r = (double)re / 255;
    double g = (double)gr / 255;
    double b = (double)bl / 255;
  
    min = r < g ? r : g;
    min = min  < b ? min  : b;

    max = r > g ? r : g;
    max = max  > b ? max  : b;

    h = (max+min)/2.0;
    s = (max+min)/2.0;
    l = (max+min)/2.0;

  if(max == min){
    h = 0;
    s = 0;
  } else {
    double d = max - min;
    s = l > 0.5 ? d / (2.0 - max - min) : d / (max + min);
    if(max == r){
      h = (g - b) / d + (g < b ? 6.0 : 0);
    } else if(max == g){
      g: h = (b - r) / d + 2.0;
    } else {
      b: h = (r - g) / d + 4.0;
    }
    h /= 6.0;
  }
}


void hsl2rgb(double h, double s, double l, uint8_t &r, uint8_t &g, uint8_t &b)
{
  double rr;
  double gg;
  double bb;

  while(h > 1){
    h -= 1;
  }
  while(h < -1){
    h += 1;
  }

  if(s == 0){
    rr = l;
    gg = l;
    bb = l;
  } else {
    double q;
    if(l < 0.5){
      q = l * (1 + s);
    } else {
      q = l + s - l * s;
    }
    double p = 2.0 * l - q;
    rr = hue2rgb(p, q, h + 1.0/3.0);
    gg = hue2rgb(p, q, h);
    bb = hue2rgb(p, q, h - 1.0/3.0);
  }
    
  r = round(rr*255);
  g = round(gg*255);
  b = round(bb*255);
}


double hue2rgb(double p, double q, double t){
    if(t < 0) t += 1.0;
    if(t > 1.0) t -= 1.0;
    if(t < 1.0/6.0) return p + (q - p) * 6.0 * t;
    if(t < 1.0/2.0) return q;
    if(t < 2.0/3.0) return p + (q - p) * (2.0/3.0 - t) * 6.0;
    return p;
}

