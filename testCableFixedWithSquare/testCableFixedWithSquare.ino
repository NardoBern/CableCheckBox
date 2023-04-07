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
  pinMode(7, INPUT_PULLUP); // checked Sercos wire couple #1
  pinMode(6, INPUT_PULLUP); // checked Sercos wire couple #2
  pinMode(5, INPUT_PULLUP); // checked Sercos wire couple #3
  pinMode(4, INPUT_PULLUP); // checked Sercos wire couple #4
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
  inVal1 = digitalRead(7);
  inVal2 = digitalRead(6);
  inVal3 = digitalRead(5);
  inVal4 = digitalRead(4);
  Reset = digitalRead(1);
}

void checkCableInter(){
  /* Check wire couple #1 */
  if ((inVal1) && !(inOldVal1) && !(xTestFail)){
    xTestFail = true;
    xResetReq = true;
    Serial.println("Check interruption failed");
  }
  inOldVal1 = inVal1;
  /* Check wire couple #2 */
  if ((inVal2) && !(inOldVal2) && !(xTestFail)){
    xTestFail = true;
    xResetReq = true;
    Serial.println("Check interruption failed");
  }
  inOldVal2 = inVal2;
  /* Check wire couple #3 */
  if ((inVal3) && !(inOldVal3) && !(xTestFail)){
    xTestFail = true;
    xResetReq = true;
    Serial.println("Check interruption failed");
  }
  inOldVal3 = inVal3;
  /* Check wire couple #4 */
  if ((inVal4) && !(inOldVal4) && !(xTestFail)){
    xTestFail = true;
    xResetReq = true;
    Serial.println("Check interruption failed");
  }
  inOldVal4 = inVal4;
}

void checkCableCont(){
  if (((inVal1) || (inVal2) || (inVal3) || (inVal4))&& !(xTestContFail) && !(xTestFail)){
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
