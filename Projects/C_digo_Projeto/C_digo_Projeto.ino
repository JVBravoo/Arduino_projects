#include <Ultrasonic.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#define TRIGGER_1 51
#define TRIGGER_2 53
#define ECHO_1 50
#define ECHO_2 52

const byte LINHAS = 4; //four rows
const byte COLUNAS = 3; //three columns
char SIMBOLOS[LINHAS][COLUNAS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
};
byte PINOS_LINHA[LINHAS] = {1, 2, 3, 4}; //connect to the row pinouts of the keypad
byte PINOS_COLUNA[COLUNAS] = {5, 6, 7};

const int SENHA_ESSPERADA = "3333";
int SENHA_DIGITADA = "";

Keypad  = (SIMBOLOS), PINOS_LINHA, PINOS_COLUNA, LINHAS, COLUNAS);

Ultrasonic sensor1(TRIGGER_1 ,ECHO_1);  // An ultrasonic sensor HC-04
Ultrasonic sensor2(TRIGGER_2 ,ECHO_2);

LiquidCrystal lcd(22, 24, 26, 28, 30 ,32)

void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);

  pinMode
void inicio(){
  displaySerial.print ("Senha: ");
}
}

void loop() {
  Serial.print("Sensor 01: ");
  Serial.print(sensor1.distanceRead()); // Prints the distance on the default unit (centimeters)
  Serial.println("cm");
  delay(1000);

  Serial.print("Sensor 02: ");
  Serial.print(sensor2.distanceRead()); // Prints the distance making the unit explicit
  Serial.println("cm");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Senha: ")
  lcd.setCursor(0,1);
  lcd.print(SENHA_DIGITADA);
  delay(1000);

  switch(customKey)
{
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':

        SENHA_DIGITADA += customKey;
        Serial.println(SENHA_DIGITADA);
        displaySerial.println(customKey);
        break;
    if (SENHA_ESPERADA == SENHA_DIGITADA)
    {
      Serial.println("Senha correta!");
      displaySerial.print("Senha correta!");
    }
    delay(2000);
    }
}

  char key = keypad.getKey();

  if (key) = Keypad.getKey();
    Serial.println(key);
}
{
  char teclaPressionada = keypad.getKey();
  if (teclaPressionada != NO_KEY)
  {
        lcd.setCursor(i, 0);
        lcd.print('*');
        i++;
  }
  }


  
  
}

