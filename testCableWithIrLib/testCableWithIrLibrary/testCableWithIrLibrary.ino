#include <IRLibAll.h>

IRsend mySender;
IRrecv myReceiver(7);
IRdecode myDecoder; 

void setup() {
  Serial.begin(2000000);
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
}

void loop() {
  if (Serial.read() != -1) {
    //send a code every time a character is received from the serial port
    //Sony DVD power A8BCA
    Serial.println("Invio qualcosa");
    mySender.send(SONY,0xa8bca, 20);
  }
  if (myReceiver.getResults()) {
    Serial.println("Ricevo: ");
    myDecoder.decode();           //Decode it
    myDecoder.dumpResults(true);  //Now print results. Use false for less detail
    myReceiver.enableIRIn();      //Restart receiver
  }
}