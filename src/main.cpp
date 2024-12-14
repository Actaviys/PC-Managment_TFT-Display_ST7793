#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define BLACK          0x0000
#define BLUE           0x001F
#define RED            0xF800
#define GREEN          0x07E0
#define CYAN           0x07FF
#define MAGENTA        0xF81F
#define YELLOW         0xFFE0
#define WHITE          0xFFFF
#define BROWN          0x00cc6d
#define DARK_BROWN     0x228B22
#define DARK_BLUE      0x8B008B
#define DARK_RED       0x808080



// ВСІ сенсорній панелі та проводка ІНШІ
// копіювати та вставляти результати з TouchScreen_Calibr_native.ino
const int XP=8,XM=A2,YP=A3,YM=9; 
const int TS_LEFT=39,TS_RT=966,TS_TOP=115,TS_BOT=926;


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button prev_btn, home_btn, next_btn; // Функціональні клавіші
Adafruit_GFX_Button btn_0, btn_1, btn_2, btn_3, btn_4, btn_5, btn_6, btn_7, btn_8, btn_9; // Числові клавіші

int pixel_x, pixel_y;     //Touch_getXY() оновлює глобальні змінні


void texts_actives(){
  tft.setCursor(90, 120);
  tft.setTextColor(GREEN);  tft.setTextSize(4);
  tft.print("status_text");
}



void setup() {
  Serial.begin(9600);
  tft.begin(0x7793); // write-only shield //240x400 ID=0x7793
  tft.setRotation(3);  //PORTRAIT(0) //HORIZONTAL(1) //PORTRAIT(2) //HORIZONTAL(3)
  tft.fillScreen(BLACK);
  texts_actives();
}

void loop() {
  TSPoint p = ts.getPoint();
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
      static uint32_t tmr = 0;
      if (millis() - tmr > 80){ // Таймер на відправку даних
        tmr = millis();
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); // Перетворення положення натиску через map в розмір екрана
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
        String resData = "touch:";
        resData += pixel_x;
        resData += ",";
        resData += pixel_y;
        resData += ",";
        resData += "OK";
        Serial.println(resData);
    }
  }
  
}

