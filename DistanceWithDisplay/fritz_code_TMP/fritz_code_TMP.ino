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

