# Arduino_AnalogTriggerDEMO

## INSTRUCTIONS:  

* Heart beat runs on pin 13 and it blinks at a defined rate indicating normal operation  
* Input v to toggle verbose. When verbose is on, every time the selected analog input is
  above threshold, a data is sent to serial output. It prints "-[ADC val]   0000000000000000000000000000000000000000000000000000000000000000"  
* Input r to inquire current value of selected anlaog input. It prints  "Threshold: [Thresh val] Current AI value: [ADC val]"  
* Input a value from 1 to 9 to map threshold from 0 to 1023. It prints "Threshold old: [old thesh] New: [new thresh]"  

* Circuit: LED anode (long leg) into DO. LED's cathode to resistor and then into ground.  
* Anolog input reads a positive signal. Circuit capacitance can induce a signal on the input. So for proper triggering:
  Use a toggle switch and then positive terminal--->switch--->*--->resistor--->Ground. Notice you need to do Analog readout at (*) 
  
  Kf
