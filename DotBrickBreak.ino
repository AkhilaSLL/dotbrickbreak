// Written by Akhila Liyanage
#include <LedControl.h>

#define PINX A2
#define PINSW 19

// Function Initializers
void StartUp(void);
void Player(void);
void CurrPosition(int pos);
void Shoot(int pos);
void CheckShot(int shotPos);
void Bricks(void);
void CheckWin(void);

// Matrix Display Init
// LedControl(dataPin, clkPin, csPin, numDevices);
LedControl mtrx = LedControl(16, 15, 14, 1);

// Joystick Init
unsigned int x = 0;

// Game Init
int playerPos = 3;          // Player starting position

byte brkRow0 = 0b10101010;  // Bricks starting state
byte brkRow1 = 0b01010101;
byte brkRow2 = 0b10101010;

// Code to run at Arduino bootup
void setup() {
  mtrx.shutdown(0, false);
  mtrx.setIntensity(0,1);
  mtrx.clearDisplay(0);
  pinMode(PINSW, INPUT_PULLUP);
  StartUp();
}

// Startup sequence that tests out all LEDs in dot matrix
void StartUp() {
  for (int i=0;i<4;++i) {
    for (int j=0;j<8;++j) {
      mtrx.setLed(0,i,j,true);
      mtrx.setLed(0,7-i,7-j,true);
      delay(50);
    }
  }
  mtrx.clearDisplay(0);
}

// Code to run continously
void loop() {
  Player();
  Bricks();
  CheckWin();
}

// Updates player based on user input
void Player() {
  CurrPosition(playerPos); 

  x = analogRead(PINX);
  if (x <= 200) {
    --playerPos;
    if (playerPos < 0) playerPos = 0;
    delay(150);
    mtrx.clearDisplay(0);
  } else if (x >= 900) {
    ++playerPos;
    if (playerPos > 7) playerPos = 7;
    delay(150);
    mtrx.clearDisplay(0);
  }

  if (!digitalRead(PINSW)) Shoot(playerPos);
}

// Sets the current position of the player
void CurrPosition(int pos) {
  mtrx.setLed(0,7,pos-1,true);
  mtrx.setLed(0,7,pos,true);
  mtrx.setLed(0,7,pos+1,true);
  mtrx.setLed(0,6,pos,true);
}

// Called when PINSW is pressed to shoot a missile
void Shoot(int pos) {
  for (int i=0;i<6;++i) {
    CurrPosition(pos);
    Bricks();

    mtrx.setLed(0,6-i,pos,true);
    delay(20);
    mtrx.clearDisplay(0);
  }

  CheckShot(pos);
}

// Checks if shot missile has hit a brick
void CheckShot(int shotPos) {
  if (bitRead(brkRow2,7-shotPos)) bitWrite(brkRow2,7-shotPos,0);
  else if (bitRead(brkRow1,7-shotPos)) bitWrite(brkRow1,7-shotPos,0);
  else if (bitRead(brkRow0,7-shotPos)) bitWrite(brkRow0,7-shotPos,0);
}

// Updates the state of the bricks
void Bricks() {
  mtrx.setRow(0,0,brkRow0);
  mtrx.setRow(0,1,brkRow1);
  mtrx.setRow(0,2,brkRow2);
}

// Check if all bricks are destroyed
void CheckWin() {
  if (!(brkRow0 || brkRow1  || brkRow2)) {
    // Show smiley face because you won
    mtrx.clearDisplay(0);
    mtrx.setLed(0,1,2,true);
    mtrx.setLed(0,1,5,true);
    mtrx.setLed(0,4,1,true);
    mtrx.setLed(0,5,2,true);
    mtrx.setLed(0,5,3,true);
    mtrx.setLed(0,5,4,true);
    mtrx.setLed(0,5,5,true);
    mtrx.setLed(0,4,6,true);

    delay(3000);

    // reset the game
    mtrx.clearDisplay(0);
    brkRow0 = 0b10101010;
    brkRow1 = 0b01010101;
    brkRow2 = 0b10101010;
    playerPos = 3;
  }
}
