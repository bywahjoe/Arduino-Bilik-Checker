#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "ardupin.h"
#include <Adafruit_MLX90614.h>
//#define SHOWDEBUG

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo inservo;
Servo outservo;
int inNormal = 90;
int outNormal = 90;

int inkuBuka = 85;
int inkuTutup = 0;
int outkuBuka = 94;
int outkuTutup = 180;

int setMaxPerson = 2;
int statusPerson = setMaxPerson;

float suhuNormal = 37;
float cekSuhu;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  mlx.begin();
  inservo.attach(servo_masuk);
  outservo.attach(servo_keluar);
  pinMode(ledMerah, OUTPUT);
  pinMode(ledHijau, OUTPUT);
  checkLed();
  setDefaultServo();
  noticeSuhu();
  cekSuhu = mlx.readObjectTempC();

  pinMode(IR_suhu, INPUT);
  pinMode(IR_masuk, INPUT);
  pinMode(IR_keluar, INPUT);
  pinMode(IR_tolak, INPUT);
}

void loop() {
  checkLed();
#ifdef SHOWDEBUG
  Serial.print("IR - Suhu");
  Serial.println(digitalRead(IR_suhu));
  Serial.print("IR - Masuk");
  Serial.println(digitalRead(IR_masuk));
  Serial.print("IR - Keluar");
  Serial.println(digitalRead(IR_keluar));
  Serial.print("IR - Tolak");
  Serial.println(digitalRead(IR_tolak));
  cekSuhu = mlx.readObjectTempC() + 2;
  Serial.print("INDIKATOR SUHU: ");
  Serial.println(cekSuhu);
  Serial.print("CAPACITY : ");
  Serial.println(statusPerson);
#endif

  if (digitalRead(IR_suhu) == LOW) {
    delay(1000);
    cekSuhu = mlx.readObjectTempC() + 2;
    tampil();
    if (statusPerson > 0) {

      if (cekSuhu <= suhuNormal) {
        inBuka();
        lcd.setCursor(0, 1);
        lcd.print("SILAHKAN MASUK!");
        while (1) {
          if (digitalRead(IR_masuk) == LOW)break;
        };
        minPerson();
        delay(3000);
        inTutup();
      } else {
        outBuka();
        lcd.setCursor(0, 1);
        lcd.print("ANDA DITOLAK!");
        while (1) {
          if (digitalRead(IR_tolak) == LOW)break;
        };
        delay(3000);
        outTutup();
      }

    } else {
      outBuka();

      lcd.setCursor(0, 1);
      lcd.print("KAPASITAS PENUH!");
      while (1) {
        if (digitalRead(IR_tolak) == LOW)break;
      };
      delay(3000);
      outTutup();
    }

    noticeSuhu();

  } else {
    setDefaultServo();
  }

  if (digitalRead(IR_keluar) == LOW) {
    while (1) {
      if (digitalRead(IR_keluar) == HIGH) break;
    }
    delay(500);
    plusPerson();
    noticeSuhu();


  }

}
void plusPerson() {
  int plusPerson = statusPerson + 1;
  statusPerson = constrain(plusPerson, 0, setMaxPerson);
}
void minPerson() {
  int plusPerson = statusPerson - 1;
  statusPerson = constrain(plusPerson, 0, setMaxPerson);
}
void setDefaultServo() {
  inservo.write(inNormal);
  outservo.write(outNormal);
}
void tampil() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  //cekSuhu = mlx.readObjectTempC();
  lcd.print(cekSuhu);
  lcd.print("C ");
  delay(500);
}
void inBuka() {
  for (int i = inkuBuka; i >= inkuTutup; i -= 3) {
    inservo.write(i);
    delay(50);
  }

}
void inTutup() {
  for (int i = inkuTutup; i <= inkuBuka; i += 3) {
    inservo.write(i);
    delay(50);
  }

}
void outBuka() {
  for (int i = outkuBuka; i <= outkuTutup; i += 3) {
    outservo.write(i);
    delay(50);
  }

}
void outTutup() {
  for (int i = outkuTutup; i >= outkuBuka; i -= 3) {
    outservo.write(i);
    delay(50);
  }

}

void noticeSuhu() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("CEK SUHU ANDA");
  lcd.setCursor(1, 1);
  lcd.print("CAPACITY: ");
  lcd.print(statusPerson);
}
void checkLed() {
  if (statusPerson > 0) {
    digitalWrite(ledHijau, HIGH);
    digitalWrite(ledMerah, LOW);
  }
  else {
    digitalWrite(ledHijau, LOW);
    digitalWrite(ledMerah, HIGH);
  }

}
