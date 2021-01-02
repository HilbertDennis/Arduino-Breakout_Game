#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h>
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

int ballx, bally, oldballx, oldbally, ballsize = 5, xdir = 1, ydir = 1;
int reverse = 0;

/*void draw_bitmap(void)
{
  mylcd.Fill_Screen(BLACK);
  for(int i = 0; i <= 120; i+=7)
    for(int j = 0; j <= 50; j+=3)
      mylcd.Draw_Bit_Map(i, j, 6, 2, (int)bitmap_array, 1);
  delay(500);
}*/

/*void draw_brick_matrix(void)
{
  mylcd.Fill_Screen(BLACK);
  for(int i = 0; i <= 120; i+=7)
    for(int j = 0; j <= 50; j+=3)
      mylcd.Draw_Rectangle(i, j, i+5, j+1);
  delay(500);
}*/
 
void move_ball_test(void)
{
  draw_ball();
  while(1)
  {
    oldballx = ballx;
    oldbally = bally;
    check_wall_collision();
    ballx+=xdir;
    bally+=ydir;
    delete_ball_optimized();
    draw_ball();
    delay(10);
  }
}

void draw_ball(void)
{
  //mylcd.Set_Draw_color(100, 100, 100);
  mylcd.fillRect(ballx, bally, ballsize, ballsize, BLUE);
}

//void delete_ball(void)
//{
//  //mylcd.Fill_Rectangle(oldballx, oldbally, oldballx+3, oldbally+3);
//  if(!reverse)
//    mylcd.Draw_Line(ballx, bally, ballx, bally+ballsize);
//  else
//    mylcd.Draw_Line(ballx + ballsize, bally, ballx+ballsize, bally+ballsize);
//}

void delete_ball_optimized(void)
{
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

void check_wall_collision(void)
{
  if(ballx + ballsize >= mylcd.width())
    xdir = -xdir;
  else if(bally <= 0)
    ydir = -ydir;
  else if(ballx <= 0)
    xdir = -xdir;
  else if(bally + ballsize >= mylcd.height())
    ydir = -ydir;
}

void setup() {
  Serial.begin(9600);

  Serial.print(F("Hello! ST77xx TFT Test"));
  // put your setup code here, to run once:

  //mylcd.Set_Draw_color(1111100000000000);
  //for(int i = 0; i < mylcd.Get_Display_Width(); i++)
  //for(int j = 0; j < mylcd.Get_Display_Height(); j++)
  //mylcd.Draw_Pixel(i, j);

  //mylcd.Init_LCD();
  mylcd.initR(INITR_BLACKTAB);
  mylcd.fillScreen(BLACK);

  ballx = bally = 50;
  
  //draw_brick_matrix();
  move_ball_test();



  /*for (int i = 0; i < 625; i++)
    bitmap_array[i] = 8;*/
  //rectangle_test();
}

void loop() {
  // put your main code here, to run repeatedly:
  

}