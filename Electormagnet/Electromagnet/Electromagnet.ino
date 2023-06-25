
#define val 255

void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(5, val);  
  analogWrite(6, 0);  
  delay(500);  
}
