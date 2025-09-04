#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD dan Servo
LiquidCrystal_I2C lcd(0x27, 16, 2); // coba ganti ke 0x3F kalau tidak tampil
Servo servo;

// Pin
const int proximityPin = 2;
const int servoPin = 9;

// Status
enum State {SIAP, TERBUKA, TERTUTUP};
State status = SIAP;

const unsigned long jedaBuka = 2000;       // lama penutup tetap terbuka (ms)
unsigned long waktuMulaiTerbuka = 0;

void setup() {
  Serial.begin(9600);
  delay(200);

  // Inisialisasi LCD
  lcd.init();          // atau ganti lcd.begin(16,2); kalau masih error
  lcd.backlight();
  delay(100);

  servo.attach(servoPin);
  servo.write(0);  // Posisi awal tertutup
  delay(300);

  pinMode(proximityPin, INPUT);

  tampilkanSiap();
}

void loop() {
  int sensor = digitalRead(proximityPin);

  switch (status) {
    case SIAP:
      if (sensor == LOW) {   // sensor aktif (tergantung logika HIGH/LOW sensor)
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
  servo.write(120); // posisi buka
  delay(500);

  lcd.clear();
  delay(5);
  lcd.setCursor(0, 0);
  lcd.print("Penutup Terbuka");
}

void tutupPenutup() {
  Serial.println("Menutup kembali...");
  servo.write(0); // posisi tutup
  delay(500);

  lcd.clear();
  delay(5);
  lcd.setCursor(0, 0);
  lcd.print("Penutup Tertutup");
}
