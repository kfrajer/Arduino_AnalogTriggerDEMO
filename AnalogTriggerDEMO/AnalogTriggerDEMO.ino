
//REFERENCE: https://forum.processing.org/two/discussion/25252/gui-for-arduino-code/p1

//INSTRUCTIONS:
//* Heart beat runs on pin 13 and it blinks at a defined rate indicating normal operation  
//* Input v to toggle verbose. When verbose is on, every time the selected analog input is
//  above threshold, a data is sent to serial output. It prints "-[ADC val]   0000000000000000000000000000000000000000000000000000000000000000"  
//* Input r to inquire current value of selected anlaog input. It prints  "Threshold: [Thresh val] Current AI value: [ADC val]"  
//* Input a value from 1 to 9 to map threshold from 0 to 1023. It prints "Threshold old: [old thesh] New: [new thresh]"  
//
//* Circuit: LED anode (long leg) into DO. LED's cathode to resistor and then into ground.  
//* Anolog input reads a positive signal. Circuit capacitance can induce a signal on the input. So for proper triggering:
//  Use a toggle switch and then positive terminal--->switch--->*--->resistor--->Ground. Notice you need to do Analog readout at (*) 

const int INCHAN = A4;
const int OUTCHAN = 9;  //Digital output 9
const int HEARTCHAN = 13;

const int HEART_DURATION = 2000;
const int SGNOUT_DURATION = 1000;

unsigned long timeHeart;
int heartBeat = 0;

int verbosity = 0;

unsigned long timerSgn;
int sgnFlag = 0;
int thresh = 48;        // Variable threshold
int incomingByte = 0;   // For incoming serial data

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  resetInputData();

  pinMode(INCHAN, INPUT) ;
  pinMode(OUTCHAN, OUTPUT);
  pinMode(HEARTCHAN, OUTPUT);  // Heart beat
  digitalWrite(OUTCHAN, LOW);

  delay(1000);  //  Hold before running
}

void loop() {

  // Make the LED blink in pin 13 for "heart beat"
  if (millis() - timeHeart > HEART_DURATION) {
    heartBeat = !heartBeat;
    digitalWrite (HEARTCHAN, heartBeat);
    timeHeart = millis();
  }

  if (sgnFlag == HIGH) {
    if (millis() - timerSgn > SGNOUT_DURATION) {
      sgnFlag = LOW;
      digitalWrite(OUTCHAN, LOW);
    }
  }

  int adc  = analogRead(INCHAN) ;    //reading analog voltage and storing it in an integer

  if (adc > thresh) {

    //    Version 1
    //    digitalWrite(OUTCHAN, HIGH) ;
    //    delay(SGNOUT_DURATION);
    //    digitalWrite(OUTCHAN, LOW) ;
    //    delay(20);

    //    Version 2
    timerSgn = millis();
    sgnFlag = HIGH;
    digitalWrite(OUTCHAN, HIGH);


    if (verbosity != 0) {
      Serial.print(-adc);
      Serial.print('\t');
      for (int i = 0; i < 63; i++)
        Serial.print('0');
      Serial.println('0');
    }

  }

  if (incomingByte >= 0) {

    if (incomingByte >= '1' && incomingByte <= '9') {
      updateThreshold();
    }

    //if (incomingByte >= 'a' && incomingByte <= 'z') {

    //Reads current analog value in INCHAN
    if (incomingByte == 'r' || incomingByte == 'R') {
      Serial.print("Threshold: ");
      Serial.print(thresh, DEC);
      Serial.print(" Current AI value: ");
      Serial.println(adc, DEC);
    }

    //Toggles auto-output when above threshold
    if (incomingByte == 'v' || incomingByte == 'V') {
      verbosity = !verbosity;
      Serial.print("Verbose now: ");
      Serial.println(verbosity, DEC);
    }

    resetInputData();
  }
}

void updateThreshold() {

  Serial.print("Threshold old: ");
  Serial.print(thresh, DEC);

  //Serial.println(incomingByte, DEC);
  int val = (incomingByte - '1') + 1; //Char to int, range 1 to 9 inclusive
  thresh = map(val, 1, 9, 0, 1023);

  Serial.print(" New: ");
  Serial.println(thresh, DEC);
}


void serialEvent() {
  //statements
  incomingByte = Serial.read();
}

void resetInputData() {
  incomingByte = -1;
}
