#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h>
#include <Keypad.h>
//LCDWIKI_SPI mylcd(ST7735S, A5, A3, -1, A2, A4, A1, A3); //software spi,model,cs,cd,miso,mosi,reset,clk,led

//Adafruit_ST7735 mylcd = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Adafruit_ST7735 mylcd = Adafruit_ST7735(10, 9, 11, 13, 8);

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

int ballx = 50, bally = 50, oldballx, oldbally;
int ballsize = 5, xdir = 1, ydir = 1;
int playerx = 54, playery = 154, oldplayerx, oldplayery, playerWidth = 26, playerHeight = 2;
int rowNr = 6, colNr = 8;

typedef struct bricktype
{
  int x, y;
  int width, height;
  bool build;
  bool active;
} brick;

brick brickMatrix[6][8] = {{1, 1, 1, 1, 1, 1, 1, 1,},
  {1, 1, 1, 1, 1, 1, 1, 1,},
  {1, 1, 1, 1, 1, 1, 1, 1,},
  {1, 1, 1, 1, 1, 1, 1, 1,},
  {1, 1, 1, 1, 1, 1, 1, 1,},
  {1, 1, 1, 1, 1, 1, 1, 1,}
};

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {30, 34, 38, 42};
byte colPins[COLS] = {44, 48, 50, 52};

Keypad kp = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
char customKey;

/*void draw_brick_matrix(void)
  {
  mylcd.Fill_Screen(BLACK);
  for(int i = 0; i <= 120; i+=7)
    for(int j = 0; j <= 50; j+=3)
      mylcd.Draw_Rectangle(i, j, i+5, j+1);
  delay(500);
  }*/

void printMenu()
{
  
}

void goToNextLevel()
{
  
}

void updateLives()
{
  
}

void printScore()
{

}

void drawPlayer(void)
{
  mylcd.fillRect(playerx, playery, playerWidth, playerHeight, YELLOW);
}

void drawBall(void)
{
  //mylcd.Set_Draw_color(100, 100, 100);
  mylcd.fillRect(ballx, bally, ballsize, ballsize, BLUE);
}

void moveBall(void)
{
  oldballx = ballx;
  oldbally = bally;
  ballx += xdir;
  bally += ydir;
  if (oldballx <= ballx && oldbally <= bally) {
    mylcd.fillRect(oldballx, oldbally, ballsize, bally - oldbally, BLACK);
    mylcd.fillRect(oldballx, oldbally, ballx - oldballx, ballsize, BLACK);
  } else if (oldballx >= ballx && oldbally >= bally) {
    mylcd.fillRect(ballx + ballsize , oldbally, oldballx - ballx, ballsize, BLACK);
    mylcd.fillRect(oldballx, bally + ballsize, ballsize, oldbally - bally, BLACK);
  } else if (oldballx <= ballx && oldbally >= bally) {
    mylcd.fillRect(oldballx, oldbally, ballx - oldballx, ballsize, BLACK);
    mylcd.fillRect(oldballx, bally + ballsize, ballsize, oldbally - bally, BLACK);
  } else if (oldballx >= ballx && oldbally <= bally) {
    mylcd.fillRect(oldballx, oldbally, ballsize, bally - oldbally, BLACK);
    mylcd.fillRect(ballx + ballsize, oldbally, oldballx - ballx, ballsize, BLACK);
  }
}

void checkWallCollision(void)
{
  if (ballx + ballsize >= mylcd.width())
    xdir = -xdir;
  else if (bally <= 0)
    ydir = -ydir;
  else if (ballx <= 0)
    xdir = -xdir;
  else if (bally + ballsize >= mylcd.height())
    ydir = -ydir;
}

void checkPlayerCollision(void)
{
  int collision = 0;
  if (bally + ballsize == playery)
    if (ballx >= playerx - ballsize && ballx <= playerx + playerWidth + ballsize)
      collision = 1;
  if (collision)
  {
    ydir = -ydir;
  }
}

void checkGameOver(void)
{
  //customKey = kp.getKey();
  if (customKey)
    if (customKey == '#')
    {
      mylcd.fillScreen(BLACK);
      exit(0);
    }
    else if(customKey == '8')
      playerWidth = 30;
}

void movePlayer(void)
{
  oldplayerx = playerx;
  oldplayery = playery;

  customKey = kp.getKey();
  if (customKey) {
    if (customKey == '4')
      playerx -= 10;
    else if (customKey == '6')
      playerx += 10;
  }

  if (playerx >= oldplayerx)
    mylcd.fillRect(oldplayerx, oldplayery, playerx - oldplayerx, playerHeight, BLACK);
  if (playerx <= oldplayerx)
    mylcd.fillRect(playerx + playerWidth, playery, oldplayerx - playerx, playerHeight, BLACK);
}

void drawBricks()
{
  for (int i = 0; i < rowNr; i++)
    for (int j = 0 ; j < colNr; j++)
      if (brickMatrix[i][j].build == 1)
        mylcd.fillRect(brickMatrix[i][j].x, brickMatrix[i][j].y, brickMatrix[i][j].width, brickMatrix[i][j].height, RED);
}

void setBricksCoords()
{
  for (int i = 0; i < rowNr; i++)
    for (int j = 0; j < colNr; j++)
    {
      brickMatrix[i][j].x = 19*i+11;
      brickMatrix[i][j].y = 8*j+30;
      brickMatrix[i][j].width = 10;
      brickMatrix[i][j].height = 3;
      brickMatrix[i][j].build = 1;
    }
}

void checkBrickCollision()
{
  for (int i = 0; i < rowNr; i++)
    for (int j = 0; j < colNr; j++)
      if(brickMatrix[i][j].active)
      {
        brickMatrix[i][j].active = 0;
        mylcd.fillRect(brickMatrix[i][j].x, brickMatrix[i][j].y, brickMatrix[i][j].width, brickMatrix[i][j].height, BLACK);
        
      }
}

void setup() {

  Serial.begin(9600);
  //mylcd.Set_Draw_color(1111100000000000);
  //for(int i = 0; i < mylcd.Get_Display_Width(); i++)
  //for(int j = 0; j < mylcd.Get_Display_Height(); j++)
  //mylcd.Draw_Pixel(i, j);

  //mylcd.Init_LCD();
  mylcd.initR(INITR_BLACKTAB);

  mylcd.fillScreen(BLACK);

  drawBall();
  drawPlayer();

  setBricksCoords();
  drawBricks();

  //draw_brick_matrix();
  //rectangle_test();
}

void loop() {
  drawPlayer();
  drawBall();
  movePlayer();
  moveBall();
  checkWallCollision();
  checkPlayerCollision();
  checkGameOver();
  delay(5);
}