#include <Vector.h>
#include <SPI.h>
#include "LCD_Functions.h"

struct rect {
  int rectX1;
  int rectY1;
  int rectX2;
  int rectY2;

  rect(int x1, int y1, int x2, int y2, int dir){
    int rectX1 = x1;
    int rectY1 = y1;
    int rectX2 = x2;
    int rectY2 = y2;
  }
};

struct ball {
  int ballX = 41;
  int ballY = 40;
  int ballR = 2;
  int ballNewX = 1;
  int ballNewY = -1;

  ball(int x, int y, int r){
    ballX = x;
    ballY = y;
    ballR = r;
  }

  int ballTop = ballY - ballR;
  int ballBot = ballY + ballR;
  int ballRight = ballX + ballR;
  int ballLeft = ballX - ballR;
};

struct brick {
  int x1;
  int y1;
  int x2;
  int y2;
  brick(int i1, int q1, int i2, int q2) {
    x1 = i1;
    y1 = q1;
    x2 = i2;
    y2 = q2;
  }
};

int joystick_state = 0;
int mapX = 0;
int mapY = 0;

bool extraLife = true;
bool sound = false;
bool win = false;
bool lose = false;

ball b(41, 40, 2);
rect r(31, 45, 51, 43, 0);

const int OBJECT_COUNT = 16;
brick* object_array[OBJECT_COUNT];
Vector<brick*> myBricks(object_array);

void updateBall(){
  b.ballTop = b.ballY - b.ballR;
  b.ballBot = b.ballY + b.ballR;
  b.ballRight = b.ballX + b.ballR;
  b.ballLeft = b.ballX - b.ballR;
}

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

const unsigned short tasksNum = 5;
task tasks[tasksNum];


enum DISP_States { DISP_Start, DISP_Wait, DISP_Win, DISP_Lose };

int TickFct_Display(int state) {
  switch(state) { // State Transitions
    case DISP_Start:
      extraLife = true;
      clearDisplay(WHITE);
      setChar('P', 25, 15, BLACK);
      setChar('r', 32, 15, BLACK);
      setChar('e', 39, 15, BLACK);
      setChar('s', 46, 15, BLACK);
      setChar('s', 53, 15, BLACK);

      setChar('S', 25, 23, BLACK);
      setChar('t', 32, 23, BLACK);
      setChar('a', 39, 23, BLACK);
      setChar('r', 46, 23, BLACK);
      setChar('t', 53, 23, BLACK);

      updateDisplay();
      
      state = DISP_Wait;
      break;
    
    case DISP_Wait:
      if(digitalRead(A2) == HIGH) {
            clearDisplay(WHITE);
            setRect(r.rectX1, r.rectY1, r.rectX2, r.rectY2, 1, BLACK);
            setRect(0, 0, 83, 47, 0, BLACK);
            for(int i = 0; i < myBricks.size(); i++) {
              setRect(myBricks.at(i)->x1, myBricks.at(i)->y1, myBricks.at(i)->x2, myBricks.at(i)->y2, 1, BLACK);
            } 
            digitalWrite(12, HIGH);
            updateDisplay();
      }
      else if(digitalRead(A3) == HIGH) { state = DISP_Start; }
      else if (win) { state = DISP_Win;}
      else if (lose) { state = DISP_Lose; }
      else { state = DISP_Wait; } 
      
      break;
   
    case DISP_Win:
      clearDisplay(WHITE);
      setChar('Y', 31, 15, BLACK);
      setChar('o', 38, 15, BLACK);
      setChar('u', 45, 15, BLACK);
      
      setChar('W', 28, 23, BLACK);
      setChar('i', 35, 23, BLACK);
      setChar('n', 42, 23, BLACK);
      setChar('!', 49, 23, BLACK);
      updateDisplay();

      extraLife = true;
      win = false;

      state = DISP_Wait;
      break;

    case DISP_Lose:
      clearDisplay(WHITE);
      setChar('G', 28, 15, BLACK);
      setChar('a', 35, 15, BLACK);
      setChar('m', 42, 15, BLACK);
      setChar('e', 49, 15, BLACK);

      setChar('O', 28, 23, BLACK);
      setChar('v', 35, 23, BLACK);
      setChar('e', 42, 23, BLACK);
      setChar('r', 49, 23, BLACK);
      updateDisplay();

      lose = false;
      extraLife = true;

      state = DISP_Wait;
      break;
    
    default:
      break;
    
  } // State Transitions
  
  switch(state) { // State actions
    case DISP_Start:
      break;
    
    case DISP_Wait:
      break;
   
    case DISP_Win:
      break;

    case DISP_Lose:
      break;
    
    default:
      break;
    
  } // State actions
  
  return state;
}


enum RECT_States { RECT_Start, RECT_Wait, RECT_Move };

int TickFct_Rectangle(int state) {
  switch(state) { // Transitions
    case RECT_Start:
      r.rectX1 = 31;
      r.rectY1 = 45;
      r.rectX2 = 51;
      r.rectY2 = 43;
      state = RECT_Wait;
      break;
    case RECT_Wait:
      if(digitalRead(A2) == HIGH) {
            state = RECT_Move;        
      } else { state = RECT_Wait; }
      break;
    
    case RECT_Move:
      setRect(r.rectX1, r.rectY1, r.rectX2, r.rectY2, 1, WHITE);
      if(digitalRead(A3) == HIGH  || win || lose) {
          clearDisplay(1);
          state = RECT_Start;
      }
      if(!(mapX < 599 && mapX > 399)) { 
            if(mapX <= 399 && (r.rectX1 > 1)) {
              r.rectX1 -= 2;
              r.rectX2 -= 2;
            } else if(mapX >= 599 && (r.rectX2 < 82)) {
              r.rectX1 += 2;
              r.rectX2 += 2; 
            }
      }
      
      setRect(r.rectX1, r.rectY1, r.rectX2, r.rectY2, 1, BLACK);
      break;
    
    default:
      state = RECT_Start;
      break;
    
  } // Transitions
  
  switch(state) { // State actions
    case RECT_Start:
      break;
    
    case RECT_Wait:
      break;
   
    case RECT_Move:
      updateDisplay();
      mapX = analogRead(A0);
      mapY = analogRead(A1);
      joystick_state = digitalRead(8);
      break;
    
    default:
      break;
    
  } // State actions
  
  return state;
}


enum BALL_States { BALL_Start, BALL_Wait, BALL_Move };

int TickFct_Ball(int state) {
  switch(state) { // State Transitions
    case BALL_Start:
      b.ballX = 41;
      b.ballY = 40;
      b.ballR = 2;
      b.ballNewY = -1;
      updateBall();
      state = BALL_Wait;
      break;
    
    case BALL_Wait:
      if(digitalRead(A2) == HIGH) {
        setCircle(b.ballX, b.ballY, b.ballR, BLACK, b.ballR);
        if(random(2) == 0) { b.ballNewX = -1; }
        else { b.ballNewX = 1; }
        state = BALL_Move;
      }
      break;
   
    case BALL_Move:
      if(digitalRead(A3) == HIGH || win || lose) {
          state = BALL_Start;
      }
      break;
    
    default:
      state = BALL_Start;
    break;
    
  } // State Transitions
  
  switch(state) { // State actions
    case BALL_Start:
      break;
    
    case BALL_Wait:
      break;
   
    case BALL_Move:
      updateDisplay();
      break;
    
    default:
      break;
    
  } // State actions
  
  return state;
}


enum COLL_States { COLL_Start, COLL_Wait, COLL_Check};

int TickFct_Collision(int state) {
   switch(state) { // State Transitions
    case COLL_Start:
      for(int y = 12; y < 35; y += 5) {
        for(int x = 3; x < 80; x += 20) {
          myBricks.push_back(new brick(x, y, x+17, y+3));
        }
      }
      state = COLL_Wait;
      break;

    case COLL_Wait:
      if(digitalRead(A2) == HIGH) {
        state = COLL_Check;
      } else {
        state = COLL_Wait;
      }
      break;
    
    case COLL_Check:
      if(myBricks.empty() && digitalRead(A3) == LOW && digitalRead(A2) == LOW) { win = true; }

      if(b.ballTop >= 46 && digitalRead(A3) == LOW && digitalRead(A2) == LOW) {
        if(extraLife){
          extraLife = false;
          digitalWrite(12, LOW);
          b.ballX = 41;
          b.ballY = 40;
          b.ballR = 2;
          b.ballNewY = -1;
          updateBall();
          b.ballTop = 0;
        } else {
          lose = true;
        }
      }
      
      if(digitalRead(A3) == HIGH || win || lose) {
          for(int i = 0; i < myBricks.size(); i++) {
            brick* temp = myBricks.at(i);
            myBricks.remove(i);
            i--;
            delete temp;
          }
          state = COLL_Start;
      }

      setCircle(b.ballX, b.ballY, b.ballR, WHITE, b.ballR);

      updateBall();

      if(b.ballTop <= 1) { b.ballY = 3 + random(2); b.ballNewY *= -1; }
      if(b.ballRight >= 82) { b.ballX = 80 - random(2); b.ballNewX *= -1; }
      if(b.ballLeft <= 1) { b.ballX = 3 + random(2); b.ballNewX *= -1; }
      if(b.ballBot > r.rectY2 && b.ballTop < r.rectY1 && b.ballRight < r.rectX2 && b.ballLeft > r.rectX1 ) {
        b.ballY -= 2;
        b.ballX += random(-2, 3);
        b.ballNewY *= -1;
      }

      for(int i = 0; i < myBricks.size(); i++) {
        brick* temp = myBricks.at(i);
        if(b.ballBot > temp->y1 && b.ballTop < temp->y2 && b.ballLeft < temp->x2 && b.ballRight > temp->x1 ) {
          sound = true;
          myBricks.remove(i);
          i--;
          
          setRect(temp->x1, temp->y1, temp->x2, temp->y2, 1, WHITE);
          setCircle(b.ballX, b.ballY, b.ballR, WHITE, b.ballR);

          if(b.ballTop <= temp->y1) { b.ballNewY *= -1; b.ballY += (2 + random(2)) * b.ballNewY; updateBall(); }
          else if(b.ballBot >= temp->y2) { b.ballNewY *= -1; b.ballY += (2 + random(2)) * b.ballNewY; updateBall(); }
          else if(b.ballRight >= temp->x2) { b.ballNewX *= -1; b.ballX += (2 + random(2)) * b.ballNewX; updateBall(); }
          else if(b.ballLeft <= temp->x1) { b.ballNewX *= -1; b.ballX += (2 + random(2)) * b.ballNewX; updateBall(); }
          
          delete temp;
        }
      }

      b.ballX += b.ballNewX;
      b.ballY += b.ballNewY;

      setCircle(b.ballX, b.ballY, b.ballR, BLACK, b.ballR);
      break;
    
    default:
      state = COLL_Start;
      break;
    
  } // State Transitions


  switch(state) { // State actions
    case COLL_Start:
      break;
    
    case COLL_Check:
      break;
    
    default:
      break;
    
  } // State actions
  
  return state;
}


enum TONE_States { TONE_Start, TONE_Wait, TONE_Play, TONE_Stop };

int TickFct_Sound(int state) {
  switch(state) { // State Transitions
    case TONE_Start:
      state = TONE_Wait;
      break;
    
    case TONE_Wait:
      if(sound) { state = TONE_Play; }
      else { state = TONE_Wait; }
      break;

    case TONE_Play:
      tone(3, 1000);
      state = TONE_Stop;
      break;

    case TONE_Stop:
      noTone(3);
      sound = false;
      state = TONE_Wait;
      break;
    
    default:
      break;

  } // State Transitions

  switch(state) { // State actions
    case TONE_Start:
      break;
    
    case TONE_Wait:
      break;

    case TONE_Play:
      break;

    case TONE_Stop:
      
      break;
    
    default:
      break;
    
  } // State actions
  
  return state;
}

void setup() {
 
  
  Serial.begin(9600);
  randomSeed(analogRead(0));

  lcdBegin();
  clearDisplay(1);
  updateDisplay();
  setContrast(60);

  updateDisplay();
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  pinMode(12, OUTPUT);
  
  
  unsigned char i = 0;
  tasks[i].state = DISP_Start;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_Display;
  i++;
  tasks[i].state = RECT_Start;
  tasks[i].period = 25;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_Rectangle;
  i++;
  tasks[i].state = BALL_Start;
  tasks[i].period = 25;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_Ball;
  i++;
  tasks[i].state = COLL_Start;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_Collision;
  i++;
  tasks[i].state = TONE_Start;
  tasks[i].period = 25;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_Sound;

}


void loop() {
  //updateDisplay();
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
      
     }
   }
  delay(25); //GCD
}
