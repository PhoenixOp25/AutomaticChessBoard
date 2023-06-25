// defines pins numbers
const int stepPin = 3; 
const int dirPin = 6; 
//const int enPin = 8;
const int limitPin = 10;
void setup() {

  pinMode(limitPin,INPUT_PULLUP);
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  
  //pinMode(enPin,OUTPUT);
  //digitalWrite(enPin,LOW);
  
}
void loop() {
  digitalWrite(dirPin,HIGH);
 if( digitalRead(limitPin) == LOW){  
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(700); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(700); 
 }
  
}
