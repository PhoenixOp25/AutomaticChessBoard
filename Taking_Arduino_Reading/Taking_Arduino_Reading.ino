#define s0 2 
#define s1 3
#define s2 4
#define s3 5
#define s4 6
#define s5 7
#define sig 8
#define SET(x,y) (x |= (1<<y))
#define CLR(x,y) (x &= (~(1<<y)))
int move_arr[2];
int arr[8][8] = {0};
String str;
void setup() {
  // put your setup code here, to run once:
      pinMode(s0,OUTPUT);
      pinMode(s1,OUTPUT);
      pinMode(s2,OUTPUT);
      pinMode(s3,OUTPUT);
      pinMode(s4,OUTPUT);
      pinMode(s5,OUTPUT);
      pinMode(sig,INPUT_PULLUP); 
      int MUX[6] = {s0,s1,s2,s3,s4,s5}; 
      Serial.begin(9600);
      reading(); //storing all sensors reading in a string
      Serial.print(str); // returning the output of all sensors to the nodemcu
}

void loop() {
  // put your main code here, to run repeatedly:
  

}

void reading() {
    int k = 0;
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
        str = str + String(val);
      }
}  
