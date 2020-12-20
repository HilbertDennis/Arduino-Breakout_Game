#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library
LCDWIKI_SPI mylcd(ST7735S, A5, A3, -1, A2, A4, A1, A3); //software spi,model,cs,cd,miso,mosi,reset,clk,led

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

byte bitmap_array[625];
int ballx, bally;
int reverse = 0;

void rectangle_test(void)
{
  int i = 0;
  mylcd.Fill_Screen(BLACK);
  mylcd.Set_Draw_color(GREEN);
  for (i = 0; i < mylcd.Get_Display_Width() / 2; i += 4)
  {
    mylcd.Draw_Rectangle(i, (mylcd.Get_Display_Height() - mylcd.Get_Display_Width()) / 2 + i, mylcd.Get_Display_Width() - 1 - i, mylcd.Get_Display_Height() - (mylcd.Get_Display_Height() - mylcd.Get_Display_Width()) / 2 - i);
    delay(5);
  }
}

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

void move_ball(void)
{
  draw_ball();
  while(ballx + 3 < mylcd.Get_Display_Width())
  {
    delete_ball();
    ballx+=1;
    draw_ball();
    delay(10);
  }
  reverse = 1;
  while(ballx > 0)
  {
    delete_ball();
    ballx-=1;
    draw_ball();
    delay(10);
  }
}

void draw_ball(void)
{
  mylcd.Set_Draw_color(100, 100, 100);
  mylcd.Fill_Rectangle(ballx, bally, ballx+3, bally+3);
}

void delete_ball(void)
{
  mylcd.Set_Draw_color(0, 0, 0);
  if(!reverse)
    mylcd.Draw_Line(ballx, bally, ballx, bally+3);
  else
    mylcd.Draw_Line(ballx + 3, bally, ballx+3, bally+3);
}

void setup() {
  // put your setup code here, to run once:
  mylcd.Init_LCD();
  mylcd.Fill_Screen(BLACK);

  ballx = bally = 50;
  
  //draw_brick_matrix();
  move_ball();



  /*for (int i = 0; i < 625; i++)
    bitmap_array[i] = 8;*/
  //rectangle_test();
}

void loop() {
  // put your main code here, to run repeatedly:

}
