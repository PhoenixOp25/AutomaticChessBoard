leenum states_enum{initial, WhiteMove, PieceCaptured, InvalidMoveW, SensorsErrorW, BlackMove, DisplacementErrorB, SensorsErrorB}; 
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
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  switch(current_state) {
    case initial:
      Serial.println("initial");
      //if(!checkInitial()) break;; //check the initial state if there is any error , then resolve it;
      current_state = WhiteMove;
      break;

    case WhiteMove:
      Serial.println("WhiteMove");
        //ifButtonPressed() //if button in pressed, check if there is any error or not
        if(pieceCaptured()) {
          current_state = PieceCaptured;
          break;
        }
        current_state = SensorsErrorW;
        break;
        
    case PieceCaptured:
      if (!ButtonPressed()) break;
      current_state = SensorsErrorW;
        break;   

    case SensorsErrorW:
      Serial.println("SensorsErrorW"); 
        if(sensorsError()) break;      
        current_state = InvalidMoveW;
        break;
         
    case InvalidMoveW:
      Serial.println("InvalidMoveW");
        if(MoveInvalid()) {
          break;        
        }
        else {
          current_state = BlackMove;
          break;
        }                     
       
    case BlackMove:
      Serial.println("BlackMove");
        if(MoveDone()) { //if button in pressed, check if there is any error or not
          current_state = DisplacementErrorB;
          break;
        }
        
    case DisplacementErrorB:
      Serial.println("DisplacementErrorB");
        if(displacementError()) break;
        current_state = SensorsErrorB;
        break;  
        
    case SensorsErrorB:
      Serial.println("SensorsErrorB");
        if(sensorsError()) break;
        current_state = WhiteMove;
        break;                                    
  }
}

bool checkinitial() {
  return true;
}

bool ButtonPressed() {
  return true;
}

bool pieceCaptured() {
  return false;
}

bool sensorsError() {
  return false;
}

bool MoveInvalid() {
  return false;
}

bool MoveDone() {
  return true;
}

bool displacementError() {
  return false;
}
