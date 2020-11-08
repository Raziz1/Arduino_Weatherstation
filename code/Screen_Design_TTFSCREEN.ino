/**********************************************************************************************************
  Screen Design TTF Screen
  equires the LCDKIWI library

  Set the pins to the correct ones for your development shield or breakout board.
  This demo use the BREAKOUT BOARD only and use these 8bit data lines to the LCD,
  pin usage as follow:
                   LCD_CS  LCD_CD  LCD_WR  LCD_RD  LCD_RST  SD_SS  SD_DI  SD_DO  SD_SCK
      Arduino Uno    A3      A2      A1      A0      A4      10     11     12      13
  Arduino Mega2560    A3      A2      A1      A0      A4      10     11     12      13

                   LCD_D0  LCD_D1  LCD_D2  LCD_D3  LCD_D4  LCD_D5  LCD_D6  LCD_D7
      Arduino Uno    8       9       2       3       4       5       6       7
  Arduino Mega2560    8       9       2       3       4       5       6       7
*/

//====================Libraries====================
#include <LCDWIKI_GUI.h> //Core graphics lib
#include <LCDWIKI_KBV.h> //Hardware-specific lib
#include <SD.h> //SD lib
#include <SPI.h>

//====================Variables====================

//Define some colour values
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
//#define CYAN    0x07FF
//#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

String description;
String icon;
String temp;
String temp_max;
String temp_min;
String wind_speed;
String current_time;
String current_date;
String snow_day;

/*
  unsigned long millis_now;
  unsigned long millis_previous = 0;
*/

int millis_now;
int millis_previous = 0;

//Constructed Function for LCD___________________
LCDWIKI_KBV mylcd(ILI9486, A3, A2, A1, A0, A4); //model,cs,cd,wr,rd,reset

#define MAX_BMP 10 //bmp file num
#define FILENAME_LEN 20 //max fiel name length

const int __Gnbmp_height = 100;                 // 320 bmp hight
const int __Gnbmp_width  = 100;
uint32_t bmp_offset = 0;

char file_name[18][FILENAME_LEN];

// These read data from the SD card file and convert them to big endian
// (the data is stored in little endian format!)
uint16_t read_16(File fp)
{
  uint8_t low;
  uint16_t high;
  low = fp.read();
  high = fp.read();
  return (high << 8) | low;
}

uint32_t read_32(File fp)
{
  uint16_t low;
  uint32_t high;
  low = read_16(fp);
  high = read_16(fp);
  return (high << 8) | low;
}

bool analysis_bpm_header(File fp)
{
  if (read_16(fp) != 0x4D42)
  {
    //return false;
    return true;
  }
  //get bpm size
  read_32(fp);
  //get creator information
  read_32(fp);
  //get offset information
  bmp_offset = read_32(fp);
  //get DIB infomation
  read_32(fp);
  //get width and heigh information
  uint32_t bpm_width = read_32(fp);
  uint32_t bpm_heigh = read_32(fp);
  if (read_16(fp) != 1)
  {
    return false;
  }
  read_16(fp);
  if (read_32(fp) != 0)
  {
    return false;
  }
  return true;
}

// This procedure reads a bitmap and draws it to the screen
// its sped up by reading many pixels worth of data at a time
// instead of just one pixel at a time. increading the buffer takes
// more RAM but makes the drawing a little faster. 20 pixels' worth
// is probably a good place
#define PIXEL_NUMBER (20)

void draw_bmp_picture(File fp, int x, int y)
{
  uint16_t i, j, k, l, m = 0;
  uint8_t bpm_data[PIXEL_NUMBER * 3] = {0};
  uint16_t bpm_color[PIXEL_NUMBER];
  fp.seek(bmp_offset);
  for (i = 0; i < 100; i++)
  {
    for (j = 0; j < 100 / PIXEL_NUMBER; j++)
    {
      m = 0;
      fp.read(bpm_data, PIXEL_NUMBER * 3);
      for (k = 0; k < PIXEL_NUMBER; k++)
      {
        bpm_color[k] = mylcd.Color_To_565(bpm_data[m + 2], bpm_data[m + 1], bpm_data[m + 1]); //change to 565
        m += 3;
      }
      for (l = 0; l < PIXEL_NUMBER; l++)
      {
        mylcd.Set_Draw_color(bpm_color[l]);
        mylcd.Draw_Pixel((j * PIXEL_NUMBER + l) + x, (i) + y);
      }
    }
  }
}


//Function for drawing yellow squares_________________________________
void fill_rectangle_test(void) {
  mylcd.Set_Draw_color(YELLOW);
  mylcd.Fill_Round_Rectangle(5, 5, 315, 96, 8);
  mylcd.Set_Draw_color(WHITE);
  mylcd.Fill_Round_Rectangle (5, 101, 315, 261, 8);
  mylcd.Fill_Round_Rectangle (5, 266, 315, 426, 8);

}
//Functions for drawing clock_________________________________

void text_time(void) {
  //Text (TIME)_____________________________________
  mylcd.Set_Text_Mode(1);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(5);
  mylcd.Print_String(current_time, 75, 20);

  //Text (DATE)____________________________________
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(current_date, 100, 75);

}

//==================== Function for Drawing Weather ====================

void text_weather(void) {
  //Current Temp________________________
  //mylcd.Set_Text_Mode(1);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_Size(5);
  mylcd.Print_String(temp + "C", 110, 125);
  //Description_________________________
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(description, 110, 175);
  //Max Temp_________________________
  mylcd.Print_String("Max:" + temp_max + "C", 110, 200);
  //Min Temp_________________________
  mylcd.Print_String("Min:" + temp_min  + "C", 200, 200);
  //Percentage of Precipitation_________________________
  mylcd.Print_String("Wind: " + wind_speed + "km/h", 110, 225);
}

void drawIcon() {
  //Draw BMP FILES____________________________________________________________________________________
  int i = 0;
  if (icon == "01d") {
    i = 0;
  } else if (icon == "01n") {
    i = 1;
  } else if (icon == "02d") {
    i = 2;
  } else if (icon == "02n") {
    i = 3;
  } else if (icon == "03d") {
    i = 4;
  } else if (icon == "03n") {
    i = 5;
  } else if (icon == "04d") {
    i = 6;
  } else if (icon == "04n") {
    i = 7;
  } else if (icon == "09d") {
    i = 8;
  } else if (icon == "09n") {
    i = 9;
  } else if (icon == "10d") {
    i = 10;
  } else if (icon == "10n") {
    i = 11;
  } else if (icon == "11d") {
    i = 12;
  } else if (icon == "11n") {
    i = 13;
  } else if (icon == "13d") {
    i = 14;
  } else if (icon == "13n") {
    i = 15;
  } else if (icon == "50d") {
    i = 16;
  } else if (icon == "50n") {
    i = 17;
  }

  File bmp_file;
  bmp_file = SD.open(file_name[i]);
  //Serial.print(file_name[i]);

  if (!bmp_file)
  {
    mylcd.Set_Text_Back_colour(BLUE);
    mylcd.Set_Text_colour(WHITE);
    mylcd.Set_Text_Size(1);
    mylcd.Print_String("didnt find BMPimage!", 0, 10);
    while (1);
  }
  if (!analysis_bpm_header(bmp_file))
  {
    mylcd.Set_Text_Back_colour(BLUE);
    mylcd.Set_Text_colour(WHITE);
    mylcd.Set_Text_Size(1);
    mylcd.Print_String("bad bmp picture!", 0, 0);
    return;
  }

  mylcd.Set_Rotation(2);
  draw_bmp_picture(bmp_file, 215, 250);
  bmp_file.close();

}

//==================== Setup ====================
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mylcd.Init_LCD();
  mylcd.Fill_Screen(BLACK);
  fill_rectangle_test();

  strcpy(file_name[0], "01d.bmp");
  strcpy(file_name[1], "01n.bmp");
  strcpy(file_name[2], "02d.bmp");
  strcpy(file_name[3], "02n.bmp");
  strcpy(file_name[4], "03d.bmp");
  strcpy(file_name[5], "03n.bmp");
  strcpy(file_name[6], "04d.bmp");
  strcpy(file_name[7], "04n.bmp");
  strcpy(file_name[8], "09d.bmp");
  strcpy(file_name[9], "09n.bmp");
  strcpy(file_name[10], "10d.bmp");
  strcpy(file_name[11], "10n.bmp");
  strcpy(file_name[12], "11d.bmp");
  strcpy(file_name[13], "11n.bmp");
  strcpy(file_name[14], "13d.bmp");
  strcpy(file_name[15], "13n.bmp");
  strcpy(file_name[16], "50d.bmp");
  strcpy(file_name[17], "50n.bmp");

  //Init SD_Card________________________
  pinMode(53, OUTPUT);
  if (!SD.begin(53))
  {
    mylcd.Set_Text_Back_colour(RED);
    mylcd.Set_Text_colour(WHITE);
    mylcd.Set_Text_Size(1);
    mylcd.Print_String("SD Card Init fail!", 0, 0);
  }
}

//==================== Main Function ====================
void loop() {
  millis_now = millis();
  if (Serial.available() > 0) {
    // read the incoming byte:
    current_date = Serial.readStringUntil('T');
    current_time = Serial.readStringUntil(',');
    current_time.remove(5, 16);
    description =  Serial.readStringUntil(',');
    icon =  Serial.readStringUntil(',');
    temp =  Serial.readStringUntil(',');
    temp_max =  Serial.readStringUntil(',');
    temp_min =  Serial.readStringUntil(',');
    wind_speed =  Serial.readStringUntil('\n');
  }

  if (millis_now - millis_previous >= 5000) {
    millis_previous = millis_now;
    mylcd.Set_Rotation(0);
    fill_rectangle_test();
    text_time();
    text_weather();
    drawIcon();
  }
}
