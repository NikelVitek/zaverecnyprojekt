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
  for(int j = 45; j >= 42; j--) {
    pinMode(j, INPUT);
  }


  // Initialize food position
  placeFood();
  TFT.begin();
  TFT.fillScreen(ILI9341_BLACK);
}

void loop() {
  menu_start(); // Start the menu
}

void menu_start() {
  TFT.setRotation(1);
  TFT.setCursor(120, 120);
  TFT.println("Press Button");

  // Wait for a button press and start the corresponding game
  while (true) {
    btn1state = digitalRead(btn1);
    btn2state = digitalRead(btn2);
    btn3state = digitalRead(btn3);
    
    if (btn1state == HIGH) {
      game1_start(); // Start game 1
      break; // Exit the menu
    }
    if (btn2state == HIGH) {
      game2_start(); // Start game 2
      break; // Exit the menu
    }
    if (btn3state == HIGH) {
      game3_start(); // Start game 3
      break; // Exit the menu
    }
  }
}

int snakeX[100]; // Array for the snake's x coordinates
int snakeY[100]; // Array for the snake's y coordinates
int snakeLength = 3; // Initial snake length
int direction = 1; // Direction: 1 = Right, 2 = Down, 3 = Left, 4 = Up
int gridSize = 5;
int foodX, foodY; // Food coordinates

void game1_start() {
  snakeX[0] = 160; // Starting X position (middle of screen)
  snakeY[0] = 120; // Starting Y position (middle of screen)
  snakeX[1] = 150;
  snakeY[1] = 120;
  snakeX[2] = 140;
  snakeY[2] = 120;
while (true) {
    // Read button states
    btn1state = digitalRead(btn1); // Up
    btn2state = digitalRead(btn2); // Down
    btn3state = digitalRead(btn3); // Left
    btn4state = digitalRead(btn4); // Right

    // Change direction based on button press
    if (btn1state == HIGH && direction != 2) direction = 4; // Up
    if (btn2state == HIGH && direction != 4) direction = 2; // Down
    if (btn3state == HIGH && direction != 1) direction = 3; // Left
    if (btn4state == HIGH && direction != 3) direction = 1; // Right

    // Move the snake in the current direction
    moveSnake();

    // Check for collisions (walls or self)
    if (checkCollision()) {
      gameOver();
      return;
    }

    // Check if snake eats the food
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
      snakeLength++;
      placeFood(); // Place new food
    }

    // Clear screen and draw snake and food
    TFT.fillScreen(ILI9341_BLACK);
    drawSnake();
    drawFood();

    delay(100); // Control game speed
  }
}

void moveSnake() {
  // Move the snake by shifting each segment to the position of the previous segment
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  // Move the head of the snake based on the current direction
  switch (direction) {
    case 1: snakeX[0]++; break; // Right
    case 2: snakeY[0]++; break; // Down
    case 3: snakeX[0]--; break; // Left
    case 4: snakeY[0]--; break; // Up
  }
}

bool checkCollision() {
  // Check if the snake hits the walls
  if (snakeX[0] < 0 || snakeX[0] >= screenWidth || snakeY[0] < 0 || snakeY[0] >= screenHeight) {
    return true;
  }

  // Check if the snake hits itself
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      return true;
    }
  }

  return false;
}

void gameOver() {
  TFT.fillScreen(ILI9341_RED); // Game over screen
  TFT.setCursor(100, 100);
  TFT.setTextColor(ILI9341_WHITE);
  TFT.setTextSize(2);
  TFT.println("GAME OVER!");
  delay(2000); // Wait for 2 seconds before returning to menu
  menu_start(); // Go back to menu
}

void placeFood() {
  // Place the food at a random location on the screen
  foodX = random(0, screenWidth);
  foodY = random(0, screenHeight);
}

void drawSnake() {
  // Draw each segment of the snake
  for (int i = 0; i < snakeLength; i++) {
    TFT.fillRect(snakeX[i], snakeY[i], gridSize, gridSize, ILI9341_GREEN);
  }
}

void drawFood() {
  // Draw the food
  TFT.fillRect(foodX, foodY, gridSize, gridSize, ILI9341_RED);
}
void game2_loop() {
  
}

void game2_start() {
  

}




void game3_loop() {
  
}

void game3_start() {
  

}
