// Assignment 1 -- CSE 222S
// Josh Gelbard and Ben Epstein

#include <SimbleeBLE.h>

int bluePin = 4;
int greenPin = 3;
int redPin = 2;


void setup() {
  SimbleeBLE.advertisementData = "meow";
  Serial.begin(9600);
  SimbleeBLE.begin();

  pinMode(buttonA, INPUT);
  pinMode(buttonB, INPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
}

void loop() {
  case disco:
  if(value1 == 0x00){ //default
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);
    delay(450);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255); 
    delay(700);   
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    delay(200);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 127);
    analogWrite(bluePin, 0);
    delay(300);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    delay(400);
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);
    delay(250);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    nextState = idleState;
    break;
  }
  else if (value1==0x01){
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    delay(750);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0); 
    delay(600);   
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 80);
    delay(500);
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    delay(100);
    analogWrite(redPin, 255);
    analogWrite(greenPin, 127);
    analogWrite(bluePin, 0);
    delay(800);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    delay(250);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    nextState = idleState;
    break;
  }
   else if (value1==0x02){
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    delay(750);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255); 
    delay(600);   
    analogWrite(redPin, 127);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 80);
    delay(500);
    analogWrite(redPin, 255);
    analogWrite(greenPin, 80);
    analogWrite(bluePin, 0);
    delay(100);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 127);
    analogWrite(bluePin, 0);
    delay(800);
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    delay(250);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    delay(500);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    delay(350);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    nextState = idleState;
    break;

}
