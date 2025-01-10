#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 48
#define TFT_CS 50
#define TFT_MOSI 51
#define TFT_CLK 46
#define TFT_RST 49
#define TFT_MISO 47
const int btn1 = 18;
const int btn2 = 19;
const int screenWidth = 320;
const int screenHeight = 240;
volatile int btn1state = 0;
volatile int btn2state = 0;
unsigned long lastDebounceTimeBtn1 = 0;
unsigned long lastDebounceTimeBtn2 = 0;
unsigned long debounceDelay = 200;
int snakeScore = 0;
int player1Score = 0;
int player2Score = 0;

Adafruit_ILI9341 TFT = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);


void button1_ISR() {
  if (millis() - lastDebounceTimeBtn1 > debounceDelay) {
    btn1state = HIGH;
    lastDebounceTimeBtn1 = millis();
  }
}

void button2_ISR() {
  if (millis() - lastDebounceTimeBtn2 > debounceDelay) {
    btn2state = HIGH;
    lastDebounceTimeBtn2 = millis();
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(btn1), button1_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(btn2), button2_ISR, RISING);

  randomSeed(analogRead(0));
  TFT.begin();
  TFT.setRotation(1);
  menu_start();
}

void loop() {

}

void menu_start() {
  TFT.fillScreen(ILI9341_BLACK);
  TFT.setTextColor(ILI9341_WHITE);
  TFT.setTextSize(2);

  TFT.setCursor(50, 50);
  TFT.println("Select a game:");
  TFT.setCursor(50, 100);
  TFT.println("1. Snake");
  TFT.setCursor(50, 150);
  TFT.println("2. Pong");

  while (true) {
    if (btn1state == HIGH) {
      btn1state = LOW;
      game1_start();
      break;
    }
    if (btn2state == HIGH) {
      btn2state = LOW;
      game2_start();
      break;
    }
  }
}


int snakeX[1000];
int snakeY[1000];
int direction = 1;
int gridSize = 5;
int foodX, foodY;

void game1_start() {
  TFT.setTextColor(ILI9341_BLACK);
  TFT.setCursor(50, 50);
  TFT.println("Select a game:");
  TFT.setCursor(50, 100);
  TFT.println("1. Snake");
  TFT.setCursor(50, 150);
  TFT.println("2. Pong");
  int snakeLength = 3;
  snakeX[0] = 160;
  snakeY[0] = 120;
  snakeX[1] = 150;
  snakeY[1] = 120;
  snakeX[2] = 140;
  snakeY[2] = 120;
  drawFood(snakeLength);
  unsigned long lastDirectionChangeTime = 500;
  
  while (true) {
    if (btn1state == HIGH && (millis() - lastDirectionChangeTime) > debounceDelay) {
      if(direction < 4){direction += 1;}
      else{direction = 1;}
      lastDirectionChangeTime = millis();
      btn1state = LOW;
    }

    if (btn2state == HIGH && (millis() - lastDirectionChangeTime) > debounceDelay) {
      if(direction > 1){direction -= 1;}
      else{direction = 4;}
      lastDirectionChangeTime = millis();
      btn2state = LOW;
    }

    moveSnake(snakeLength);

    if (checkCollision(snakeLength)) {
      gameOver(1);
      return;
    }

    snakeLength = checkFoodEaten(snakeLength);
    drawSnake(snakeLength);
    delay(40);
  }
}

void moveSnake(int snakeLength) {
  TFT.fillRect(snakeX[snakeLength - 1], snakeY[snakeLength - 1], gridSize, gridSize, ILI9341_BLACK);
  
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }
  switch (direction) {
    case 1:
      snakeX[0]++;
      break;
    case 2:
      snakeY[0]++;
      break;
    case 3:
      snakeX[0]--;
      break;
    case 4:
      snakeY[0]--;
      break;
  }



}

void drawSnake(int snakeLength) {
  for (int i = 0; i < snakeLength; i++) {
    TFT.fillRect(snakeX[i], snakeY[i], gridSize, gridSize, ILI9341_GREEN);
  }
}

int checkFoodEaten(int snakeLength) {
  if (abs(snakeX[0] - foodX) <= gridSize && abs(snakeY[0] - foodY) <= gridSize) {
    snakeLength += 5;
    snakeScore += 10;
    TFT.fillRect(foodX, foodY, gridSize, gridSize, ILI9341_BLACK);
    drawFood(snakeLength);

    for (int i = snakeLength - 1; i >= snakeLength - 5; i--) {
      snakeX[i] = snakeX[snakeLength - 6];
      snakeY[i] = snakeY[snakeLength - 6];
    }
  }
  return snakeLength;
}

bool checkCollision(int snakeLength) {
  if (snakeX[0] < 0 || snakeX[0] + gridSize > screenWidth || snakeY[0] < 0 || snakeY[0] + gridSize > screenHeight) {
    return true;
  }

  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      return true;
    }
  }

  return false;
}

void gameOver(int game) {
  TFT.setTextSize(2);
  TFT.setCursor(100, 100);
  TFT.setTextColor(ILI9341_WHITE);
  switch(game){
    case 1:
      TFT.fillScreen(ILI9341_RED);
      TFT.println("GAME OVER!");
      TFT.setTextSize(1);
      TFT.setCursor(98, 120);
      TFT.print("You reached ");
      TFT.print(snakeScore);
      TFT.print(" score.");
      TFT.setCursor(40, 150);
      TFT.println("Press any button to go back to the menu.");
      break;
    case 2:
      TFT.setCursor(80, 80);
      TFT.print("Player ");
      TFT.print((player1Score > player2Score) ? "1" : "2");
      TFT.println(" wins!");
      TFT.setCursor(120, 120);
      TFT.print(player1Score);
      TFT.print("  :  ");
      TFT.print(player2Score);
      TFT.setTextSize(1);
      TFT.setCursor(40, 170);
      TFT.println("Press any button to go back to the menu.");
      break;
    default:
      TFT.fillScreen(ILI9341_RED);
      TFT.println("!!ERROR!!");
      break;
  }
  
  while (true) {
    if (btn1state == HIGH || btn2state == HIGH) {
      delay(1000);
      snakeScore = 0;
      player1Score = 0;
      player2Score = 0;
      btn1state = LOW;
      btn2state = LOW;
      menu_start();
      break;
    }
  }
}

void drawFood(int snakeLength) {
  bool validFood = false;
  while (!validFood) {
    foodX = random(0, screenWidth / gridSize) * gridSize;
    foodY = random(0, screenHeight / gridSize) * gridSize;

    validFood = true;
    for (int i = 0; i < snakeLength; i++) {
      if (snakeX[i] == foodX && snakeY[i] == foodY) {
        validFood = false;
        break;
      }
    }
  }
  TFT.fillRect(foodX, foodY, gridSize, gridSize, ILI9341_RED);
}


int ballSpeedX = (random(0,2)) ? 6 : -6;
int ballSpeedY = (random(0,2)) ? random(-1,-5) : random(1, 5);
int paddleWidth = 5;
int paddleHeight = 20;
int ballSize = 5;
int ballX = 160;
int ballY = 120;
int paddle1Y = 100;
int paddle2Y = 100;
bool paddle1DirectionUp = true; 
bool paddle2DirectionUp = true; 

void game2_start() {
  TFT.setTextColor(ILI9341_BLACK);
  TFT.setCursor(50, 50);
  TFT.println("Select a game:");
  TFT.setCursor(50, 100);
  TFT.println("1. Snake");
  TFT.setCursor(50, 150);
  TFT.println("2. Pong");

  int paddle1T, paddle2T;

  while (true) {
    if (btn2state == HIGH) {
      paddle1DirectionUp = !paddle1DirectionUp;
      delay(10); 
      btn2state = LOW;
    }

    if (btn1state == HIGH) {
      paddle2DirectionUp = !paddle2DirectionUp;
      delay(10);
      btn1state = LOW;
    }

    paddle1T = paddle1Y;
    if (paddle1DirectionUp && paddle1Y > 0) {
      paddle1Y -= 5;
    } else if (!paddle1DirectionUp && paddle1Y < screenHeight - paddleHeight) {
      paddle1Y += 5;
    }

    paddle2T = paddle2Y;

    if (paddle2DirectionUp && paddle2Y > 0) {
      paddle2Y -= 5;
    } else if (!paddle2DirectionUp && paddle2Y < screenHeight - paddleHeight) {
      paddle2Y += 5;
    }

    drawPaddles(paddle1T,paddle2T);
    moveBall(paddle1T, paddle2T);
    Serial.println(paddle2Y);
    delay(40);
  }
}

void moveBall(int paddle1T, int paddle2T) {
  TFT.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_BLACK);

  ballX += ballSpeedX;
  ballY += ballSpeedY;

  if (ballSpeedY == 0){
    ballSpeedY = (random(0,2)) ? random(-1,-5) : random(1, 5);
  }

  if (ballY <= 0) {
    ballSpeedY = -ballSpeedY;
    ballY = 1;
  }

  if(ballY + ballSize >= screenHeight){
    ballSpeedY = -ballSpeedY;
    ballY = screenHeight - ballSize;
  }

  if (ballX <= paddleWidth && ballY >= paddle1Y && ballY <= paddle1Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
    ballSpeedY += random(-5, 5);
    delay(25);
  }

  if (ballX >= screenWidth - ballSize - paddleWidth && ballY >= paddle2Y && ballY <= paddle2Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
    ballSpeedY += random(-5, 5);
    delay(25);
  }

  if (ballX <= 0) {
    player2Score++;

    if(player2Score>=5){
      TFT.fillRect(0, paddle1T-5, paddleWidth, paddleHeight+5, ILI9341_BLACK);
      TFT.fillRect(screenWidth - paddleWidth, paddle2T-5, paddleWidth, paddleHeight+5, ILI9341_BLACK);
      TFT.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_BLACK);
      gameOver(2);
    }
    else{
    resetGame(paddle1Y, paddle2Y);
    }
  }

  if (ballX >= screenWidth) {
    player1Score++;
    if(player1Score>=5){
      TFT.fillRect(0, paddle1T-5, paddleWidth, paddleHeight+5, ILI9341_BLACK);
      TFT.fillRect(screenWidth - paddleWidth, paddle2T-5, paddleWidth, paddleHeight+5, ILI9341_BLACK);
      TFT.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_BLACK);
      gameOver(2);
    }
    else{
    resetGame(paddle1Y, paddle2Y);
    }
  }
  drawBall();
}

void resetGame(int paddle1T,int paddle2T){
    ballX = 160;
    ballY = 120;
    ballSpeedX = (random(0,2)) ? 6 : -6;
    ballSpeedY = (random(0,2)) ? random(-1,-4) : random(1, 4);
    paddle1Y = 100;
    paddle2Y = 100;
    TFT.fillRect(0, 0, paddleWidth, screenHeight, ILI9341_BLACK);
    TFT.fillRect(screenWidth - paddleWidth, 0, paddleWidth, screenHeight, ILI9341_BLACK);
    
    TFT.fillRect(0, paddle1Y, paddleWidth, paddleHeight, ILI9341_GREEN);
    TFT.fillRect(screenWidth - paddleWidth, paddle2Y, paddleWidth, paddleHeight, ILI9341_GREEN);
    drawBall();
    delay(1000);
}

void drawPaddles(int paddle1T,int paddle2T) {

    TFT.fillRect(0, paddle1T, paddleWidth, paddleHeight, ILI9341_BLACK);
    TFT.fillRect(screenWidth - paddleWidth, paddle2T, paddleWidth, paddleHeight, ILI9341_BLACK);


    TFT.fillRect(0, paddle1Y, paddleWidth, paddleHeight, ILI9341_GREEN);
    TFT.fillRect(screenWidth - paddleWidth, paddle2Y, paddleWidth, paddleHeight, ILI9341_GREEN);
}


void drawBall() {
  TFT.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_WHITE);
}

