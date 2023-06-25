
#define s0 4
#define s1 5
#define s2 6
#define s3 7
#define sig 8
#define c1 9
#define SET(x,y) (x |= (1<<y))
#define CLR(x,y) (x &= (~(1<<y)))

void setup() {
  // put your setup code here, to run once:
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(c1,OUTPUT);
  pinMode(sig,INPUT_PULLUP);
  SET(PORTD,s0);
  CLR(PORTD,s1);
  CLR(PORTD,s2);
  CLR(PORTD,s3);
  digitalWrite(c1,HIGH);

  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int val = digitalRead(sig);
  Serial.println(val);
  digitalWrite(13,val);

}
