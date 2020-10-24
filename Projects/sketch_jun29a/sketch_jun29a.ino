#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SPI.h>    
#include <MFRC522.h>   
#include <Ultrasonic.h>
#include <SoftwareSerial.h>

#define BUZZER 8
#define SS_PIN 53
#define RST_PIN 5

char condPY;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = { {'1','2','3','A'},
                        {'4','5','6','B'},
                        {'7','8','9','C'},
                        {'*','0','#','D'}
                      };
byte rowsPins[ROWS] = {22,23,24,25};
byte colsPins[COLS] = {26,27,28,29};
char cond = '0'; // Condicao de comunicacao entre BT (inicial = 0)

Keypad keypad = Keypad ( makeKeymap(keys), rowsPins, colsPins, ROWS, COLS );
SoftwareSerial BTSerial(3,4); // RX, TX

const String SENHA_ESPERADA_1 = "789D";
const String SENHA_ESPERADA_2 = "123A";
const String SENHA_ESPERADA_3 = "ABCD";
const String SENHA_ESPERADA_4 = "2158BD";
String SENHA_DIGITADA = "";

String IDtag = ""; //Variável que armazenará o ID da Tag
bool Permitido = false;
String TagsCadastradas[] = {"47af2e0", "b447b677", "cf5fcb49"};

LiquidCrystal lcd (A0,A1,A2,A3,A4,A5);
MFRC522 LeitorRFID(SS_PIN, RST_PIN);

unsigned long time;
Ultrasonic ultrasonic(12, 13);

void setup() {
  BTSerial.begin(38400);
  Serial.begin(9600);           // Inicializa a comunicação Serial
  SPI.begin();                  // Inicializa comunicacao SPI
  LeitorRFID.PCD_Init();        // Inicializa o leitor RFID
  pinMode(BUZZER, OUTPUT);      // Declara o pino do buzzer como saída

  lcd.begin(16,2);
  delay(700);
  InicioTeclado();
 
}

void loop() {  
//  Serial.println(0);
  LeituraRFID();      // Chama a função responsável por fazer a leitura das Tag's
  LeituraTeclado();   // Chama a função resposavel pela leitura da senha numerica  
  LeituraSensor();
  
 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InicioTeclado(){
  lcd.print("DIGITE SUA SENHA");
  lcd.setCursor(0,1);
  lcd.blink();
  SENHA_DIGITADA = "";
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LeituraTeclado(){
 
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
    case 'A':
    case 'B':
    case 'C':
    case 'D':
            SENHA_DIGITADA += customKey;
          //imrpime na tela o símbolo pressionado
          lcd.print("*");
          break;

    case '*':
            lcd.clear();
            InicioTeclado();
            break;

    case '#':
          lcd.clear();
           
          //se a senha digitada foi igual a ESPERADA           
          if(SENHA_ESPERADA_1 == SENHA_DIGITADA or
            SENHA_ESPERADA_2 == SENHA_DIGITADA or
            SENHA_ESPERADA_3 == SENHA_DIGITADA or
            SENHA_ESPERADA_4 == SENHA_DIGITADA)
          {

            acessoLiberadoTeclado();
            lcd.print("Senha Correta!!!   ");
            lcd.setCursor(0,1);
            lcd.print("   Bem-vindo");
           
          }
          else {

            acessoNegadoTeclado();
            lcd.print("Senha Incorreta!!! ");
            lcd.setCursor(0,1);
            lcd.print("Tente Novamente");
                         
          }
         
      delay(2000);
      lcd.clear();
      InicioTeclado();
     
  }
  }
}

void acessoLiberadoTeclado(){
 
  cond = '3'; // Variavel receberá '3' para ser lido pelo bluetooth SLAVE e fazer a verificação
  BTSerial.write(cond); // Passará o valor da variavel pelo Bluetooth para o MASTER
  //Permitido = false;  // Seta a variável Permitido como false novamente
 
}

void acessoNegadoTeclado(){
 
  cond = '4'; // Variavel receberá '2' para ser lido pelo bluetooth SLAVE e fazer a verificação
  BTSerial.write(cond); // Passará o valor da variavel pelo Bluetooth para o MASTER
 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LeituraRFID(){

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
 
  if(Permitido == true) acessoLiberadoRFID(); // Se a variável Permitido for verdadeira será chamada a função acessoLiberado()     
  else acessoNegadoRFID(); // Se não será chamada a função acessoNegado()
 
  delay(2000); // Aguarda 2 segundos para efetuar uma nova leitura
}

void acessoLiberadoRFID(){
 
  //Serial.println("Tag Cadastrada: " + IDtag); // Exibe a mensagem "Tag Cadastrada" e o ID da tag não cadastrada
  cond = '1'; // Variavel receberá '1' para ser lido pelo bluetooth SLAVE e fazer a verificação
  BTSerial.write(cond); // Passará o valor da variavel pelo Bluetooth para o MASTER
  efeitoPermitido();  // Chama a função efeitoPermitido()
  Permitido = false;  // Seta a variável Permitido como false novamente
 
}

void acessoNegadoRFID(){
 
  //Serial.println("Tag NAO Cadastrada: " + IDtag); // Exibe a mensagem "Tag NAO Cadastrada" e o ID da tag cadastrada
  cond = '2'; // Variavel receberá '2' para ser lido pelo bluetooth SLAVE e fazer a verificação
  BTSerial.write(cond); // Passará o valor da variavel pelo Bluetooth para o MASTER
  efeitoNegado(); // Chama a função efeitoNegado()
 
}

void efeitoPermitido(){  
 
  int qtd_bips = 3; // Definindo a quantidade de bips
  for(int j=0; j<qtd_bips; j++){
    
  // Ligando o buzzer com uma frequência de 1500 hz e ligando o led verde.
  tone(BUZZER,1500);
    
  delay(100);   
    
  // Desligando o buzzer e led verde.    
  noTone(BUZZER);
  delay(100);
  }  
}

void efeitoNegado(){  
 
  int qtd_bips = 1;  // Definindo a quantidade de bips
 
  for(int j=0; j<qtd_bips; j++){   
    
  // Ligando o buzzer com uma frequência de 500 hz e ligando o led vermelho.
  tone(BUZZER,500);
  delay(500);
  noTone(BUZZER);
  delay(500);
    
  }  
}

void efeitoSensorAtivado(){  

 int qtd_bips = 40;
 for(int j=0; j<qtd_bips; j++){
   
 tone(BUZZER,500);
   
 delay(20);  
   
     
 noTone(BUZZER);
 delay(20);
 }  
}

void LeituraSensor(){

 int distance;
 distance = ultrasonic.distanceRead();
 time = millis();
 if(distance < 50){
 delay(2000);
  if(time > 1000){
    Serial.println('1');
    
    
  
  delay(1000);
  }
  
 }
 }
//else {
  //Serial.println(ultrasonic.distanceRead());
  //delay(1000);
  
  //}

