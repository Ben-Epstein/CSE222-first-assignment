// Assignment 1 -- CSE 222S
// Josh Gelbard and Ben Epstein

#include <SimbleeBLE.h>

int bluePin = 4;
int greenPin = 3;
int redPin = 2;

int buttonA = 5;
int buttonB = 6;

int buzzerPin = 1;

int storedBlue = 0;
int storedRed = 0;
int storedGreen = 0;

enum State {
  idleState,
  keyState,
  getTimeState,
  setTimeState,
  setBulbState,
  getBulbState,
  setColorState,
  getColorState,
  setFadeState,
  getFadeState,
  sendStatusState,
  fadeState,
  discoState
};

int magicNumber = 0;
int key = 0;
int value1 = 0;
int value2 = 0;
int value3 = 0;
char sendData[4];
boolean lightState = false;
unsigned long onTimerHold = 0;
boolean onTimerState = false;
unsigned long onStartTime = 0;
unsigned long offStartTime = 0;
unsigned long offTimerHold = 0;
boolean offTimerState = false;
boolean setOn = false;
boolean setOff = false;
int fadeTime = 0;

State state = idleState;
State nextState;

int length = 15; // the number of notes

//twinkle twinkle little star from https://gist.github.com/baojie/4522173
char notes[] = "ccggaag ffeeddc ggffeed ggffeed ccggaag ffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void setup() {
  // put your setup code here, to run once:
  SimbleeBLE.advertisementData = "meow";
  //Serial.begin(9600);
  SimbleeBLE.begin();

  pinMode(buttonA, INPUT);
  pinMode(buttonB, INPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (onTimerState == true) {
    if ((millis() / 1000) - onStartTime >= onTimerHold) {
      state = setBulbState;
      value1 = 0x01;
      onTimerState = false;
    }
  }
  if (offTimerState == true) {
    if ((millis() / 1000) - offStartTime >= offTimerHold) {
      state = setBulbState;
      value1 = 0x00;
      offTimerState = false;
    }
  }

  if (digitalRead(buttonA)) {
    state = setBulbState;
    value1 = 0x01;
  }
  else if (digitalRead(buttonB)) {
    state = setBulbState;
    value1 = 0x00;
  }

  switch (state) {
    case idleState:
      if (magicNumber == 0x11) {
        nextState = keyState;
      }
      break;

    case keyState:
      if (key == 0x00) {
        nextState = setBulbState;
      }
      else if (key == 0x01) {
        nextState = getBulbState;
      }
      else if (key == 0x02) {
        nextState = setColorState;
      }
      else if (key == 0x03) {
        nextState = getColorState;
      }
      else if (key == 0x04) {
        nextState = setTimeState;
      }
      else if (key == 0x05) {
        nextState = getTimeState;
      }
      else if (key == 0x06) {
        nextState = setFadeState;
      }
      else if (key == 0x07) {
        nextState = getFadeState;
      }
      else if (key == 0x08) {
        nextState = discoState;
      }
      break;

    case setBulbState:
      if (value1 == 0x00) {
        fade(0, 0, 0);
        storedRed = 0;
        storedGreen = 0;
        storedBlue = 0;
        lightState = false;
        nextState = sendStatusState;
      }
      else if (value1 == 0x01) {
        fade(255, 255, 255);
        storedRed = 255;
        storedGreen = 255;
        storedBlue = 255;
        lightState = true;
        nextState = sendStatusState;
      }
      break;

    case getBulbState:
      sendData[0] = lightState;
      send(sendData, 1);
      nextState = idleState;
      break;

    case setColorState:
      fade(value1, value2, value3);
      if (value1 > 0 || value2 > 0 || value3 > 0) {
        lightState = true;
      }
      else {
        lightState = false;
      }
      storedRed = value1;
      storedGreen = value2;
      storedBlue = value3;
      nextState = sendStatusState;
      magicNumber = 0;
      break;

    case getColorState:
      sendData[0] = storedRed;
      sendData[1] = storedGreen;
      sendData[2] = storedBlue;
      send(sendData, 3);
      nextState = idleState;
      break;

    case setTimeState:
      // Intervals of 10 seconds, program sends as time/10. So 100 seconds is sent as 10
      // Must be greater than 20 seconds (2*10 seconds)
      if (value1 == 0x00) {
        setOn = true;
      }
      else if (value1 == 0x01) {
        setOff = true;
      }
      if (value2 >= 0x02) {
        if (setOn == true) {
          onTimerHold = value2 * 10;
        }// 24 hours = ~85,000
        else if (setOff == true) {
          offTimerHold = value2 * 10;
        }
      }
      if (value3 == 0x01) {
        if (setOn == true) {
          onTimerState = true;
          onStartTime = millis() / 1000;
          setOn = false;
        }
        else if (setOff == true) {
          offTimerState = true;
          offStartTime = millis() / 1000;
          setOff = false;
        }
      }
      magicNumber = 0;
      value1 = 0;
      value2 = 0;
      value3 = 0;
      key = 0;
      nextState = idleState;
      break;

    case getTimeState:
      if (value1 == 0x00) {
        if (!onTimerState) {
          sendData[0] = 0x0F;
          sendData[1] = 0xFF;
          send(sendData, 2);
        }
        else {
          sendData[0] = onTimerHold - ((millis() / 1000) - onStartTime);
          send(sendData, 1);
        }
      }
      else if (value1 == 0x01) {
        if (!offTimerState) {
          sendData[0] = 0x0;
          sendData[1] = 0xFF;
          send(sendData, 2);
        }
        else {
          sendData[0] = offTimerHold - ((millis() / 1000) - offStartTime);
          send(sendData, 1);
        }
      }
      magicNumber = 0;
      break;
    case sendStatusState:
      sendData[0] = lightState;
      sendData[1] = storedRed;
      sendData[2] = storedGreen;
      sendData[3] = storedBlue;
      send(sendData, 4);
      nextState = idleState;
      break;

    case setFadeState:
      if (value1 >= 0x00 && value1 <= 0x0a) {
        fadeTime = value1;
      }
      nextState = idleState;
      break;

    case getFadeState:
      sendData[0] = fadeTime;
      send(sendData, 1);
      nextState = idleState;
      break;

    case discoState:
      disco();
      magicNumber = 0;
      storedRed = 0;
      storedGreen = 0;
      storedBlue = 0;
      nextState = idleState;
      break;
  }
  state = nextState;
}

void SimbleeBLE_onReceive(char *data, int len) {
  magicNumber = data[0];
  key = data[1];
  value1 = data[2];
  value2 = data[3];
  value3 = data[4];
}

void send(char data[], int len) {
  SimbleeBLE.send(data, len);
}

void fade(double endRed, double endGreen, double endBlue) {
  int redColor;
  int greenColor;
  int blueColor;
  int stepDelay = fadeTime * 10;
  int redDiff = abs(storedRed - endRed);
  int greenDiff = abs(storedGreen - endGreen);
  int blueDiff = abs(storedBlue - endBlue);

  if (fadeTime > 0 && !(redDiff == 0 && greenDiff == 0 && blueDiff == 0)) {
    for (int i = 1; i <= 100; i++) {
      redColor = storedRed + ((endRed - storedRed) / 100) * i;
      analogWrite(redPin, redColor);
      greenColor = storedGreen + ((endGreen - storedGreen) / 100) * i;
      analogWrite(greenPin, greenColor);
      blueColor = storedBlue + ((endBlue - storedBlue) / 100) * i;
      analogWrite(bluePin, blueColor);
      delay(stepDelay);
    }
    analogWrite(redPin, endRed);
    analogWrite(greenPin, endGreen);
    analogWrite(bluePin, endBlue);
  }
  else {
    analogWrite(redPin, endRed);
    analogWrite(greenPin, endGreen);
    analogWrite(bluePin, endBlue);
  }
}

void disco() {
  if (value1 == 0x00) { //default
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);
    analogWrite(buzzerPin, 100);
    delay(450);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    analogWrite(buzzerPin, 10);
    delay(700);
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    analogWrite(buzzerPin, 100);
    delay(200);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 127);
    analogWrite(bluePin, 0);
    analogWrite(buzzerPin, 10);
    delay(300);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    analogWrite(buzzerPin, 100);
    delay(400);
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);
    analogWrite(buzzerPin, 10);
    delay(250);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    analogWrite(buzzerPin, 100);
    delay(250);
    analogWrite(buzzerPin, 0);
  }
  else if (value1 == 0x01) {
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
  }
  else if (value1 == 0x02) {
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
  }
  else if (value1 == 0x03){
    for (int i = 0; i < length; i++) {
      if (notes[i] == ' ') {
        delay(beats[i] * tempo); // rest
      } 
      else {
        playNote(notes[i], beats[i] * tempo);
      }
      int randomRed = random(0,255);
      int randomGreen = random(0,255);
      int randomBlue = random(0,255);
      analogWrite(redPin, randomRed);
      analogWrite(greenPin, randomGreen);
      analogWrite(bluePin, randomBlue);
    
      // pause between notes
      delay(tempo / 2); 
    }
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  }
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    analogWrite(buzzerPin, 255);
    delayMicroseconds(tone);
    analogWrite(buzzerPin, 0);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

