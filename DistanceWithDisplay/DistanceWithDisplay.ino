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

const int trigPin = 9;
const int echoPin = 10;
const int buttonPin = 6;
const float beta = 0.95;
const float margin = 1 + 0.05;
long duration;
float distance;
int buttonState = 0;

float distPush = 0.0;
float distUp = 0.0;

int percentUp = 0;
int percentDown = 0;

long timeUp = 0;
long timeDown = 0;
long lastTime = 0;
long currentTime = 0;


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
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
    printCharAt(6, 0, "*");
    printCharAt(6, 1, " ");
    timeUp += currentTime - lastTime;
  } else {
    printCharAt(6, 0, " ");
    printCharAt(6, 1, "*");
    timeDown += currentTime - lastTime;
   }
   lastTime = currentTime;

  lcd.setCursor(1, 0);
  lcd.print(timeUp/1000);
  
  lcd.setCursor(1, 1); 
  lcd.print(timeDown/1000);

  percentUp = 100 * timeUp / currentTime;
  percentDown = 100 * timeDown / currentTime;
  printPercent(7, 0, percentUp);
  printPercent(7, 1, percentDown);

  delay(300);
 
}

float cumulativeDistance(float newDist, float prevDist, float coef){
  return  newDist*(1-coef) + prevDist*coef;
}

void printCharAt(int col, int row, String symbol){
    lcd.setCursor(col, row);
    lcd.print(symbol);
}

void printDistance(int col, int row, float value){
  lcd.setCursor(col, row);
  lcd.print("      ");
  lcd.setCursor(col, row);
  lcd.print(value);
}

// Print the percentage at given position.
// Assume that the percentage is in the range [0, 100].
// The percentage is given as an integer number to which the percentage sign ("%") is appended.
// The overall length of the percentage string should not exceed 3 characters, therefore if the
// value is 100, the percentage sign is omitted.
void printPercent(int col, int row, int value){
  lcd.setCursor(col, row);
  if (value < 10) {
    lcd.print(" ");  
  }
  lcd.print(value);
  if (value < 100){
    lcd.print("%");
  }
}


