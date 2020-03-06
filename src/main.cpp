#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>

#include <TFT_eSPI.h>

#include "saxobanklogo.h"
#include "webpage.h"

const char *ssid = "xmas";
const char *password = "xmasxmas";

WebServer server(80);

uint32_t animDelay = 10;
uint8_t textFont = 4;
uint8_t textSize = 1;
String message = "God jul";

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

#define BALLS_COUNT 19

int balls[BALLS_COUNT][2] = {
  {25, 208},
  {55, 204},
  {100, 206},
  {32, 188},
  {70, 185},
  {88, 182},
  {42, 165},
  {67, 164},
  {92, 162},
  {50, 143},
  {80, 141},
  {50, 124},
  {75, 126},
  {52, 108},
  {80, 105},
  {65, 98},
  {65, 80},
  {56, 64},
  {70, 54}
};

#define COLORS_COUNT 6

uint32_t ballColors[COLORS_COUNT] = {
  TFT_RED,
  TFT_ORANGE,
  TFT_BLUE,
  TFT_YELLOW,
  TFT_WHITE,
  TFT_GREEN
};

#define STAR_COLORS_COUNT 4

uint32_t starColors[STAR_COLORS_COUNT] = {
  TFT_WHITE,
  TFT_LIGHTGREY,
  TFT_DARKGREY,
  TFT_LIGHTGREY
};

int starColor = 0;

void drawLogo()
{
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, 240, 135, ttgo);
}

void clearScreen()
{
  tft.fillScreen(TFT_BLACK);
}

void drawBranch(int y, int height, int radius)
{
  uint32_t color = 0x3c41;
  tft.fillTriangle(67 - radius, y, 67 + radius, y, 67, y - height, color);
}

void drawStar(int yOffset, uint32_t color)
{
  int x = 67;
  int y = yOffset + 30;
  int radius = 12;
  tft.fillTriangle(x - radius, y - radius / 2, x + radius, y - radius / 2, x, y + radius, color);
  tft.fillTriangle(x - radius, y + radius / 2, x + radius, y + radius / 2, x, y - radius, color);
}

void drawBall(int x, int y, uint32_t color)
{
  tft.fillCircle(x, y, 6, color);
}

void drawBalls(int y, uint32_t color)
{
  for(int i=0; i<BALLS_COUNT; i++)
  {
    drawBall(balls[i][0], y + balls[i][1], color);
  }
}

void drawRandomBall()
{
  int ball = random(BALLS_COUNT);
  int colorIndex = random(COLORS_COUNT);
  drawBall(balls[ball][0], balls[ball][1], ballColors[colorIndex]);
}

void animate(uint32_t ms)
{
  starColor = (starColor + 1) % STAR_COLORS_COUNT;
  drawStar(0, starColors[starColor]);
  drawRandomBall();
  drawRandomBall();
  drawRandomBall();
  drawRandomBall();
  drawRandomBall();
  delay(ms);
}

void drawTree(int y)
{
  tft.setRotation(0);

  clearScreen();

  tft.fillRect(57, y + 210, 20, 30, 0x7140);

  tft.setTextDatum(MC_DATUM);
  tft.setTextFont(textFont);
  tft.setTextSize(textSize);
  tft.drawString(message, 67, 225);

  drawBranch(y + 210, 60, 67);
  drawBranch(y + 180, 50, 50);
  drawBranch(y + 152, 50, 40);
  drawBranch(y + 125, 40, 30);
  drawBranch(y + 100, 30, 23);
  drawBranch(y + 80, 30, 15);
  drawBranch(y + 60, 30, 10);

  drawStar(y, TFT_WHITE);

  drawBalls(y, TFT_YELLOW);
}

void initDisplay()
{
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);

  //! The backlight has been initialized in the TFT_eSPI library
  // if (TFT_BL > 0) {
  //     pinMode(TFT_BL, OUTPUT);
  //     digitalWrite(TFT_BL, HIGH);
  // }

  drawLogo();
  delay(1500);
}

void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  String del(animDelay);
  String font(textFont);
  String size(textSize);
  s.replace("%animDelay%", del);
  s.replace("%message%", message);
  s.replace("%textFont%", font);
  s.replace("%textSize%", size);
  server.send(200, "text/html", s); //Send web page
}

void handlePost() {
  animDelay = server.arg("animDelay").toInt();
  textFont = server.arg("textFont").toInt();
  textSize = server.arg("textSize").toInt();
  message = server.arg("message");
  drawTree(0);
  handleRoot();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start");

  initDisplay();
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);  

  server.on("/", HTTP_GET, handleRoot);
  server.on("/", HTTP_POST, handlePost);
  server.begin();

  delay(1500);
  drawTree(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

  animate(animDelay);

  // for (int y = 0; y > -240; y--)
  // {
  //   drawTree(y);
  //   delay(10);
  // }


  // for (int i = 1; i < 100; i++)
  // {
  //   animate(100);
  // }
  // for (int i = 1; i < 1000; i++)
  // {
  //   animate(1);
  // }
}