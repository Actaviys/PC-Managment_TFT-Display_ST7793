#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <GParser.h> // Бібліотека для парсингу

#define MINPRESSURE 20
#define MAXPRESSURE 900

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
const int TS_LEFT=75,TS_RT=1000,TS_TOP=115,TS_BOT=926;

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button sensor_btn, on_off_btn; // Системні кнопки
Adafruit_GFX_Button right_mouse_btn, left_mouse_btn, mouse_ring_btn;// Кнопки управління
Adafruit_GFX_Button volume_slider_btn;// Кнопки повзунків


bool touchFlag = 0; // Флажок для сенсора
bool ButtFlag_OnOff = 0; // Для кнопки ON/OFF



void initButtonsFunk(){ // Функція ініціалізації кнопок
  sensor_btn.initButton(&tft, 20, 6, 40, 12, WHITE, GREEN, RED, "Sensor", 1);
  on_off_btn.initButton(&tft, 385, 25, 30, 30, WHITE, BROWN, DARK_BROWN, " ", 1);
  right_mouse_btn.initButton(&tft, 75, 229, 150, 20, WHITE, CYAN, BLUE, "<<<---", 1);
  mouse_ring_btn.initButton(&tft, 200, 229, 100, 20, WHITE, CYAN, BLUE, "___", 1);
  left_mouse_btn.initButton(&tft, 325, 229, 150, 20, WHITE, CYAN, BLUE, "--->>>", 1);

  sensor_btn.drawButton(touchFlag);
  on_off_btn.drawButton(ButtFlag_OnOff);
  right_mouse_btn.drawButton(false);
  mouse_ring_btn.drawButton(false);
  left_mouse_btn.drawButton(false);
}




void SliderVolume(int sldVal, int x=30, int y=30){ // Слайдер для регулювання гучності ///////////////////////////////////////////////////
  // tft.drawLine(25, 40, 25, 100, RED);
  // tft.drawRect(25, 30, 5, 150, RED);
  tft.fillRect(x-2, y-8, 4, 150, RED); // Полоска 
  volume_slider_btn.initButton(&tft, x, y+sldVal, 30, 15, WHITE, DARK_RED, RED, " ", 1);
  volume_slider_btn.drawButton(false);
  // tft.fillRect(x, y+1, 30, 15, DARK_RED);
}


void setup() {
  pinMode(LED1, OUTPUT); digitalWrite(LED1, touchFlag);
  Serial.begin(9600);
  tft.begin(0x7793); // write-only shield //240x400 ID=0x7793
  tft.setRotation(3);  //PORTRAIT(0) //HORIZONTAL(1) //PORTRAIT(2) //HORIZONTAL(3)
  tft.fillScreen(BLACK);

  initButtonsFunk();

  SliderVolume(20, 40, 50); // Слайдер
}






int sx, sy;  
void TouchScreenPositionSend(int x, int y){ // Надсилаю дані курсора в форматованому варіанті
  sx = map(x, 0, 382, 0, 1920); // Калібрування для виводу x
  sy = map(y, 9, 235, 0, 1080); // Калібрування для виводу y
  String resData = "touch_positions:";
  resData += sx;
  resData += ",";
  resData += sy;
  Serial.println(resData);
}



int pixel_x, pixel_y;     
bool Touch_getXY(void){
  TSPoint p = ts.getPoint(); // Touch_getXY() оновлює глобальні змінні
  pinMode(YP, OUTPUT);      //Відновити спільні шпильки
  pinMode(XM, OUTPUT);      //Тому що TFT контрольні штифти
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    static uint32_t tmr_xy = 0;
    if (millis() - tmr_xy > 80){ // Таймер на відправку даних
      tmr_xy = millis();
      pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); // Синхронізую дані сенсора, з екраном
      pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height()); // Синхронізую дані сенсора, з екраном
      if (touchFlag){
        TouchScreenPositionSend(pixel_x, pixel_y); // Надсилаю дані про позицію натиску на сенсор
        Serial.println("touch_state: 1"); // Натсилаю  якщо наниснуто на сенсор
      }
    }
  }
  return pressed;
}


String textInput = "";
void texts_actives(){
  tft.setCursor(10, 10);
  tft.setTextColor(CYAN);  tft.setTextSize(4);
  tft.print(textInput);

  tft.setCursor(100, 100);
  tft.setTextColor(WHITE);  tft.setTextSize(4);
  tft.print("");
}


void ValueButtonSend(String but, bool bState){ // Надсилаю значення кнопок
  String resValue = "value_buttons:";
  resValue += but;
  resValue += ",";
  resValue += bState;
  Serial.println(resValue);
}



void ControlsButtonList(){ // Контроль всіх кнопок
  bool down = Touch_getXY();
  sensor_btn.press(down && sensor_btn.contains(pixel_x, pixel_y));
  on_off_btn.press(down && on_off_btn.contains(pixel_x, pixel_y));

// Перевірки натискання на кнопки
  if (sensor_btn.justPressed()) { // Включення та відключення надсиланеня даних з сенсора
    touchFlag = !touchFlag;

    // ValueButtonSend("Sensor", touchFlag);
  } if (sensor_btn.justReleased()) sensor_btn.drawButton(touchFlag);


  if (on_off_btn.justPressed()) { // ON/OFF
    ButtFlag_OnOff = !ButtFlag_OnOff;
    if (ButtFlag_OnOff == 1) tft.fillRect(40, 80, 160, 80, RED); else tft.fillRect(40, 80, 160, 80, GREEN);
    digitalWrite(LED1, ButtFlag_OnOff);

    ValueButtonSend("On/Off", ButtFlag_OnOff);
  } if (on_off_btn.isPressed()) on_off_btn.drawButton(ButtFlag_OnOff);
}







void loop() {
  if (Serial.available()){ // Приймаю дані
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

  static uint32_t tmr = 0; // Таймер на відправку даних
  if (millis() - tmr > 110){ // Відправляю дані
    tmr = millis();
    texts_actives();
    ControlsButtonList();
    //
  }

}

