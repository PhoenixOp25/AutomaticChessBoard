
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

void setup() {
  // put your setup code here, to run once:
  pinMode(X_step_pin, OUTPUT);
  pinMode(X_dir_pin, OUTPUT);
  pinMode(Y_step_pin, OUTPUT);
  pinMode(Y_dir_pin, OUTPUT);
  pinMode(X_limit_pin, INPUT_PULLUP);
  pinMode(Y_limit_pin, INPUT_PULLUP);
  Serial.begin(9600);

  find_home(Y_step_pin,Y_dir_pin,Y_limit_pin);
  find_home(X_step_pin,X_dir_pin,X_limit_pin);
    
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
