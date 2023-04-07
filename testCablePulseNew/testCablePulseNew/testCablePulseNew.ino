int In0;
int oldIn0;
int counter0;
bool xStop;
bool bOn;
int pulseCount;
unsigned long startMillis;
unsigned long currentMillis;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  pinMode(7, INPUT); // Sercos wire couple #1
  pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);
  startMillis = millis();  //initial start time
}

void sendPulses(const uint8_t nPin, const uint8_t nPeriodMillis, const uint8_t nNum)
{
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  //for (uint8_t nI = 0; nI < nNum; nI++)    
    //{
      if (currentMillis - startMillis >= nPeriodMillis){  
        pulseCount++;
        //Serial.println(bOn);
        if (bOn){
          digitalWrite(13, true);
          digitalWrite(nPin, bOn);
          bOn = false;}
        else{
          digitalWrite(13,false);
          digitalWrite(nPin, bOn);
          bOn = true;
        }
        
        
        //bOn = !bOn;
        startMillis = currentMillis;
        //Serial.println(pulseCount++);
      }
    //}
    
}

void receivePulse(){
  In0 = digitalRead(7);
  //Serial.println(In0);
  if ((In0) && !(oldIn0)){
    //Serial.println("Entro qui");
    counter0++;
    //Serial.println(counter0);
  }
  if (!(In0) && (oldIn0)){
    //Serial.println("Entro qui");
    counter0++;
    //Serial.println(counter0);
  }
  oldIn0 = In0;
  
}


void loop() {
  if ((pulseCount >= 2000) && !(xStop)){
    xStop = true;
    Serial.print("Inviati: ");
    Serial.print(pulseCount);
    Serial.println(" impulsi");
    Serial.print("Ricevuti: ");
    Serial.print(counter0);
    Serial.println(" impulsi");
  }
  
  
    // put your main code here, to run repeatedly:
  if (!(xStop)){
    sendPulses(3, 10, 2000);
    //Serial.println(xStop);
  }
  receivePulse();

  
}
