
// Arduino Pro mini.
// port 4, 5 to tmp102

#include <Wire.h>
#include <floatToString.h>


#define PIN_SCE   7 //Pin 3 on LCD 7
#define PIN_RESET 6 //Pin 4 on LCD 6
#define PIN_DC    5 //Pin 5 on LCD 5
#define PIN_SDIN  4 //Pin 6 on LCD 4
#define PIN_SCLK  3 //Pin 7 on LCD 3

#define LCD_COMMAND 0 
#define LCD_DATA  1

#define LCD_X     84
#define LCD_Y     48


// http://www.carlos-rodrigues.com/projects/pcd8544/

static const byte ASCII2[][5] = {
  {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
  ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c \
  ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j 
  ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
  ,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
};

char numbers[][5][17] = {{ 
   {0xc0, 0x30, 0x08, 0x04, 0x04,  0x81, 0x41, 0x41, 0x41, 0x41,  0x81, 0x02, 0x02, 0x04, 0x18,  0xe0, 0x00}
  ,{0xff, 0x00, 0x00, 0x00, 0xff,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0xff, 0x00, 0x00, 0x00,  0xff, 0x00}
  ,{0xff, 0x00, 0x00, 0x00, 0xff,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0xff, 0x00, 0x00, 0x00,  0xff, 0x00}
  ,{0xff, 0x00, 0x00, 0x00, 0xff,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0xff, 0x00, 0x00, 0x00,  0xff, 0x00}
  ,{0x07, 0x18, 0x20, 0x40, 0x40,  0x81, 0x82, 0x82, 0x82, 0x82,  0x81, 0x40, 0x40, 0x20, 0x18,  0x07, 0x00}
 },
{ 
   {0x00, 0x00, 0x00, 0x18, 0x14, 0xf2, 0x01, 0x01, 0x01, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  ,{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  ,{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  ,{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  ,{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x80, 0x80, 0x80, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
 }, 
{ 
   {0x00, 0x80, 0x70, 0x0c, 0x02,  0x81, 0x41, 0x21, 0x21, 0x41,  0x81, 0x02, 0x0c, 0x70, 0x80,  0x00, 0x00}
  ,{0xfc, 0x83, 0x80, 0x80, 0xfc,  0x03, 0x00, 0x00, 0x00, 0x00,  0x83, 0x7c, 0x00, 0x00, 0x03,  0xfc, 0x00}
  ,{0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x80, 0x60, 0x18, 0x06,  0x01, 0x80, 0x60, 0x18, 0x06,  0x01, 0x00}
  ,{0x00, 0x00, 0x80, 0x60, 0x18,  0x06, 0x01, 0x80, 0x60, 0x18,  0x06, 0x01, 0x00, 0x00, 0x00,  0x00, 0x00}
  ,{0xf8, 0x86, 0x81, 0x80, 0x80,  0x80, 0x8e, 0x89, 0x88, 0x88,  0x88, 0x88, 0x88, 0x88, 0x88,  0xf8, 0x00}
 },
{ 
   {0x1f, 0x11, 0x11, 0x11, 0x11,  0x11, 0x11, 0xd1, 0x31, 0x11,  0x01, 0x01, 0x01, 0xc1, 0x31,  0x0f, 0x00}
  ,{0x00, 0x00, 0x00, 0xc0, 0x30,  0x0c, 0x03, 0x00, 0x00, 0xc0,  0x30, 0x0c, 0x03, 0x00, 0x00,  0x00, 0x00}
  ,{0x00, 0x1c, 0x23, 0x40, 0x80,  0x40, 0x20, 0x20, 0x41, 0x81,  0x01, 0x01, 0x02, 0x04, 0x38,  0xc0, 0x00}
  ,{0xc0, 0x20, 0x20, 0x20, 0xe0,  0x00, 0x00, 0x00, 0x00, 0x80,  0x7f, 0x00, 0x00, 0x00, 0x00,  0xff, 0x00}
  ,{0x07, 0x18, 0x20, 0x40, 0x40,  0x81, 0x82, 0x82, 0x81, 0x80,  0x40, 0x40, 0x20, 0x10, 0x0e,  0x01, 0x00}
 },
{ 
   {0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x80, 0x60,  0x18, 0x06, 0x01, 0x01, 0xff,  0x00, 0x00}
  ,{0x00, 0x00, 0x00, 0x00, 0x80,  0x60, 0x18, 0x06, 0x01, 0x80,  0xe0, 0x00, 0x00, 0x00, 0xff,  0x00, 0x00}
  ,{0x80, 0x60, 0x18, 0x06, 0x01,  0x00, 0x60, 0x58, 0x46, 0x41,  0x7f, 0x00, 0x00, 0x00, 0x7f,  0xc0, 0x00}
  ,{0x07, 0x04, 0x04, 0x04, 0x04,  0x04, 0x04, 0x04, 0x04, 0x04,  0xfc, 0x00, 0x00, 0x00, 0xfc,  0x07, 0x00}
  ,{0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,  0xff, 0x80, 0x80, 0x80, 0xff,  0x00, 0x00}
 },
{ 
   {0xff, 0x01, 0x01, 0x01, 0xf1,  0x11, 0x11, 0x11, 0x11, 0x11,  0x11, 0x11, 0x11, 0x11, 0x11,  0x1f, 0x00}
  ,{0xff, 0x00, 0x00, 0x00, 0xff,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}
  ,{0x7f, 0x80, 0x80, 0x00, 0x83,  0x41, 0x41, 0x41, 0x41, 0x41,  0x82, 0x02, 0x04, 0x08, 0x30,  0xc0, 0x00}
  ,{0x80, 0x80, 0x80, 0x81, 0x81,  0x00, 0x00, 0x00, 0x00, 0x00,  0x81, 0x7e, 0x00, 0x00, 0x00,  0xff, 0x00}
  ,{0x0f, 0x10, 0x20, 0x40, 0x43,  0x82, 0x82, 0x82, 0x82, 0x82,  0x41, 0x40, 0x20, 0x10, 0x0c,  0x03, 0x00}
 },
 { 
   {0x00, 0x80, 0x60, 0x18, 0x04,  0x02, 0xc1, 0x21, 0x11, 0x11,  0x21, 0xc1, 0x02, 0x04, 0x08,  0xf0, 0x00}
  ,{0xfe, 0x01, 0x00, 0x00, 0xfc,  0x03, 0x00, 0x00, 0x00, 0x00,  0x00, 0x01, 0x01, 0x01, 0x01,  0x01, 0x00}
  ,{0xff, 0x00, 0x00, 0x00, 0x03,  0x82, 0x41, 0x41, 0x41, 0x41,  0x82, 0x02, 0x04, 0x08, 0x30,  0xc0, 0x00}
  ,{0xff, 0x00, 0x00, 0x00, 0xff,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0xff, 0x00, 0x00, 0x00,  0xff, 0x00}
  ,{0x03, 0x0c, 0x10, 0x20, 0x40,  0x41, 0x82, 0x82, 0x82, 0x82,  0x41, 0x40, 0x20, 0x10, 0x0c,  0x03, 0x00}
 },
 { 
   {0x1f, 0x11, 0x11, 0x11, 0x11,  0x11, 0x11, 0x11, 0x11, 0x11,  0x11, 0xf1, 0x01, 0x01, 0x01,  0xff, 0x00}
  ,{0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x80, 0x60, 0x18,  0x06, 0x01, 0x80, 0x60, 0x18,  0x07, 0x00}
  ,{0x00, 0x00, 0x00, 0x00, 0xe0,  0x18, 0x06, 0x01, 0x80, 0x60,  0x18, 0x06, 0x01, 0x00, 0x00,  0x00, 0x00}
  ,{0x00, 0x80, 0x78, 0x07, 0x00,  0x00, 0xe0, 0x1c, 0x03, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}
  ,{0x00, 0xff, 0x80, 0x80, 0x80,  0xfe, 0x01, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}
 },
 { 
   {0x00, 0xe0, 0x18, 0x04, 0x02,  0x82, 0x41, 0x21, 0x21, 0x41,  0x82, 0x02, 0x04, 0x18, 0xe0,  0x00, 0x00}
  ,{0x00, 0x3f, 0xc0, 0x00, 0x00,  0x0f, 0x10, 0x20, 0x20, 0x10,  0x0f, 0x00, 0x00, 0xc0, 0x3f,  0x00, 0x00}
  ,{0xc0, 0x30, 0x08, 0x05, 0x02,  0x80, 0x40, 0x40, 0x40, 0x40,  0x80, 0x02, 0x05, 0x08, 0x30,  0xc0, 0x00}
  ,{0xff, 0x00, 0x00, 0x00, 0xff,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0xff, 0x00, 0x00, 0x00,  0xff, 0x00}
  ,{0x03, 0x0c, 0x10, 0x20, 0x40,  0x41, 0x82, 0x82, 0x82, 0x82,  0x41, 0x40, 0x20, 0x10, 0x0c,  0x03, 0x00}
 },
 { 
   {0xe0, 0x18, 0x04, 0x02, 0x02,  0x81, 0x41, 0x21, 0x21, 0x41,  0x81, 0x02, 0x02, 0x04, 0x18,  0xe0, 0x00}
  ,{0x1f, 0x60, 0x80, 0x00, 0x00,  0x0f, 0x10, 0x20, 0x20, 0x10,  0x0f, 0x00, 0x00, 0x00, 0x00,  0xff, 0x00}
  ,{0x00, 0x00, 0x00, 0x01, 0x02,  0x04, 0x04, 0x04, 0x04, 0x04,  0x02, 0xff, 0x00, 0x00, 0x00,  0xff, 0x00}
  ,{0xe0, 0x20, 0x20, 0x20, 0xc0,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0xff, 0x00, 0x00, 0x00,  0xff, 0x00}
  ,{0x03, 0x0c, 0x10, 0x20, 0x40,  0x41, 0x82, 0x82, 0x82, 0x82,  0x41, 0x40, 0x20, 0x10, 0x0c,  0x03, 0x00}
 }
};

char coma [][6] = {
  {0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0xff,0x81,0x81,0x81,0xff},
};
 
int led = 13;
int tmp102address = 0x48;
char val[5];
char valm[5];
char valM[5];
float Min = 100.00;
float Max = 0.00;

// the setup routine runs once when you press reset:
void setup() {
 
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  Wire.begin();  
  LCDInit();
}

// the loop routine runs over and over again forever:
void loop() {
  LCDClear();
  float celcius = getTemp();
  if (celcius < Min) Min = celcius;
  if (celcius > Max) Max = celcius;
  
  floatToString(val,celcius,2,2);
  floatToString(valm,Min,2,2);
  floatToString(valM,Max,2,2);
  //LCDString("m:"); 
  LCDString(valm); 
  //LCDString("::"); 
  //LCDString(valM); 
  gotoXY(0,0);

  int n0=val[0]-48;
  LCDBitmap(numbers[n0],1);
  int n1=val[1]-48;
  LCDBitmap(numbers[n1],21);
  LCDBitmap_coma(coma,38);
  int n3=val[3]-48;
  LCDBitmap(numbers[n3],48);
  int n4=val[4]-48;
  LCDBitmap(numbers[n4],68);

  delay(500);              
}

void LCDBitmap(char my_array[][17],int pos){
  for (int index = 0 ; index < 5 ; index++){
    gotoXY(pos,1*(index+1));
    for (int index_row = 0 ; index_row < 17 ; index_row++)
       LCDWrite(LCD_DATA, my_array[index][index_row]);
    for ( int i = pos ; i < 64 ; i++) LCDWrite (LCD_DATA, 0x00); 
  }
}

void LCDBitmap_coma(char my_array[][6], int pos){
  for (int index = 0 ; index < 5 ; index++){
    gotoXY(pos,1*(index+1));
    for (int index_row = 0 ; index_row < 6 ; index_row++)
       LCDWrite(LCD_DATA, my_array[index][index_row]);
    for ( int i = pos ; i < 64 ; i++) LCDWrite (LCD_DATA, 0x00); 
  }
}

void gotoXY(int x, int y) {
  LCDWrite(0, 0x80 | x);  // Column.
  LCDWrite(0, 0x40 | y);  // Row.  ?
}

void LCDCharacter(char character) {
  LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding

  for (int index = 0 ; index < 5 ; index++)
    LCDWrite(LCD_DATA, ASCII2[character - 0x20][index]);
    //0x20 is the ASCII character for Space (' '). The font table starts with this character
  LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding
}

//Given a string of characters, one by one is passed to the LCD
void LCDString(char *characters) {
  while (*characters)
    LCDCharacter(*characters++);
}

//Clears the LCD by writing zeros to the entire screen
void LCDClear(void) {
  for (int index = 0 ; index < (LCD_X * LCD_Y / 8) ; index++)
    LCDWrite(LCD_DATA, 0x00);   
  gotoXY(0, 0); //After we clear the display, return to the home position
}

//This sends the magical commands to the PCD8544
void LCDInit(void) {
  //Configure control pins
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  //Reset the LCD to a known state
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);

  LCDWrite(LCD_COMMAND, 0x21); //Tell LCD that extended commands follow
  LCDWrite(LCD_COMMAND, 0xB0); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
  LCDWrite(LCD_COMMAND, 0x04); //Set Temp coefficent
  LCDWrite(LCD_COMMAND, 0x14); //LCD bias mode 1:48: Try 0x13 or 0x14
  LCDWrite(LCD_COMMAND, 0x20); //We must send 0x20 before modifying the display control mode
  LCDWrite(LCD_COMMAND, 0x0C); //Set display control, normal mode. 0x0D for inverse
}

//There are two memory banks in the LCD, data/RAM and commands. This 
//function sets the DC pin high or low depending, and then sends
//the data byte
void LCDWrite(byte data_or_command, byte data) {
  digitalWrite(PIN_DC, data_or_command); //Tell the LCD that we are writing either to data or a command
  //Send the data
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

float getTemp(){
  Wire.requestFrom(tmp102address,2);
  
  byte MSB = Wire.read ();
  byte LSB = Wire.read ();
  
  int TempSum = ((MSB << 8) | LSB) >> 4;
  float celcius = TempSum*0.0625;
  return celcius;
}
