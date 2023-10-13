#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

int _timeout;

LiquidCrystal lcd(8,9,10,11,12,13);
DS1307 RTC;
int temp, inc, hours1, minut, add = 5;
int next = A0;
int INC = A1;
int set_mad = A2;
int mrng =2;
int aft = 3;
int evng = 4;
int buzzer=6 ;
int pir = 7;

int HOUR, MINUT, SECOND;

void setup() {
  Serial.begin(9600);

  Wire.begin();
  RTC.begin();
  lcd.begin(16, 2);
  pinMode(INC, INPUT);
  pinMode(next, INPUT);
  pinMode(set_mad, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(mrng,OUTPUT);
  pinMode(aft,OUTPUT);
  pinMode(evng,OUTPUT);
  pinMode(pir, INPUT);
  lcd.setCursor(0, 0);
  lcd.print("Medicin reminder");
  lcd.setCursor(0, 1);
  lcd.print(" Using Arduino  ");
  delay(2000);
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__,__TIME__));
  }
}

void loop() {
  int temp = 0, val = 1, temp4;
  DateTime now = RTC.now();
  Serial.println(digitalRead(pir));
  if (digitalRead(set_mad) == 1)  
  {
    
    lcd.setCursor(0, 0);
    lcd.print("  Set Medicine  ");
    lcd.setCursor(0, 1);
    lcd.print("  Reminder time ");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time 1");
    defualt();
    time(1);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time 2");
    defualt();
    delay(1000);
    time(2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time 3");
    defualt();
    time(3);
    lcd.setCursor(0, 0);
    lcd.print("Medicin reminder");
    lcd.setCursor(0, 1);
    lcd.print("  time has set  ");
    delay(2000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(6, 0);
  lcd.print(HOUR = now.hour(), DEC);
  lcd.print(":");
  lcd.print(MINUT = now.minute(), DEC);
  lcd.print(":");
  lcd.print(SECOND = now.second(), DEC);
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);
  match();
  delay(200);
}

void defualt() {
  lcd.setCursor(0, 1);
  lcd.print(HOUR);
  lcd.print(":");
  lcd.print(MINUT);
  lcd.print(":");
  lcd.print(SECOND);
}


void time(int x) {
  int temp = 1, minuts = 0, hours = 0, seconds = 0;
  while (temp == 1) {
    if (digitalRead(INC) == 1) {
      HOUR++;
      if (HOUR == 24) {
        HOUR = 0;
      }
      while (digitalRead(INC) == 1)
       ;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time ");
    lcd.print(x);
    lcd.setCursor(0, 1);
    lcd.print(HOUR);
    lcd.print(":");
    lcd.print(MINUT);
    lcd.print(":");
    lcd.print(SECOND);
    delay(100);
    if (digitalRead(next) == 1) {
      hours1 = HOUR;
      EEPROM.write(add++, hours1);
      temp = 2;
      while (digitalRead(next) == 1)
        ;
    }
  }
  while (temp == 2) {
    if (digitalRead(INC) == 0) {
      MINUT++;
      if (MINUT == 60) { 
      MINUT = 0; }
      while (digitalRead(INC) == 0)
        ;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time ");
    lcd.print(x);
    lcd.setCursor(0, 1);
    lcd.print(HOUR);
    lcd.print(":");
    lcd.print(MINUT);
    lcd.print(":");
    lcd.print(SECOND);
    delay(100);
    if (digitalRead(next) == 0) {
      minut = MINUT;
      EEPROM.write(add++, minut);
      temp = 0;
      while (digitalRead(next) == 0)
        ;
    }
  }
  delay(1000);
}
void match() {
  int tem[17];
  for (int i = 11; i < 17; i++) {
    tem[i] = EEPROM.read(i);
  }
  if (HOUR == tem[11] && MINUT == tem[12] ){
    
    digitalWrite(mrng,HIGH);
    delay(100);
    digitalWrite(aft,LOW);
    digitalWrite(evng,LOW);
    beep();
    beep();
    beep();
    beep();
    lcd.setCursor(0, 0);
    lcd.print(" Take morning dose  ");
    lcd.setCursor(0, 1);
    lcd.print("     Medicine     ");
    beep();
    beep();
    beep();
    beep();
    }
    
  

  if (HOUR == tem[13] && MINUT == tem[14]) {
    
    digitalWrite(aft,HIGH);
    delay(100);
    digitalWrite(mrng,LOW);
    
    digitalWrite(evng,LOW);
    
    beep();
    beep();
    beep();
    beep();
    lcd.setCursor(0, 0);
    lcd.print(" Take afternoon dose  ");
    lcd.setCursor(0, 1);
    lcd.print("     Medicine     ");
    beep();
    beep();
    beep();
    beep();
  }
  



  if (HOUR == tem[15] && MINUT == tem[16] ) {
    
   digitalWrite(evng,HIGH);
   delay(100);
    digitalWrite(mrng,LOW);
    
    digitalWrite(aft,LOW);
    
    beep();
    beep();
    beep();
    beep();
    lcd.setCursor(0, 0);
    lcd.print("Take evening dose ");
    lcd.setCursor(0, 1);
    lcd.print("     Medicine    ");
    beep();
    beep();
    beep();
    beep();
    }
    if(digitalRead(pir)==1){
    digitalWrite(mrng,LOW);
    digitalWrite(aft,LOW);
    digitalWrite(evng,LOW);
    digitalWrite(buzzer,LOW);
    delay(50000);
    
    }
  
}


void beep() {
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(500);
}
