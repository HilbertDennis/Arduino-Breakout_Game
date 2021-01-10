#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h>
#include <Keypad.h>

Adafruit_ST7735 mylcd = Adafruit_ST7735(10, 9, 11, 13, 8);

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

int ballx = 20, bally = 110, oldballx, oldbally;
int ballsize = 6;
int xdir = -1, ydir = -1;
int playerx = 54, playery = 154, oldplayerx, oldplayery;
int playerWidth = 30, playerHeight = 3;
int rowNr = 6, colNr = 8;

//structura utilizata pentru a retine toate datele legate de caramizi
typedef struct bricktype
{
  int x, y;
  int width, height;
  int build;
  bool active;
} brick;

//matricea de caramizi
brick brickMatrix[6][8];

int level;    //variabila pentru nivele
int lives = 3;    //numarul de vieti pentru fiecare nivel

//matricile care descriu pozitionarea caramizilor pentru fiecare nivel
int level_1_matrix[6][8] = {{0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 1},
                            {0, 0, 0, 0, 0, 0, 0, 0}
                           };

int level_2_matrix[6][8] = {{1, 1, 0, 0, 0, 0, 1, 1},
                            {1, 1, 0, 0, 0, 0, 1, 1},
                            {1, 1, 0, 0, 0, 0, 1, 1},
                            {1, 1, 0, 0, 0, 0, 1, 1},
                            {1, 1, 0, 0, 0, 0, 1, 1},
                            {1, 1, 0, 0, 0, 0, 1, 1}
                           };

int level_3_matrix[6][8] = {{1, 1, 1, 1, 1, 1, 1, 1},
                            {1, 1, 0, 0, 0, 0, 1, 1},
                            {1, 1, 0, 1, 1, 0, 1, 1},
                            {1, 1, 0, 1, 1, 0, 1, 1},
                            {1, 1, 0, 0, 0, 0, 1, 1},
                            {1, 1, 1, 1, 1, 1, 1, 1}
                           };

int level_4_matrix[6][8] = {{1, 1, 1, 0, 0, 1, 1, 1,},
                            {1, 1, 1, 0, 0, 1, 1, 1,},
                            {1, 1, 0, 1, 1, 0, 1, 1,},
                            {1, 1, 0, 1, 1, 0, 1, 1,},
                            {1, 1, 1, 0, 0, 1, 1, 1,},
                            {1, 1, 1, 0, 0, 1, 1, 1,}
                           };

int level_5_matrix[6][8] = {{1, 1, 1, 1, 1, 1, 1, 1,},
                            {1, 1, 1, 1, 1, 1, 1, 1,},
                            {1, 1, 0, 0, 0, 0, 1, 1,},
                            {1, 1, 0, 0, 0, 0, 1, 1,},
                            {1, 1, 1, 1, 1, 1, 1, 1,},
                            {1, 1, 1, 1, 1, 1, 1, 1,}
                           };

//partea declarativa pentru keypad
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

//functia de printare a "meniului" care apare la inceput de joc
void printMenu()
{
  mylcd.fillScreen(BLACK);
  mylcd.setTextColor(MAGENTA);
  mylcd.setTextSize(2);
  mylcd.setCursor(20, 44);
  mylcd.println("ARDUINO");
  mylcd.setCursor(17, 64);
  mylcd.println("BREAKOUT");
  mylcd.setCursor(38, 84);
  mylcd.println("GAME");
  mylcd.setCursor(12, 114);
  mylcd.setTextSize(1);
  mylcd.println("Press \"5\" to start");
}

//functia de incarcare a nivelului
void loadLevel(int levelNr)
{
  if (levelNr == 1)
  {
    level = 1;
    mylcd.fillScreen(BLACK);
    mylcd.setTextColor(MAGENTA);
    mylcd.setTextSize(2);
    mylcd.setCursor(25, 54);
    mylcd.println("LEVEL 1");
    delay(2000);
    mylcd.fillScreen(BLACK);
  }
  else if (levelNr == 2)
  {
    level = 2;
    mylcd.fillScreen(BLACK);
    mylcd.setTextColor(MAGENTA);
    mylcd.setTextSize(2);
    mylcd.setCursor(25, 54);
    mylcd.println("LEVEL 2");
    delay(2000);
    mylcd.fillScreen(BLACK);
  }
  else if (levelNr == 3)
  {
    level = 3;
    mylcd.fillScreen(BLACK);
    mylcd.setTextColor(MAGENTA);
    mylcd.setTextSize(2);
    mylcd.setCursor(25, 54);
    mylcd.println("LEVEL 3");
    delay(2000);
    mylcd.fillScreen(BLACK);
  }
  else if (levelNr == 4)
  {
    level = 4;
    mylcd.fillScreen(BLACK);
    mylcd.setTextColor(MAGENTA);
    mylcd.setTextSize(2);
    mylcd.setCursor(25, 54);
    mylcd.println("LEVEL 4");
    delay(2000);
    mylcd.fillScreen(BLACK);
  }
  else if (levelNr == 5)
  {
    level = 5;
    mylcd.fillScreen(BLACK);
    mylcd.setTextColor(MAGENTA);
    mylcd.setTextSize(2);
    mylcd.setCursor(25, 54);
    mylcd.println("LEVEL 5");
    delay(2000);
    mylcd.fillScreen(BLACK);
  }
  else if (levelNr == 6)
  {
    level = 6;
    mylcd.fillScreen(BLACK);
    mylcd.setTextColor(MAGENTA);
    mylcd.setTextSize(2);
    mylcd.setCursor(20, 54);
    mylcd.print("GG");
    delay(200);
    mylcd.setCursor(50, 54);
    mylcd.print("mah");
    mylcd.setCursor(80, 74);
    delay(700);
    mylcd.print("G");
    delay(500);
    mylcd.setCursor(92, 74);
    mylcd.print("G");
    exit(0);

  }

  lives = 3;
  playerx = 54; playery = 154;
  ballx = 20;
  bally = 110;
  xdir = -1;
  ydir = -1;

  drawLives();
  drawBall();
  drawPlayer();

  setBricksCoords();
  drawBricks();
}

//asteapta pana cand butonul '5' este apasat pe keypad
void waitForStart()
{
  while (1) {
    customKey = kp.getKey();
    if (customKey == '5') {
      loadLevel(1);
      break;
    }
  }
}

//verifica daca trebuie sa actualizeze nivelul
void goToNextLevel()
{
  int changeLevel = 1;
  for (int i = 0; i < rowNr; i++)
    for (int j = 0 ; j < colNr; j++)
      if (brickMatrix[i][j].active && brickMatrix[i][j].build)
        changeLevel = 0;
  if (changeLevel || customKey == '*')
  {
    delay(1000);
    level++;
    loadLevel(level);
  }
}

//partea grafica pentru desenarea vietilor pe lcd
void drawLives()
{
  mylcd.fillCircle(8, 10, 3, YELLOW);
  mylcd.fillCircle(20, 10, 3, YELLOW);
  mylcd.fillCircle(32, 10, 3, YELLOW);
}

//actualizarea vietilor
void updateLives()
{
  if (lives == 2)
    mylcd.fillCircle(32, 10, 3, BLACK);
  else if (lives == 1)
    mylcd.fillCircle(20, 10, 3, BLACK);
  else if (lives == 0)
    mylcd.fillCircle(8, 10, 3, BLACK);
}

//desenarea player-ului la inceput de nivel
void drawPlayer(void)
{
  mylcd.fillRect(playerx, playery, playerWidth, playerHeight, YELLOW);
}

//desenarea bilei
void drawBall(void)
{
  mylcd.fillRect(ballx, bally, ballsize, ballsize, BLUE);
}

//functia care sterge de pe ecran pixelii ce nu mai sunt utilizati pentru a desena bila
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

//verificarea coliziunilor cu peretii
void checkWallCollision(void)
{
  if (ballx + ballsize >= mylcd.width())
    xdir = -xdir;
  if (bally <= 17)
    ydir = -ydir;
  if (ballx <= 0)
    xdir = -xdir;
  if (bally + ballsize >= mylcd.height())
    ydir = -ydir;
}

//verificarea coliziunii cu player-ul
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

//verifica daca bila cade pe langa player, fara sa-l atinga si scade vietile in cazul respectiv
void checkGameOver(void)
{
  if (bally + ballsize == playery + 1)
  {
    lives--;
    updateLives();
    if (lives == 0)
    {
      mylcd.setCursor(10, 64);
      mylcd.setTextColor(CYAN);
      mylcd.setTextSize(2);
      mylcd.print("GAME OVER");
      delay(1000);
      exit(0);
    }
    mylcd.fillRect(ballx, bally, ballsize, ballsize, BLACK);
    ballx = 20;
    bally = 110;
    xdir = -1;
    ydir = -1;
  }

  if (customKey)
    if (customKey == '#')
    {
      mylcd.fillScreen(BLACK);
      exit(0);
    }
}

//functia de miscare stanga/dreapta a player-ului
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

//setarea datelor despre caramizi
void setBricksCoords()
{
  for (int i = 0; i < rowNr; i++)
    for (int j = 0; j < colNr; j++)
    {
      brickMatrix[i][j].x = 19 * i + 11;
      brickMatrix[i][j].y = 8 * j + 30;
      brickMatrix[i][j].width = 10;
      brickMatrix[i][j].height = 3;

      if (level == 1)
        brickMatrix[i][j].build = level_1_matrix[i][j];
      else if (level == 2)
        brickMatrix[i][j].build = level_2_matrix[i][j];
      else if (level == 3)
        brickMatrix[i][j].build = level_3_matrix[i][j];

      if (brickMatrix[i][j].build)
        brickMatrix[i][j].active = true;
    }
}

//desenarea matricilor de caramizi
void drawBricks()
{
  for (int i = 0; i < rowNr; i++)
    for (int j = 0 ; j < colNr; j++)
      if (brickMatrix[i][j].build)
        mylcd.fillRect(brickMatrix[i][j].x, brickMatrix[i][j].y, brickMatrix[i][j].width, brickMatrix[i][j].height, RED);
}

//verificarea coliziunii cu caramizile
void checkBrickCollision()
{
  for (int i = 0; i < rowNr; i++)
    for (int j = 0; j < colNr; j++)
      if (brickMatrix[i][j].active && brickMatrix[i][j].build)
      {
        if (colTest(brickMatrix[i][j].x, brickMatrix[i][j].y)) {
          brickMatrix[i][j].active = false;
          mylcd.fillRect(brickMatrix[i][j].x, brickMatrix[i][j].y, brickMatrix[i][j].width, brickMatrix[i][j].height, BLACK);
        }

      }
}

//calculul coliziunii pe pixeli
bool colTest(int bx, int by)
{
  int col = 0;
  if (ballx == bx + 10 && bally + ballsize >= by && bally <= by + 3)
  {
    xdir = -xdir;
    col++;
  }
  if (bally + ballsize == by && ballx + ballsize >= bx && ballx <= bx + 10)
  {
    ydir = -ydir;
    col++;
  }
  if (ballx + ballsize == bx && bally + ballsize >= by && bally <= by + 3)
  {
    xdir = -xdir;
    col++;
  }
  if (bally == by + 3 && ballx + ballsize >= bx && ballx <= bx + 10)
  {
    ydir = -ydir;
    col++;
  }

  if (col > 0)
    return true;

  return false;
}

void setup() {

  //se initializeaza ecranul
  mylcd.initR(INITR_BLACKTAB);


  //printeaza meniul
  printMenu();
  //asteapta comanda de start
  waitForStart();

  //deseneaza vietile, bila si player-ul
  drawLives();
  drawBall();
  drawPlayer();

  //seteaza coordonatele caramizilor si le deseneaza pe ecran
  setBricksCoords();
  drawBricks();
}


void loop() {
  drawPlayer();
  drawBall();
  movePlayer();
  moveBall();
  checkBrickCollision();
  checkWallCollision();
  checkPlayerCollision();
  checkGameOver();
  goToNextLevel();
}