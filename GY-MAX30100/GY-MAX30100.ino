#include <MAX30100.h>
#include <MAX30100_BeatDetector.h>
#include <MAX30100_Filters.h>
#include <MAX30100_PulseOximeter.h>
#include <MAX30100_Registers.h>
#include <MAX30100_SpO2Calculator.h>
#include <Wire.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define REPORTING_PERIOD_MS     500
PulseOximeter pox;

const int numReadings=10; float filterweight=0.5;
uint32_t tsLastReport = 0; uint32_t last_beat=0; byte beat=0;
int readIndex=0; int average_beat=0; int average_SpO2=0;
bool calculation_complete=false; bool calculating=false; bool initialized=false;

void onBeatDetected() { last_beat=millis(); }
void initial_display() { if (not initialized)  { Serial.println("Initialized gy-MAX30100"); initialized=true; } }
void display_calculating(int j){ Serial.println(); lcd.clear(); lcd.setCursor(0, 0); lcd.print("Measuring...");  for (int i=0; i<=j; i++) { Serial.print(". "); } }
void display_values() 
{ 
  Serial.print("\n\nBPM: "); Serial.print(average_beat); Serial.print("\nSpO2: "); Serial.print(average_SpO2); Serial.print("%\n\n"); 
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("BPM: "); lcd.print(average_beat); lcd.setCursor(0, 1); lcd.print("SpO2: "); lcd.print(average_SpO2); lcd.print("%");  
  delay(3000);
}

void calculate_average(int beat, int SpO2) 
{
  if (readIndex==numReadings) { calculation_complete=true; calculating=false; initialized=false; readIndex=0; display_values(); }
  if (not calculation_complete and beat>30 and beat<220 and SpO2>50) {
    average_beat = filterweight * (beat) + (1 - filterweight ) * average_beat;
    average_SpO2 = filterweight * (SpO2) + (1 - filterweight ) * average_SpO2;
    readIndex++; display_calculating(readIndex);
  }
}

void setup()
{ 
  Serial.begin(9600); lcd.begin(16, 2); lcd.setCursor(0, 0); lcd.print("Pulse/Heart Rate"); lcd.setCursor(0, 1); lcd.print("& Blood Oxygen"); 
  pox.begin(); pox.setOnBeatDetectedCallback(onBeatDetected); 
}

void loop()
{
    pox.update(); 
    if ((millis() - tsLastReport > REPORTING_PERIOD_MS) and (not calculation_complete)) { calculate_average(pox.getHeartRate(), pox.getSpO2()); tsLastReport = millis(); }
    if ((millis()-last_beat>2000)) { calculation_complete=false; average_beat=0; average_SpO2=0; initial_display(); }
}
