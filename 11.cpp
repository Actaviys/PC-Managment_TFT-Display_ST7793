#include <Arduino.h>

void setup() {
  // Ініціалізація серійної консолі
  Serial1.begin(9600);
}

void loop() {
  // Зразкові значення x і y
  int x = 120; // Наприклад, вхідне значення x
  int y = 200; // Наприклад, вхідне значення y

  // Виклик функції для перетворення координат
  int sx = mapToRange(x, 0, 240, 0, 1080);
  int sy = mapToRange(y, 0, 400, 0, 1920);

  // Виведення результатів у консоль
  Serial.print("sx: ");
  Serial.print(sx);
  Serial.print(", sy: ");
  Serial.println(sy);

  // Затримка для зручності читання результатів
  delay(1000);
}

// Функція для перетворення значення з одного діапазону в інший
int mapToRange(int value, int inMin, int inMax, int outMin, int outMax) {
  return map(value, inMin, inMax, outMin, outMax);
}
