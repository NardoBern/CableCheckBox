#include <stdint.h>
#include <stdio.h>

int inVal;
int inOldVal;

int Reset;
int oldReset;

bool xTestFail;
bool xTestContFail;
bool xResetReq;

bool xClock;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  pinMode(7, INPUT_PULLUP); // checked input
  pinMode(1, INPUT_PULLUP); // reset

  pinMode(13, OUTPUT); // error output
  pinMode(9, OUTPUT); // clock output
}

void clockGen(){
  if (xClock){
    digitalWrite(9, HIGH);
    xClock = false;
  }
  else{
    digitalWrite(9, LOW);
    xClock = true;
  }
}

void readInput(){
  inVal = digitalRead(7);
  Reset = digitalRead(1);
}

void checkCableInter(){
  if ((inVal) && !(inOldVal) && !(xTestFail)){
    xTestFail = true;
    xResetReq = true;
    Serial.println("Check interruption failed");
  }
  inOldVal = inVal;
}

void checkCableCont(){
  if ((inVal) && !(xTestContFail) && !(xTestFail)){
    xTestContFail = true;
    xResetReq = true;
    Serial.println("Check continuity failed");
  } 
}

void writeOutput(){
  digitalWrite(13, xTestFail || xTestContFail);
}

void reset(){
  if (Reset && !(oldReset) && xResetReq){
    Serial.println("Restart Procedure");
    xResetReq = false;
    xTestFail = false;
    xTestContFail = false;
  }
  oldReset = Reset;
}

void loop() {
  // put your main code here, to run repeatedly:
  clockGen();

  readInput();

  checkCableInter();

  checkCableCont();

  reset();

  writeOutput();

}
