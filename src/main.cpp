#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <GParser.h> // Бібліотека для парсингу

#define MINPRESSURE 100
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


#define LED1 52 

// Сенсорні панелі та проводка ІНШІ
// копіювати та вставляти результати з TouchScreen_Calibr_native.ino
const int XP=9,XM=A3,YP=A2,YM=8; 
const int TS_LEFT=40,TS_RT=956,TS_TOP=115,TS_BOT=926;
// const int TS_LEFT=41,TS_RT=946,TS_TOP=106,TS_BOT=916;

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button on_btn, off_btn;



void initButtonsFunk(){
  on_btn.initButton(&tft,  60, 200, 50, 30, WHITE, GREEN, BLACK, "ON", 2);
  off_btn.initButton(&tft, 180, 200, 50, 30, WHITE, GREEN, BLACK, "OFF", 2);
  on_btn.drawButton(false);
  off_btn.drawButton(false);
}

void setup() {
  pinMode(LED1, OUTPUT);
  Serial.begin(9600);
  tft.begin(0x7793); // write-only shield //240x400 ID=0x7793
  tft.setRotation(3);  //PORTRAIT(0) //HORIZONTAL(1) //PORTRAIT(2) //HORIZONTAL(3)
  tft.fillScreen(BLACK);

  initButtonsFunk();
}




int sx, sy;  
void TouchScreenPositionSend(int x, int y){ // Надсилаю дані курсора в форматованому варіанті
  sx = map(x, 13, 400, 0, 1920); // Калібрування для виводу x
  sy = map(y, 10, 215, 0, 1080); // Калібрування для виводу y
  String resData = "touch_positions:";
  resData += sx;
  resData += ",";
  resData += sy;
  Serial.println(resData);
}

bool touchFlag = 1;
int pixel_x, pixel_y;     
bool Touch_getXY(void){
  TSPoint p = ts.getPoint(); // Touch_getXY() оновлює глобальні змінні
  pinMode(YP, OUTPUT);      //Відновити спільні шпильки
  pinMode(XM, OUTPUT);      //Тому що TFT контрольні штифти
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
      pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv має сенс для мене
      pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
      if (touchFlag){
        TouchScreenPositionSend(pixel_x, pixel_y);
      }
  }
  return pressed;
}


String textInput = "";
void texts_actives(){
  tft.setCursor(10, 10);
  tft.setTextColor(CYAN);  tft.setTextSize(4);
  tft.print(textInput);
}


void ValueButtonSend(String but){ // Надсилаю значення кнопок
  String resValue = "value_buttons:";
  resValue += but;
  Serial.println(resValue);
}


// void InputMesage(int inpData){

// }


void ControlsButtonList(){
  bool down = Touch_getXY();
  on_btn.press(down && on_btn.contains(pixel_x, pixel_y));
  off_btn.press(down && off_btn.contains(pixel_x, pixel_y));

  if (on_btn.justReleased()) on_btn.drawButton();
  if (on_btn.justPressed()) {
    on_btn.drawButton(true);
    tft.fillRect(40, 80, 160, 80, MAGENTA);

    touchFlag = !touchFlag;
    digitalWrite(LED1, touchFlag);
    ValueButtonSend("ON");
  }

  if (off_btn.justReleased()) off_btn.drawButton();
  if (off_btn.justPressed()) {
    off_btn.drawButton(true);
    tft.fillRect(40, 80, 160, 80, RED);
    ValueButtonSend("OFF");
      
  }
}







void loop() {
  if (Serial.available()){
    char buf[100];
    Serial.readBytesUntil('\n', buf, 100);
    GParser data_cmd(buf, ':');
    int ints[50];
    data_cmd.parseInts(ints);

    switch (ints[0])
    {
    case 0:
      tft.fillScreen(BLACK);
      initButtonsFunk();
      data_cmd.split();
      textInput = data_cmd[1];
      break;
      
    case 1:
      int st = ints[1];
      break;
    
    }
  }

  static uint32_t tmr = 0;
  if (millis() - tmr > 110){ // Таймер на відправку даних
    tmr = millis();
    texts_actives();
    ControlsButtonList();
    //
  }

}

