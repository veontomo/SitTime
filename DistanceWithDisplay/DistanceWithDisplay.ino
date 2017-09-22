// push button: https://www.arduino.cc/en/Tutorial/Button
// lcd: https://www.arduino.cc/en/Tutorial/HelloWorld
// autoscroll for lcd: https://www.arduino.cc/en/Tutorial/LiquidCrystalAutoscroll
// distance measurement: https://create.arduino.cc/projecthub/silicon-ripley-10/distance-measurement-using-ultrasonic-sensor-and-arduino-9bacd4


/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/



// include the library code:
#include <LiquidCrystal.h>
#include <math.h>

const int trigPin = 9;
const int echoPin = 10;
const int buttonPin = 6;
const float beta = 0.95;
const float margin = 1 + 0.05;
unsigned long duration;
float distance;
int buttonState = 0;

float distPush = 0.0;
float distUp = 0.0;

unsigned int percentUp = 0;
unsigned int percentDown = 0;

unsigned long timeUp = 0;
unsigned long timeDown = 0;
unsigned long lastTime = 0;
unsigned long currentTime = 0;

const float MAX_DIST_SM = 99.9;

const int scales[6] = {60, 60, 24, 30, 365, 100};
const char names[7] = {'s', 'm', 'h', 'd', 'M', 'y', 'c'};



// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("u");
  lcd.setCursor(0, 1);
  lcd.print("d");
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  currentTime = millis();

   // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.017;
  
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
      distPush = cumulativeDistance(distPush, distance, beta);
      printDistance(11, 1, distPush);
  }
  distUp =  cumulativeDistance(distUp, distance, beta); 
  printDistance(11, 0, distUp);
  
  

  if (distUp > distPush*margin){
    printCharAt(6, 0, '*');
    printCharAt(6, 1, ' ');
    timeUp += currentTime - lastTime;
  } else {
    printCharAt(6, 0, ' ');
    printCharAt(6, 1, '*');
    timeDown += currentTime - lastTime;
   }
   lastTime = currentTime;

  printTime(1, 0, timeUp/1000);
  printTime(1, 1, timeDown/1000); 
  
  percentUp = round(100.0 * timeUp / currentTime);
  percentDown = round(100.0 * timeDown / currentTime);
  printPercent(7, 0, percentUp);
  printPercent(7, 1, percentDown);

  delay(300);
 
}

float cumulativeDistance(float newDist, float prevDist, float coef){
  return  newDist*(1-coef) + prevDist*coef;
}

void printCharAt(int col, int row, char c){
    lcd.setCursor(col, row);
    lcd.print(c);
}

void printDistance(int col, int row, float value){
  const String EMPTY_STRING = "     ";
  lcd.setCursor(col, row);
  lcd.print(EMPTY_STRING);
  lcd.setCursor(col, row);
  
  if (value < 0){
    lcd.print("?");
  } else {
    if(value > MAX_DIST_SM){
      lcd.print(".");
    } else {
      String d = String(value, 1);
      lcd.print(d.substring(0, EMPTY_STRING.length()));
   }
  }
}

// Print the percentage at given position.
// Assume that the percentage is in the range [0, 100].
// The percentage is given as an integer number to which the percentage sign ("%") is appended.
// The overall length of the percentage string should not exceed 3 characters, therefore if the
// value is 100, the percentage sign is omitted.
void printPercent(int col, int row, unsigned int value){
  lcd.setCursor(col, row);
  String result;
  if (value < 10) {
    result = " ";  
  } else {
    result = "";
  }
  result += String(value, DEC);
  if (value < 100){
    result += "%";
  }
  lcd.print(result);
  
}

/// Print a time at given position.
/// The timestamp is given in seconds. 
void printTime(int col, int row, unsigned long seconds){
  const String EMPTY_STRING = "     ";
  lcd.setCursor(col, row);
  lcd.print(EMPTY_STRING);
  lcd.setCursor(col, row);
  lcd.print(secToString(seconds));
}

String secToString(const unsigned int sec){
  int i = 0;
  int rest = 0;
  int value = sec;
  while (value >= scales[i]){
    rest = value % scales[i];
    value = (value - rest) / scales[i];
    i++;
 }
  return String(value, DEC).substring(0, 2) + names[i] + String(rest, DEC).substring(0, 2);
}
