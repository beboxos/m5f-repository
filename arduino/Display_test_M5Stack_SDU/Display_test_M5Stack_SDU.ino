#include <M5Stack.h>
#include "M5StackUpdater.h"

// the setup routine runs once when M5Stack starts up
void setup() {
  
  // initialize the M5Stack object
  M5.begin();
  checkSDUpdater();

  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();
  
  // Lcd display
  M5.Lcd.fillScreen(WHITE);
  delay(500);
  M5.Lcd.fillScreen(RED);
  delay(500);
  M5.Lcd.fillScreen(GREEN);
  delay(500);
  M5.Lcd.fillScreen(BLUE);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  delay(500);

  // text print
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("Display Test!");

  // draw graphic
  delay(1000);
  M5.Lcd.drawRect(100, 100, 50, 50, BLUE);
  delay(1000);
  M5.Lcd.fillRect(100, 100, 50, 50, BLUE);
  delay(1000);
  M5.Lcd.drawCircle(100, 100, 50, RED);
  delay(1000);
  M5.Lcd.fillCircle(100, 100, 50, RED);
  delay(1000);
  M5.Lcd.drawTriangle(30, 30, 180, 100, 80, 150, YELLOW);
  delay(1000);
  M5.Lcd.fillTriangle(30, 30, 180, 100, 80, 150, YELLOW);

}

// the loop routine runs over and over again forever
void loop(){

  //rand draw 
  M5.Lcd.fillTriangle(random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(0xfffe));

  M5.update();
}
