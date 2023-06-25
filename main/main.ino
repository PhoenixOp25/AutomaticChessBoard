
#include<ESP8266WiFi.h>  //   include ESP8266 library
const char* ssid= "Ligma";    // wifi name
const char* pass= "RandomPassword";  // wifi password
WiFiServer server(80); 

enum states_enum{initial, WhiteMove, PieceCaptured, InvalidMoveW, SensorsErrorW, SendingMove, GettingMove, BlackMove, DisplacementErrorB, SensorsErrorB}; 
int current_state = initial;

char chess[8][8] = {
  {'R','N','B','K','Q','B','N','R'},
  {'P','P','P','P','P','P','P','P'},
  {'.','.','.','.','.','.','.','.'},
  {'.','.','.','.','.','.','.','.'},
  {'.','.','.','.','.','.','.','.'},
  {'.','.','.','.','.','.','.','.'},
  {'P','P','P','P','P','P','P','P'},
  {'R','N','B','K','Q','B','N','R'}
};

#define s0 2 
#define s1 3
#define s2 4
#define s3 5
#define s4 6
#define s5 7
#define sig 8

String response;
String str; //String of all reading of sensors;
String H_move = ""; //Initialisation of a human move int string format
String C_move = ""; // Initialisation of a computer move in string format
int cur_x; // Initialisation of current move x-coordinate
int cur_y; // Initialisation of current move y-coordinate
int prev_x; // Initialisation of previous move x-coordinate
int prev_y; // Initialisation of previous move y-coordinate
char piece1;
char piece2;
int arr[8][8] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1}
};
int move_arr[2]; //to store the changes that happened in one move // changes happen at only two squares
// i am also assuming that sensors give 1 as output if piece is above them
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    WiFi.begin(ssid,pass);     
    while ((!(WiFi.status() == WL_CONNECTED))) {
        delay(300); 
    }    
    //Serial.print("IP address: ");
    //Serial.println(WiFi.localIP());
    server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:  
  WiFiClient myclient = server.available();
  if (!myclient) {
      return;
  }
  while (!myclient.available()) {
      delay(1);
  }  
  String str = (myclient.readStringUntil('\r'));
  myclient.flush();  // flush out extra line from request
  Serial.println(str);  
  
  switch(current_state) {
    case initial:
      //Serial.println("initial");
      //if(!checkInitial()) break;; //check the initial state if there is any error , then resolve it;
      current_state = WhiteMove;
      break;

    case WhiteMove:
      //Serial.println("WhiteMove");
        //ifButtonPressed() //if button in pressed, check if there is any error or not
        while(!Serial.available()) { // Waiting for data from sensors arduino
          delay(1);
        }
        whitemove();
        if(piececaptured()) {
          current_state = PieceCaptured;
          break;
        }
        else {
          current_state = SendingMove;
          break;
        }
        
    case PieceCaptured:
      while(!Serial.available()) {
        delay(1);
      }
      whitemove();
      capturedpiece();
      current_state = SendingMove;  
      break;

//    case SensorsErrorW:
//      Serial.println("SensorsErrorW"); 
//        if(sensorsError()) break;      
//        current_state = InvalidMoveW;
//        break;
         
//    case InvalidMoveW:
//      Serial.println("InvalidMoveW");
//        if(MoveInvalid()) {
//          break;        
//        }
//        else {
//          String str1 = int_to_square(sq1);
//          String str2 = int_to_square(sq2);
//          H_move = str1+str2;
//          current_state = SendingMove;
//          break;
//        }                     
    
    case SendingMove:
      MoveNotation();      
      response = "HTTP/1.1 200 OK\r\n Content-type:text/html\r\n\r\n"+H_move;
      current_state = GettingMove;
      break;

    case GettingMove:
      while(!Serial.available()) {
          //C_move = Serial.readStringUntil('\n');   
          break;
      }    
      C_move = Serial.readStringUntil('\n'); // Computer Move is through the nodemcu server //(Python code)
      current_state = BlackMove;
      break;
         
    case BlackMove:
      //Serial.println("BlackMove");
      // C_move stores the engine best move and store it in a string format
      // We have to instruct motor accoridng to the coordinates
      Serial.print(C_move);
      while(!MoveDone()) {
        // Send Move to arduino
        // which instructs the piece to move
        
      }
      if(MoveDone()) { //if button in pressed, check if there is any error or not
        //current_state = DisplacementErrorB;
        current_state - WhiteMove;
        break;
      }
        
//    case DisplacementErrorB:
//      Serial.println("DisplacementErrorB");
//        if(displacementError()) break;
//        current_state = SensorsErrorB;
//        break;  
//        
//    case SensorsErrorB:
//      Serial.println("SensorsErrorB");
//        if(sensorsError()) break;
//        current_state = WhiteMove;
//        break;                                    
//  }
}

bool checkinitial() {
  // 
  return true;
}

//bool ButtonPressed() {
//  return true;
//}
//
//
//
//bool sensorsError() {
//  return false;
//}

//bool MoveInvalid() {
//  // Move is transferred to the the server
//  // stockfish analyse the move and check if the move is invalid or not
//  // if move is valid -> return false;
//  // else -> return true;
//  return false;
//}

//bool MoveDone() {
//  return true;
//}
//
//bool displacementError() {
//  return false;
//}
}
 
void whitemove() {
  int k = 0;
  str = Serial.readStringUntil('\n');
  for(int i=0; i<64; i++) {
    int val = str[i]-'0';
    int r = i/8;
    int c = i%8;
    if(arr[r][c]!=val) {
      move_arr[k++] = i;
    }
    arr[r][c] = val;
  }  
}

bool piececaptured() {
  int cnt = 0;
  int sq1 = move_arr[0];
  int sq2 = move_arr[1];
  int x1 = sq1/8;
  int y1 = sq1%8;
  int x2 = sq2/8;
  int y2 = sq2%8;  
  if(chess[x1][y1]!='.' && arr[x1][y1]==0) { ////if there was a piece previously on the square and now its not
    cnt++;
  }
  if(chess[x2][y2]!='.' && arr[x2][y2]==0) { //if there was a piece previously on the square and now its not
    cnt++;
  }
  if(cnt==1) { // the count is either 1 or 2 
    // if the count is 1 -> No capturing
    // if the count is 2 -> capturing was there
    if(chess[x1][y1]!='.') {
      update_board(sq2,chess[x1][y1]);
      update_board(sq1,'.');
      prev_x = x1;
      prev_y = y1;
      cur_x = x2;
      cur_y = y2;        
    }
    else {
      update_board(sq1,chess[x2][y2]);
      update_board(sq2,'.'); 
      prev_x = x2;
      prev_y = y2;
      cur_x = x1;
      cur_y = y1;              
    }
    return false;
  }
  piece1 = chess[x1][y1];
  piece2 = chess[x2][y2];
  chess[x1][y1] = '.';
  chess[x2][y2] = '.';
  return true;
}

void capturedpiece() {
  int sq1 = move_arr[0];
  int sq2 = move_arr[1];
  int x1 = sq1/8;
  int y1 = sq1%8;
  int x2 = sq2/8;
  int y2 = sq2%8;   
  if(arr[x1][y1]==1) {
    update_board(sq1,piece2);
    prev_x = x2;
    prev_y = y2;
    cur_x = x1;
    cur_y = y1;
  }
  else {
    update_board(sq2,piece1);
    prev_x = x1;
    prev_y = y1;
    cur_x = x2;
    cur_y = y2;    
  }
}

void update_board(int sq_no , char piece) {
  int r = sq_no/8;
  int c = sq_no%8;
  chess[r][c] = piece;
}

void MoveNotation() {
  H_move = "";
  String chr;
  if(prev_x == 1) chr = "a";
  else if(prev_x == 2) chr = "b";
  else if(prev_x == 3) chr = "c";
  else if(prev_x == 4) chr = "d";
  else if(prev_x == 5) chr = "e";
  else if(prev_x == 6) chr = "f";
  else if(prev_x == 7) chr = "g";
  else if(prev_x == 8) chr = "h";
  H_move = H_move + chr;
  
  if(prev_y == 1) chr = "1";
  else if(prev_y == 2) chr = "2";
  else if(prev_y == 3) chr = "3";
  else if(prev_y == 4) chr = "4";
  else if(prev_y == 5) chr = "5";
  else if(prev_y == 6) chr = "6";
  else if(prev_y == 7) chr = "7";
  else if(prev_y == 8) chr = "8"; 
  H_move = H_move + chr;   

  if(cur_x == 1) chr = "a";
  else if(cur_x == 2) chr = "b";
  else if(cur_x == 3) chr = "c";
  else if(cur_x == 4) chr = "d";
  else if(cur_x == 5) chr = "e";
  else if(cur_x == 6) chr = "f";
  else if(cur_x == 7) chr = "g";
  else if(cur_x == 8) chr = "h"; 
  H_move = H_move + chr;  
  
  if(cur_x == 1) chr = "1";
  else if(cur_x == 2) chr = "2";
  else if(cur_x == 3) chr = "3";
  else if(cur_x == 4) chr = "4";
  else if(cur_x == 5) chr = "5";
  else if(cur_x == 6) chr = "6";
  else if(cur_x == 7) chr = "7";
  else if(cur_x == 8) chr = "8"; 
  H_move = H_move + chr;      
}
