#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

const int proximityPin = 2;
const int servoPin = 9;

enum State {SIAP, TERBUKA, TERTUTUP};
State status = SIAP;

const unsigned long jedaBuka = 2000;
unsigned long waktuMulaiTerbuka = 0;

void setup() {
  Serial.begin(9600);
  delay(200);

  lcd.init();
  lcd.backlight();
  delay(100);

  servo.attach(servoPin);
  servo.write(0);
  delay(300);

  pinMode(proximityPin, INPUT);

  tampilkanSiap();
}

void loop() {
  int sensor = digitalRead(proximityPin);

  switch (status) {
    case SIAP:
      if (sensor == LOW) {
        bukaPenutup();
        status = TERBUKA;
        waktuMulaiTerbuka = millis();
      }
      break;

    case TERBUKA:
      if (millis() - waktuMulaiTerbuka >= jedaBuka) {
        tutupPenutup();
        status = TERTUTUP;
      }
      break;

    case TERTUTUP:
      tampilkanSiap();
      status = SIAP;
      break;
  }

  delay(100);
}

void tampilkanSiap() {
  Serial.println(">> Smart Trash siap mendeteksi");
  lcd.clear();
  delay(5);
  lcd.setCursor(0, 0);
  lcd.print("  Smart Trash  ");
  lcd.setCursor(0, 1);
  lcd.print("     Siap...   ");
}

void bukaPenutup() {
  Serial.println("Sampah terdeteksi >> Penutup Terbuka");
  servo.write(120);
  delay(500);

  lcd.clear();
  delay(5);
  lcd.setCursor(0, 0);
  lcd.print("Penutup Terbuka");
}

void tutupPenutup() {
  Serial.println("Menutup kembali...");
  servo.write(0);
  delay(500);

  lcd.clear();
  delay(5);
  lcd.setCursor(0, 0);
  lcd.print("Penutup Tertutup");
}

