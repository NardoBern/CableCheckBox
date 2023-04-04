
boolean toggle0 = 0;
boolean toggle1 = 0;
boolean oldToggle1;
boolean toggle2 = 0;
int Val;
int oldInVal;
int Start = 1;
int oldStart = 1;
bool xFailed = false;
bool xStartTest = false;
bool xWait3pulse = false;
int countWaitPulse = 0;
int counter;
int correctPulse = 511; //511 con 8 kHz di timer2 e 10hz di timer1

void setup(){
  Serial.begin(2000000);
  //set pins as outputs
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(0, INPUT);
  pinMode(1, INPUT_PULLUP);

cli();//stop interrupts

//set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  //OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536) 1 Hz
  OCR1A = 1999; // 10Hz
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

//set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  //OCR2A = 199; // = (16*10^6) / (16000*8) - 1  (10khZ)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);


sei();//allow interrupts

}//end setup

ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle0){
    digitalWrite(8,HIGH);
    toggle0 = 0;
  }
  else{
    digitalWrite(8,LOW);
    toggle0 = 1;
  }
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle1){
    toggle1 = 0;
  }
  else{
    toggle1 = 1;
  }
}
  
ISR(TIMER2_COMPA_vect){//timer1 interrupt 8kHz toggles pin 9
//generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle2){
    digitalWrite(9,HIGH);
    toggle2 = 0;
  }
  else{
    digitalWrite(9,LOW);
    toggle2 = 1;
  }
}

void pulseTest(){
Val = digitalRead(0);
  if ((Val) && !(oldInVal)){
    counter++;
  }
  oldInVal = Val;

  if (!(toggle1) && (oldToggle1)){
    
    Serial.println(counter);
    if (counter != correctPulse){//digitalWrite(13, HIGH);
      //cli();
      xFailed = true;
      xStartTest = false;}
    //else{//digitalWrite(13,LOW);}    
    counter = 0;
  }

  if ((toggle1) && !(oldToggle1)){
    Serial.println(counter);
    if (counter != correctPulse){//digitalWrite(13, HIGH);
      //cli();
      xFailed = true;
        xStartTest = false;}
     //else{//digitalWrite(13,LOW);}
    counter = 0;
  }
  oldToggle1 = toggle1;
  
}

void writeOutput(){
  digitalWrite(13, xFailed);
}

void loop(){
  //do other things here
  Start = digitalRead(1);
  if ((Start) && !(oldStart)){
    xStartTest = true;
    xFailed = false;}
  oldStart = Start;
  pulseTest();
  
  writeOutput();

}

