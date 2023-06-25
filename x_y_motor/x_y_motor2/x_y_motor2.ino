#include <Servo.h>
#define X_step_pin 2
#define X_dir_pin 5
#define Y_step_pin 3
#define Y_dir_pin 6

#define SET(x,y) (x |= (1<<y))
#define CLR(x,y) (x &= (~(1<<y)))

//When pullup resistor is used and C and NC pins are used, Limit switches are high when pressed.
//Connect C to black and NC to white in cnc shield
#define X_limit_pin 9
#define Y_limit_pin 10

#define faster_speed 1000
#define medium_speed 2000
#define slow_speed 4000
Servo myservo;
int pos = 0;
String str;
int prev_x = 1;
int prev_y = 1;
int from_x;
int from_y;
int to_x;
int to_y;
void setup() {
  // put your setup code here, to run once:
  myservo.attach(11);
  pinMode(X_step_pin, OUTPUT);
  pinMode(X_dir_pin, OUTPUT);
  pinMode(Y_step_pin, OUTPUT);
  pinMode(Y_dir_pin, OUTPUT);
  pinMode(X_limit_pin, INPUT_PULLUP);
  pinMode(Y_limit_pin, INPUT_PULLUP);
  Serial.begin(9600);
  digitalWrite(electromagnet1,HIGH);
  digitalWrite(electromagnet2,LOW);
  find_home(Y_step_pin,Y_dir_pin,Y_limit_pin);
  find_home(X_step_pin,X_dir_pin,X_limit_pin);
  CLR(PORTD,X_dir_pin);
  CLR(PORTD,Y_dir_pin);
  move_centre();
  str = Serial.readStringUntil('\n'); //str format should be (previous_square + current_square)
  from_x = str[0]-'a' + 1;
  from_y = str[1]-'0';
  to_x = str[2]-'a' + 1;
  to_y = str[3] - '0'

    // the magnet will move firstly from (prev->from)
    // then it will from (from->to) and 'prev' is updated as 'to'
  move_x_y(prev_x,prev_y,from_x,from_y);  
  for(pos=0;pos<=158;pos++) {
    myservo.write(pos);
    delay(5);
  }
  move_x_y(from_x,from_y,to_x,to_y);
  for(pos=158;pos>=0;pos--) {
    myservo.write(pos);
    delay(5);
  }
  prev_x = to_x;
  prev_y = to_y;
}

void loop() {
  // put your main code here, to run repeatedly:
}
void find_home(int step_pin,int dir_pin,int limit_pin) {
    SET(PORTD, dir_pin);
  while(digitalRead(limit_pin) == LOW)
    move_one_step(medium_speed,step_pin);
  CLR(PORTD, dir_pin);
  for(int i = 0; i < 100; i++)
    move_one_step(slow_speed,step_pin);
  SET(PORTD, dir_pin);
  while(digitalRead(limit_pin) == LOW){
    move_one_step(slow_speed,step_pin);
  }
}
void move_one_step(int speed, int step_pin){
  SET(PORTD, step_pin);
  delayMicroseconds(20);
  CLR(PORTD, step_pin);
  delayMicroseconds(speed);
}

void move_x_y(int from_x, int from_y, int to_x, int to_y) {
  //move_first_corner();
  if(from_x <= to_x) {
      SET(PORTD,X_step_pin);
      delayMicroseconds(speed);
  }
  else {
    CLR(PORTD,X_step_pin);
    delayMicroseconds(speed);    
  }
  if(from_y <= to_y) {
      SET(PORTD, step_pin);    
      delayMicroseconds(speed);    
  }
  else {
    CLR(PORTD,Y_step_pin);
    delayMicroseconds(speed);    
  }  
  for(int i=0; i<from_x-1; i++) {
    move_one_block(X_step_pin, X_dir_pin);
  }
  for(int i=0; i<from_y-1; i++) {
    move_one_block(Y_step_pin,Y_dir_pin);
  }
}

void move_first_corner() {
  for(int i=0; i<750; i++) {
    move_one_step(medium_speed,X_step_pin);
  }
//  for(int i=0; i<500; i++) {
//    move_one_step(medium_speed,Y_step_pin);
//  }
  
}

void move_one_block(int step_pin, int dir_pin) {
  for(int i=0; i<1250; i++) {
    move_one_step(medium_speed,step_pin);
  }
}

void move_centre(int step_pin, int dir_pin) {
  for(int i=0; i<625; i++) {
    move_one_step(medium_speed,step_pin);
  }
}
