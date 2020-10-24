#include <LiquidCrystal.h>  

int alpha;
int beta;
int charlie;
int delta;
int echo;
int foxtrot;

int led1;
int led2;
int led3;
int led_out;
int total_led;

int num1;
int num2;

LiquidCrystal lcd (12, 11, 5, 4, 3, 2);

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(A0, INPUT);
pinMode(A1, INPUT);
pinMode(A2, INPUT);
pinMode(A3, INPUT);
pinMode(13, OUTPUT);
pinMode(10, OUTPUT);
pinMode(9, OUTPUT);
pinMode(8, OUTPUT);
pinMode(7, OUTPUT);
pinMode(6, OUTPUT);
lcd.begin(16,2);

}

void loop() {
  // put your main code here, to run repeatedly:
  
alpha = digitalRead(13);
beta = digitalRead(10);
charlie = digitalRead(9);
delta = digitalRead(8);
echo = digitalRead(7);
foxtrot = digitalRead(6);
led1 = digitalRead(A0);
led2 = digitalRead(A1);
led3 = digitalRead(A2);
led_out = digitalRead(A3);

total_led = led1 + (led2 * 2) + (led3 * 4) + (led_out * 8); 
num1 = (alpha * 4) + (beta * 2) + charlie;
num2 = (delta * 4) + (echo * 2) + foxtrot;

int total = num1 + num2;

if ( total != total_led){
  lcd.print(num1);
  lcd.print("+");
  
  lcd.print(num2);
  lcd.print("=");
  
  lcd.print(total_led);
  lcd.print("[ERRO]");
  
  lcd.setCursor(0,1);
  lcd.print(num1);
  lcd.print("+");
  
  lcd.print(num2);
  lcd.print("=");
  lcd.print(total);
  
} else {
  lcd.print(num1);
  lcd.print("+");
  lcd.print(num2);
  lcd.print("=");
  lcd.print(total);

}

delay(800);
lcd.clear();

}

