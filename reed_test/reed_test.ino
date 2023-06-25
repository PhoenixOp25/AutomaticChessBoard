
#define s0 2 
#define s1 3
#define s2 4
#define s3 5
#define s4 6
#define s5 7
#define sig 8
#define SET(x,y) (x |= (1<<y))
#define CLR(x,y) (x &= (~(1<<y)))

void setup() {
  // put your setup code here, to run once:
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(s4,OUTPUT);
  pinMode(s5,OUTPUT);
  pinMode(sig,INPUT_PULLUP); 
  auto MUX[6] = {s0,s1,s2,s3,s4,s5}; 
  int arr[8][8] = {0};
  Serial.begin(9600);

  piece();
}

void loop() {
  // put your main code here, to run repeatedly:

}
void piece() {
    for(int i=0; i<64; i++) {
    int key = i;
    int r = i/8;
    int c = i%8;
    //Serial.println("G/"+String(r)+"/"+String(c));     
    for(int j=0; j<6; j++) {
      int x = (key&1);
      if(x==0) CLR(PORTD,MUX[j]);
      else SET(PORTD,MUX[j]);
      key = key>>1;
    }
    int val = digitalRead(sig);

    arr[r][c] = val;
  }
  for(int i=0; i<8; i++) {
    for(int j=0; j<8;j++) {
      Serial.print(arr[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
