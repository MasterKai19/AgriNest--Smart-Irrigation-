// Reconstructed from Smart_Irrigation_Code.pdf; see docs/SOURCE_NOTES.md.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "ControlLogic.h"

constexpr int DHT_PIN = 2;
constexpr int SOIL_PIN = A0;
constexpr int RAIN_PIN = 4;
constexpr int RELAY_PIN = 7;  // Active LOW: HIGH means pump OFF.
constexpr int RED_LED = 8;
constexpr int GREEN_LED = 9;
constexpr int BLUE_LED = 10;
constexpr int ECHO_PIN = 12;
constexpr int TRIG_PIN = 13;
constexpr unsigned long ECHO_TIMEOUT_US = 30000UL;

DHT dht(DHT_PIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Preload OFF before enabling the relay output; initialize it before sensors.
  digitalWrite(RELAY_PIN, HIGH);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RAIN_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  Serial.println(F("SMART IRRIGATION SYSTEM"));
  Serial.println(F("System Starting... Motor: OFF"));
  lcd.setCursor(0, 0);
  lcd.print(F("Smart Irrigation"));
  lcd.setCursor(0, 1);
  lcd.print(F("System Starting"));
  delay(2000);
  lcd.clear();
}

void loop() {
  const int soilValue = analogRead(SOIL_PIN);
  const bool rainDetected = digitalRead(RAIN_PIN) == LOW;
  const float temperature = dht.readTemperature();
  const float humidity = dht.readHumidity();

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  const IrrigationState s = evaluateIrrigation(
      soilValue, rainDetected, temperature, humidity, duration);

  digitalWrite(RELAY_PIN, s.motorRunning ? LOW : HIGH);
  digitalWrite(GREEN_LED, s.systemError ? HIGH : LOW);
  digitalWrite(RED_LED, !s.systemError && s.motorRunning ? HIGH : LOW);
  digitalWrite(BLUE_LED, !s.systemError && !s.motorRunning ? HIGH : LOW);

  Serial.println(F("------------------------------"));
  Serial.print(F("Soil Value      : ")); Serial.println(soilValue);
  Serial.print(F("Temperature     : "));
  if (isnan(temperature)) Serial.println(F("ERROR"));
  else { Serial.print(temperature); Serial.println(F(" C")); }
  Serial.print(F("Humidity        : "));
  if (isnan(humidity)) Serial.println(F("ERROR"));
  else { Serial.print(humidity); Serial.println(F(" %")); }
  Serial.print(F("Rain Status     : "));
  Serial.println(rainDetected ? F("RAIN DETECTED") : F("NO RAIN"));
  Serial.print(F("Distance        : "));
  if (s.distanceCm < 0) {
    Serial.println(F("NO READING"));
    Serial.println(F("Water Level     : NO READING"));
    Serial.println(F("Tank Status     : UNKNOWN"));
  } else {
    Serial.print(s.distanceCm); Serial.println(F(" cm"));
    Serial.print(F("Water Level     : "));
    Serial.print(s.waterLevelCm); Serial.println(F(" cm"));
    Serial.print(F("Tank Status     : "));
    Serial.println(s.tankLow ? F("LOW TANK") : F("WATER AVAILABLE"));
  }
  Serial.print(F("Soil Condition  : "));
  Serial.println(s.soilDry ? F("DRY") : F("WET"));
  Serial.print(F("System Status   : "));
  Serial.println(s.systemError ? F("ERROR") : F("OK"));
  Serial.print(F("Motor Status    : "));
  Serial.println(s.motorRunning ? F("RUNNING") : F("OFF"));
  Serial.print(F("Action          : "));
  if (s.systemError) Serial.println(F("System Error"));
  else if (s.tankLow) Serial.println(F("Tank Low - Motor Stopped"));
  else if (s.soilDry && rainDetected) Serial.println(F("Rain detected"));
  else if (s.motorRunning) Serial.println(F("Watering Soil"));
  else Serial.println(F("No Watering"));
  Serial.print(F("Red LED         : ")); Serial.println(digitalRead(RED_LED) ? F("ON") : F("OFF"));
  Serial.print(F("Blue LED        : ")); Serial.println(digitalRead(BLUE_LED) ? F("ON") : F("OFF"));
  Serial.print(F("Green LED       : ")); Serial.println(digitalRead(GREEN_LED) ? F("ON") : F("OFF"));

  lcd.clear();
  lcd.setCursor(0, 0);
  if (s.systemError) lcd.print(F("SYSTEM ERROR"));
  else if (s.tankLow) lcd.print(F("TANK LOW"));
  else if (s.motorRunning) lcd.print(F("Soil: DRY"));
  else if (rainDetected && s.soilDry) lcd.print(F("Dry + Rain"));
  else if (rainDetected) lcd.print(F("Wet + Rain"));
  else lcd.print(F("Soil: WET"));
  lcd.setCursor(0, 1);
  lcd.print(s.motorRunning ? F("Motor: RUNNING") : F("Motor: OFF"));
  delay(2000);
}
