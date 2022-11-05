#include <SoftwareSerial.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#define RX_PIN 255
#define TX_PIN 3
#define INVERTED 1
LiquidCrystal_I2C lcd(0x27,16,2);
const byte LINHAS = 4; //Número de linhas do teclado.
const byte COLUNAS = 4; //Número de colunas do teclado.
const float vazao = 25.6; //Constante de Vazão obtida experimentalmente.
// Matriz do Teclado
char SIMBOLOS[LINHAS][COLUNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'c','0','e','D'}, //'c' = clear(Limpa Tela) AND 'e' = enter(Confirmar)
};
byte PINOS_LINHA[LINHAS] = {9, 8, 7, 6}; //Pinos do Arduíno conectados com as Linhas.
byte PINOS_COLUNA[COLUNAS] = {5, 4, 3, 2};  //Pinos do Arduíno conectados com as Colunas.
//Agora vem a instância do keypad, responsável por capturar uma tecla pressionada.
Keypad customKeypad = Keypad( makeKeymap(SIMBOLOS), PINOS_LINHA, PINOS_COLUNA, LINHAS, COLUNAS);
int valvula = 11;
int led = 12;
String senhaDig = "0";
int limite = 1000;
String Numero = "0";
String repetir = "";
String repet = "";
float tempo;
int Nrepeticoes = 0;
//Agora vem a instância do display, responsável por mostrar a saída na tela.
SoftwareSerial displaySerial = SoftwareSerial(RX_PIN,TX_PIN,INVERTED);
void setup() {
  lcd.init(); 
  lcd.clear();
  lcd.setCursor(0,0); // Modifica Cursor para 0,0.
  lcd.backlight();
  pinMode(valvula,OUTPUT); // Seta a Válvula como Saída.
  pinMode(led,OUTPUT); // Seta o Led como Saída.
  digitalWrite(valvula,HIGH); // Válvula recebe HIGH.
  digitalWrite(led,LOW); // LED recebe LOW.
  lcd.print("Bem-vindo");
  lcd.setCursor(0,1); // Modifica Cursor para 0,1.
  lcd.print("Digital Tap");// put your setup code here, to run once:
  //Tempo de espera de inicialização
  delay(3000); // Delay de 3000ms
  inicio();
}
void inicio(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Volume:"); // Anota "Volume" no LCD.
}

void loop() {
  // put your main code here, to run repeatedly:
  char customKey = customKeypad.getKey();
  // Recebe os Valores do Teclado
  int v1 = 0;
  float vlCompara = 0;
  if(customKey){ 
    switch(customKey){
      //caso algum dessas teclas forem pressionadas...
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0':
            senhaDig+=customKey;
            lcd.print(customKey);
            break;
      case 'A':
            Nrepeticoes = 1;
            lcd.setCursor(0,1);
            lcd.print("1 Repeticao");
            break;
      case 'B':
            Nrepeticoes = 2;
            lcd.setCursor(0,1);
            lcd.print("2 Repeticoes");
            break;
      case 'C':
            Nrepeticoes = 3;
            lcd.setCursor(0,1);
            lcd.print("3 Repeticoes");
            break;
      case 'D':
            Nrepeticoes = 4;
            lcd.setCursor(0,1);
            lcd.print("4 Repeticoes");
            break;
     case 'c':
            lcd.clear();
            senhaDig="0";
            inicio();
            break;
     case 'e':
            vlCompara = senhaDig.toInt();
            if(vlCompara == 0 && Nrepeticoes != 0){
                lcd.clear();
                lcd.print("Insira um valor"); 
                delay(2000);
                lcd.clear();           
            }
            if(vlCompara >= limite && Nrepeticoes != 0){
                lcd.clear();
                lcd.print("Limite excedido"); 
                delay(2000);
                lcd.clear(); 
            }
            if(vlCompara >= limite && Nrepeticoes == 0){
                lcd.clear();
                lcd.print("Limite excedido"); 
                lcd.setCursor(0,1);
                lcd.print("Repeticao Nula");
                delay(2000);
                lcd.clear();
            }
            if(vlCompara < limite && Nrepeticoes == 0){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Repeticoes");
                lcd.setCursor(0,1);
                lcd.print("Indefinidas");
                delay(2000);
                lcd.clear();
            }
            if(vlCompara < limite && Nrepeticoes != 0 && vlCompara>1){
              lcd.clear();
              for(int i = 0; i < Nrepeticoes; i++){
              tempo = ((vlCompara*1000*1.045)/vazao);
              lcd.print("Espere o pedido");
              lcd.setCursor(0,1);
              lcd.print(vlCompara);
              digitalWrite(valvula,LOW);
              for (float a = 0; a < tempo; a++){
                delay(1);
              }     
              digitalWrite(valvula,HIGH);
              lcd.clear();
              lcd.print("Volte sempre");
              delay(3000);
              lcd.clear();
              }
            }
            senhaDig = "";
            Numero = "0";
            repetir = "";
            repet = "";
            vlCompara = 0;
            Nrepeticoes = 0;
            setup();
            break;
    }
  }
}
