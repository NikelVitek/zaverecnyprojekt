#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 48
#define TFT_CS 50
#define TFT_MOSI 51
#define TFT_CLK 46
#define TFT_RST 49
#define TFT_MISO 47
const int btn1 = 45;
const int btn2 = 44;
const int btn3 = 43;
const int btn4 = 42;
const int screenWidth = 320;
const int screenHeight = 240;
int btn1state = 0;
int btn2state = 0;
int btn3state = 0;
int btn4state = 0;

Adafruit_ILI9341 TFT = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  Serial.begin(9600);
  for (int j = 45; j >= 42; j--) {
    pinMode(j, INPUT);
  }
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
    btn1state = digitalRead(btn1);
    btn2state = digitalRead(btn2);
    btn3state = digitalRead(btn3);
    
    if (btn1state == HIGH) {
      game1_start();
      break;
    }
    if (btn2state == HIGH) {
      game2_start();
      break;
    }
    if (btn3state == HIGH) {
      game3_start();
      break;
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
  unsigned long debounceDelay = 750;
  
  while (true) {
    btn3state = digitalRead(btn3);
    btn4state = digitalRead(btn4);

    if (btn3state == HIGH && (millis() - lastDirectionChangeTime) > debounceDelay) {
      if(direction < 4){direction += 1;}
      else{direction=1;}
      lastDirectionChangeTime = millis();
    }
    if (btn4state == HIGH && (millis() - lastDirectionChangeTime) > debounceDelay) {
      if(direction > 1){direction -= 1;}
      else{direction = 4;}
      lastDirectionChangeTime = millis();
    }

    moveSnake();

    if (checkCollision()) {
      gameOver();
      return;
    }

    checkFoodEaten();

    drawSnake();
    delay(50);
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
    btn1state = digitalRead(btn1);
    btn2state = digitalRead(btn2);
    btn3state = digitalRead(btn3);
    btn4state = digitalRead(btn4);

    if (btn1state == HIGH || btn2state == HIGH || btn3state == HIGH || btn4state == HIGH) {
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

void game2_start() {
}

void game3_start() {
}
