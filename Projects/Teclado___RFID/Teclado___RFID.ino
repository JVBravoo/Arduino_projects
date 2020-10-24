#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SPI.h>    
#include <MFRC522.h>   
#include <SoftwareSerial.h>

#define LED_VERDE 6
#define LED_VERMELHO 7
#define BUZZER 8
#define SS_PIN 53
#define RST_PIN 5

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {{'1','2','3'},
                          {'4','5','6'},
                          {'7','8','9'},
                          {'*','0','#'}};
byte rowsPins[ROWS] = {22,23,24,25};
byte colsPins[COLS] = {26,27,28};
char cond = '0';

Keypad keypad = Keypad ( makeKeymap(keys), rowsPins, colsPins, ROWS, COLS );
SoftwareSerial BTSerial(3,4); // RX, TX

const String SENHA_ESPERADA_1 = "26022018";
const String SENHA_ESPERADA_2 = "0000";
const String SENHA_ESPERADA_3 = "12345";
const String SENHA_ESPERADA_4 = "54321";
String SENHA_DIGITADA = "";
String IDtag = ""; //Variável que armazenará o ID da Tag
bool Permitido = false;
String TagsCadastradas[] = {"47af2e0", "b447b677", "cf5fcb49"};

LiquidCrystal lcd (A0,A1,A2,A3,A4,A5);
MFRC522 LeitorRFID(SS_PIN, RST_PIN); 

void setup() {
  
  BTSerial.begin(38400);
  Serial.begin(9600);             // Inicializa a comunicação Serial
  SPI.begin();                    // Inicializa comunicacao SPI 
  LeitorRFID.PCD_Init();          // Inicializa o leitor RFID
  pinMode(LED_VERDE, OUTPUT);     // Declara o pino do led verde como saída
  pinMode(LED_VERMELHO, OUTPUT);  // Declara o pino do led vermelho como saída
  pinMode(BUZZER, OUTPUT);        // Declara o pino do buzzer como saída

  lcd.begin(16,2);
  delay(700); 
  inicio();
  
}

void inicio(){
  lcd.print("SENHA AI VEI");
  lcd.setCursor(0,1);
  lcd.blink();
  SENHA_DIGITADA = "";
}

void loop() {  

  Leitura();        // Chama a função responsável por fazer a leitura das Tag's

  char customKey = keypad.getKey();
 
  if (customKey){
    
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
             //imrpime na tela o símbolo pressionado
             lcd.print("*");
             break;

      case '*':
              lcd.clear();
              inicio();
              break;

      case '#':
            lcd.clear();
             
            //se a senha digitada foi igual a ESPERADA              
            if(SENHA_ESPERADA_1 == SENHA_DIGITADA or
               SENHA_ESPERADA_2 == SENHA_DIGITADA or
               SENHA_ESPERADA_3 == SENHA_DIGITADA or
               SENHA_ESPERADA_4 == SENHA_DIGITADA)
            {
              
              lcd.print("Senha Correta!!!");
              lcd.setCursor(0,1);
              lcd.print("Pode entrar pai   ");
              
            } 
            else {

              lcd.print("Senha Incorreta!");
              lcd.setCursor(0,1);
              lcd.print("Tas errado visse");
                           
            }
            
        delay(2000);
        lcd.clear();
        inicio();
        
    }
  }
  
}


void Leitura(){

  IDtag = ""; // Inicialmente IDtag deve estar vazia.
  
  // Verifica se existe uma Tag presente
  if ( !LeitorRFID.PICC_IsNewCardPresent() || !LeitorRFID.PICC_ReadCardSerial() ) {
      delay(50);
      return;
  }
  
  // Pega o ID da Tag através da função LeitorRFID.uid e Armazena o ID na variável IDtag        
  for (byte i = 0; i < LeitorRFID.uid.size; i++) {        
      IDtag.concat(String(LeitorRFID.uid.uidByte[i], HEX));
  }        
  
  // Compara o valor do ID lido com os IDs armazenados no vetor TagsCadastradas[]
  for (int i = 0; i < (sizeof(TagsCadastradas)/sizeof(String)); i++) {
    
    if(  IDtag.equalsIgnoreCase(TagsCadastradas[i])  ){
      
        Permitido = true; //Variável Permitido assume valor verdadeiro caso o ID Lido esteja cadastrado
    
    }
  }       
  
  if(Permitido == true) acessoLiberado(); // Se a variável Permitido for verdadeira será chamada a função acessoLiberado()        
  else acessoNegado(); // Se não será chamada a função acessoNegado()
  
  delay(2000); // Aguarda 2 segundos para efetuar uma nova leitura
}

void acessoLiberado(){
  
  Serial.println("Tag Cadastrada: " + IDtag); // Exibe a mensagem "Tag Cadastrada" e o ID da tag não cadastrada
  cond = '1'; // Variavel receberá '1' para ser lido pelo bluetooth SLAVE e fazer a verificação
  BTSerial.write(cond); // Passará o valor da variavel pelo Bluetooth para o MASTER
  efeitoPermitido();  // Chama a função efeitoPermitido()
  Permitido = false;  // Seta a variável Permitido como false novamente
  
}

void acessoNegado(){
  
  Serial.println("Tag NAO Cadastrada: " + IDtag); // Exibe a mensagem "Tag NAO Cadastrada" e o ID da tag cadastrada
  cond = '2'; // Variavel receberá '2' para ser lido pelo bluetooth SLAVE e fazer a verificação
  BTSerial.write(cond); // Passará o valor da variavel pelo Bluetooth para o MASTER
  efeitoNegado(); // Chama a função efeitoNegado()
  
}

void efeitoPermitido(){  
  
  int qtd_bips = 3; // Definindo a quantidade de bips
  for(int j=0; j<qtd_bips; j++){
    
    // Ligando o buzzer com uma frequência de 1500 hz e ligando o led verde.
    tone(BUZZER,1500);
    digitalWrite(LED_VERDE, HIGH);  // Acendendo o led Verde
    delay(100);   
    
    // Desligando o buzzer e led verde.      
    noTone(BUZZER);
    digitalWrite(LED_VERDE, LOW);
    delay(100);
  }  
}

void efeitoNegado(){  
  
  int qtd_bips = 1;  // Definindo a quantidade de bips
  
  for(int j=0; j<qtd_bips; j++){   
    
    // Ligando o buzzer com uma frequência de 500 hz e ligando o led vermelho.
    tone(BUZZER,500);
    digitalWrite(LED_VERMELHO, HIGH);   // Acendendo o led Vermelho
    delay(500); 
      
    // Desligando o buzzer e o led vermelho.
    noTone(BUZZER);
    digitalWrite(LED_VERMELHO, LOW);
    delay(500);
    
  }  
}


















