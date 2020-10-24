/******************************************************//
// BIBLIOTECAS //

#include <MCUFRIEND_kbv.h> // Biblioteca responsavel pelo LCD
#include <SoftwareSerial.h> // Biblioteca para abrir pinos para conexão Serial

//******************************************************//

//******************************************************//
// DEFINICOES //

#define PRETO   0x0000  // Define a cor preta no LCD
#define BRANCO 0xFFFF   // Define a cor branca no LCD
#define VERDE 0x07E0    // Define a cor verde no LCD
#define VERMELHO 0xF800 // Define a cor vermelho no LCD
#define TEAL 0x008B8B // Define a cor TEAL no LCD

//******************************************************//


MCUFRIEND_kbv tft; // Instancia de um objeto MCUFRIEND
SoftwareSerial BTSerial(10,11); // RX, TX

char cartao; // Variavel que fará a verificação do que estásendo lido pela comunicação Bluetooth

void setup() {
  
  BTSerial.begin(38400); // Inicializacao BTSerial
  Serial.begin(9600);

  // Inicializacao do LCD
  uint16_t ID = tft.readID(); // Leitura do codigo de identificacao do controlador
  tft.begin(ID);
  telaInicial();
  
 }
  
 void loop(){

  // Verificando a condicao do Bluetooth
  if(BTSerial.available()) {

    // variavel cartao recebendo a condicao lida(enviada) pelo Bluetooth
    cartao = BTSerial.read();

    if(cartao == '1') {
      acessoAutorizado();
      telaInicial();
    }
    else if(cartao == '2') {
      acessoNegado();
      telaInicial();
    }
    else {
      telaInicial();
    }
  }
  
 }

 void acessoAutorizado() {
  
  tft.setRotation(1);      // Define a posicao(rotaçao) do texto que ficara no LCD
  tft.fillScreen(VERDE);   // Preenche o LCD com a cor definida
  tft.setCursor(10,80);    // Define a posicao que o texto aparecera(linha, coluna)
  tft.setTextColor(PRETO); // Define a cor que o texto aparecera 
  tft.setTextSize(4.5);    // Define o tamanha do texto
  tft.print("   Acesso\n  Autorizado");
  delay(2000);
  
}

void acessoNegado() {
  
  tft.setRotation(1);
  tft.fillScreen(VERMELHO);
  tft.setCursor(10,80);
  tft.setTextColor(PRETO);
  tft.setTextSize(4.5);
  tft.print("   Acesso\n    Negado  ");
  delay(2000);
  
}

void telaInicial() {
  
  tft.setRotation(1);
  tft.fillScreen(TEAL);
  tft.setCursor(100,60);
  tft.setTextColor(BRANCO);
  tft.setTextSize(4.5);
  tft.print("PASSE");
  tft.setCursor(120,105);
  tft.setTextColor(BRANCO);
  tft.setTextSize(4.5);
  tft.print("SEU");
  tft.setCursor(100,150);
  tft.setTextColor(BRANCO);
  tft.setTextSize(4.5);
  tft.print("CARTAO");

}
