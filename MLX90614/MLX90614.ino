#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>

#define OLED_RESET 4
Adafruit_SSD1306 display1(OLED_RESET);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.println("Adafruit MLX90614 test");  
  mlx.begin();  
}

void loop() {
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
  Serial.println();

  display1.setTextColor(WHITE); display1.clearDisplay();  display1.setCursor(0, 0);
  display1.setTextSize(2); display1.print(mlx.readObjectTempC()); display1.print(" "); display1.print((char)247); display1.print("C\n");
  display1.setTextSize(2); display1.print(mlx.readObjectTempF()); display1.print(" "); display1.print((char)247); display1.print("F");
  display1.display();
  
  delay(500);
}
