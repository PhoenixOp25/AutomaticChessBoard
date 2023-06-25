
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
  CLR(PORTD,X_dir_pin);
  CLR(PORTD,Y_dir_pin);
  move_first_centre();
  int prev_x = 0;
  int prev_y = 0;
  //move_x_y(4,3);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    String str = Serial.readStringUntil('\n');
    char x_s = "";
    char y_s = "";

    x_s = str[2]; // available string is of the form 'G/x/y' 
    y_s = str[4]; // x-coordinate is at 2nd index while y-coordinate is at 4th index;
    
    int x = x_s - '0'; 
    int y = y_s - '0';

    move_x_y(x-prev_x,y-prev_y); 
    
    prev_x = x;
    prev_y = y;
  }
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

void move_x_y(int x, int y) {
  //move_first_centre();
  for(int i=0; i<x-1; i++) {
    move_one_block(X_step_pin, X_dir_pin);
  }
  for(int i=0; i<y-1; i++) {
    move_one_block(Y_step_pin,Y_dir_pin);
  } 
}

void move_first_centre() {
  for(int i=0; i<625; i++) {
    move_one_step(medium_speed,X_step_pin);
  }
  for(int i=0; i<625; i++) {
    move_one_step(medium_speed,Y_step_pin);
  }
}

void move_one_block(int step_pin, int dir_pin) {
  for(int i=0; i<1250; i++) {
    move_one_step(medium_speed,step_pin);
  }
}
