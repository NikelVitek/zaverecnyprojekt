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
  TFT.fillScreen(ILI9341_BLACK);
}

void loop() {
  menu_start();
}

void menu_start() {
  TFT.fillScreen(ILI9341_BLACK);
  TFT.setCursor(120, 120);
  TFT.println("Press Button");

  while (true) {
    if (btn1state == HIGH) {
      game1_start();
      btn1state = LOW;
    }
    if (btn2state == HIGH) {
      game2_start();
      btn2state = LOW;
    }
  }
}

int snakeX[1000];
int snakeY[1000];
int snakeLength = 3;
int direction = 1;
int gridSize = 5;
int foodX, foodY;

void game1_start() {
  TFT.setTextColor(ILI9341_BLACK);
  TFT.setCursor(120, 120);
  TFT.println("Press Button");
  drawFood();
  snakeX[0] = 160;
  snakeY[0] = 120;
  snakeX[1] = 150;
  snakeY[1] = 120;
  snakeX[2] = 140;
  snakeY[2] = 120;

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

    moveSnake();

    if (checkCollision()) {
      gameOver();
      return;
    }

    checkFoodEaten();

    drawSnake();
    delay(40);
  }
}

void moveSnake() {
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

  TFT.fillRect(snakeX[0], snakeY[0], gridSize, gridSize, ILI9341_GREEN);

  if (snakeLength > 3) {
    TFT.fillRect(snakeX[snakeLength - 2], snakeY[snakeLength - 2], gridSize, gridSize, ILI9341_GREEN);
  }
}

void drawSnake() {
  for (int i = 0; i < snakeLength; i++) {
    TFT.fillRect(snakeX[i], snakeY[i], gridSize, gridSize, ILI9341_GREEN);
  }
}

void checkFoodEaten() {
  if (abs(snakeX[0] - foodX) <= gridSize && abs(snakeY[0] - foodY) <= gridSize) {
    snakeLength += 5;

    TFT.fillRect(foodX, foodY, gridSize, gridSize, ILI9341_BLACK);

    drawFood();

    for (int i = snakeLength - 1; i >= snakeLength - 5; i--) {
      snakeX[i] = snakeX[snakeLength - 6];
      snakeY[i] = snakeY[snakeLength - 6];
    }
  }
}

bool checkCollision() {
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

void gameOver() {
  TFT.fillScreen(ILI9341_RED);
  TFT.setCursor(100, 100);
  TFT.setTextColor(ILI9341_WHITE);
  TFT.setTextSize(2);
  TFT.println("GAME OVER!");
  TFT.setTextSize(1);
  TFT.setCursor(35, 120);
  TFT.println("Press any button to restart or go to menu.");
  
  while (true) {
    if (btn1state == HIGH || btn2state == HIGH) {
      delay(1000);
      menu_start();
      break;
    }
  }
}

void drawFood() {
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

int ballSpeedX = (random(0,1)) ? 6 : -6;
int ballSpeedY = (random(0,1)) ? random(-1,-5) : random(1, 5);
int paddleWidth = 5;
int paddleHeight = 20;
int ballSize = 5;
int ballX = 160;
int ballY = 120;

bool paddle1DirectionUp = true; 
bool paddle2DirectionUp = true; 

void game2_start() {
  TFT.setTextColor(ILI9341_BLACK);
  TFT.setCursor(120, 120);
  TFT.println("Press Button");
  
  int paddle1Y = 100;
  int paddle2Y = 100;
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

    moveBall(paddle1Y, paddle2Y, paddle1T, paddle1Y);
    drawPaddles(paddle1T, paddle1Y, paddle2T, paddle2Y);
    delay(40);
  }
}

void moveBall(int paddle1Y, int paddle2Y, int paddle1T, int paddle2T) {
  TFT.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_BLACK);

  ballX += ballSpeedX;
  ballY += ballSpeedY;

  if (ballSpeedY == 0){
    ballSpeedY = (random(0,1)) ? random(-1,-5) : random(1, 5);
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
    delay(40);
  }

  if (ballX >= screenWidth - ballSize - paddleWidth && ballY >= paddle2Y && ballY <= paddle2Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
    ballSpeedY += random(-5, 5);
    delay(40);
  }

  if (ballX <= 0 || ballX >= screenWidth) {
    ballX = 160;
    ballY = 120;
    ballSpeedX = (random(0,1)) ? 6 : -6;
    ballSpeedY = (random(0,1)) ? random(-1,-4) : random(1, 4);
    paddle1Y, paddle1T, paddle2Y, paddle2T = 100;
  }

  drawBall();
}


void drawPaddles(int paddle1T, int paddle1Y, int paddle2T, int paddle2Y) {
  TFT.fillRect(0, paddle1T, paddleWidth, paddleHeight, ILI9341_BLACK);
  TFT.fillRect(screenWidth - paddleWidth, paddle2T, paddleWidth, paddleHeight, ILI9341_BLACK);

  TFT.fillRect(0, paddle1Y, paddleWidth, paddleHeight, ILI9341_GREEN);
  TFT.fillRect(screenWidth - paddleWidth, paddle2Y, paddleWidth, paddleHeight, ILI9341_GREEN);
}

void drawBall() {
  TFT.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_WHITE);
}
