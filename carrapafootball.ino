#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SoftwareSerial.h>
#include "timed.h"

//SoftwareSerial bluetooth(4, 5);
//Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);

/* Adaptando para minha montagem */
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 8);
SoftwareSerial bluetooth(0, 1); // RX, TX originais do arduino nano


static const unsigned char PROGMEM boneco1[] = { 0x08, 0xd2, 0x3d, 0xd2, 0x08 };
static const unsigned char PROGMEM boneco2[] = { 0x08, 0xd2, 0x3d, 0xd2, 0x20 };
static const unsigned char PROGMEM boneco3[] = { 0x20, 0xd2, 0x3d, 0xd2, 0x08 };

/* Cada minuto no jogo, vale 5 segundos de tempo real */
TimedExecution velocidade_jogo = TimedExecution(5000);

TimedExecution velocidade_boneco = TimedExecution(100);

/* Velocidade horizontal */
TimedExecution velocidade_boneco_h = TimedExecution(100);

int linhaBoneco = 0;
int colunaBoneco = 10;
int qtdPassosLinha = 1; /* O boneco anda sempre um pixel de cada vez */
int qtdPassosColuna = 6;
int fimSprintBoneco = linhaBoneco + qtdPassosLinha;
int fimSprintLateralBoneco = colunaBoneco + qtdPassosColuna;
int linhaBola = 12;
int colunaBola = 30;
int qtdPassosBola = 15;
int qtd_interacoes = 0;
int tempo = 0;
int qtdGols = 0;
const int linhaGol = 83;
const int colunaGol = 15;
const int tamanhoGol = 15;


/* Direcoes de movimento */
char direcao_v[] = "";
char direcao_h[] = "";

void desenhaBoneco(int linhaBoneco, int colunaBoneco){
  display.drawBitmap(linhaBoneco, colunaBoneco, boneco1, 8, 5, BLACK);
}

void apagaBoneco(int linhaBoneco, int colunaBoneco){
  display.drawBitmap(linhaBoneco, colunaBoneco, boneco1, 8, 5, WHITE);
}

void desenhaGol(){
    for (int i=colunaGol; i < colunaGol + tamanhoGol; i++){
        display.drawPixel(linhaGol, i, BLACK);
        display.drawPixel(linhaGol+1, i, BLACK);
    }
    display.display();
}

void desenhaPlacar(){
  display.print(tempo % 45);
  display.print("[");
  display.print((tempo / 45) + 1);
  display.print("]");
  display.print(qtdGols);
  display.println(" Gol(s)");
  if (tempo >= 90){
    display.clearDisplay();
    display.println("Fim de jogo!");
    display.display();
    delay(5000);
    display.clearDisplay();
    qtdGols = 0;
    qtd_interacoes = 0;
    tempo = 0;
    reset();
  }
  display.display();
}

void desenhaBola(int linhaBola, int colunaBola){
  display.drawPixel(linhaBola, colunaBola, BLACK);
  display.drawPixel(linhaBola+1, colunaBola, BLACK);
  display.drawPixel(linhaBola, colunaBola+1, BLACK);
  display.drawPixel(linhaBola+1, colunaBola+1, BLACK);
  display.display();
}

void andar_horizontalmente(const char *direcao){
  if (velocidade_boneco_h.expired()){
    if (direcao_h[0] == 'd'){
     fimSprintLateralBoneco = colunaBoneco + qtdPassosColuna;
     if (fimSprintLateralBoneco > 48 - 5){
       fimSprintLateralBoneco = 48 - 5;
     }
      apagaBoneco(linhaBoneco, colunaBoneco);
      desenhaBoneco(linhaBoneco, ++colunaBoneco);
    }
    else if (direcao_h[0] == 'e'){
     fimSprintLateralBoneco = colunaBoneco - qtdPassosColuna;
     if (fimSprintLateralBoneco < 0 + 5){
        fimSprintLateralBoneco = 0 + 5;
      }
      apagaBoneco(linhaBoneco, colunaBoneco);
      desenhaBoneco(linhaBoneco, --colunaBoneco);
    }
  }
}

void andar_verticalmente(char *direcao){
  if (velocidade_boneco.expired()){
    if (direcao_v[0] == 'f'){
      fimSprintBoneco = linhaBoneco + qtdPassosLinha;
      if (fimSprintBoneco > 84 - 8){
        fimSprintBoneco = 84 - 8;
      }
      apagaBoneco(linhaBoneco, colunaBoneco);
      desenhaBoneco(++linhaBoneco, colunaBoneco);
    }
    else if (direcao_v[0] == 'b'){
      fimSprintBoneco = linhaBoneco - qtdPassosLinha;
      if (fimSprintBoneco < 0){
        fimSprintBoneco = 0;
      }
      apagaBoneco(linhaBoneco, colunaBoneco);
      desenhaBoneco(--linhaBoneco, colunaBoneco);

    }
  }
}

void reset(){
    linhaBoneco = 0;
    colunaBoneco = 10;
    linhaBola = 12;
    colunaBola = 30;
    desenhaBola(linhaBola, colunaBola);
    desenhaBoneco(linhaBoneco, colunaBoneco);
    desenhaPlacar();
    display.display();
}

void chutar(char *direcao){
  if ((linhaBola >= linhaBoneco && linhaBola <= linhaBoneco + 8) && (colunaBola >= colunaBoneco && colunaBola <= colunaBoneco + 5)){
      if (direcao == "f"){
        int fimSprintBola = linhaBola + qtdPassosBola;
        for (int i = linhaBola; i < fimSprintBola; i++){
          display.clearDisplay();
          desenhaBoneco(linhaBoneco, colunaBoneco);
          desenhaBola(linhaBola, colunaBola);
          desenhaPlacar();
          desenhaGol();
          delay(60);
          linhaBola = i;
        }
      }
      else if(direcao == "d"){
        int fimSprintBola = linhaBola + qtdPassosBola;
        for (int i = linhaBola; i < fimSprintBola; i++){
            colunaBola += 1;
            display.clearDisplay();
            desenhaBoneco(linhaBoneco, colunaBoneco);
            desenhaBola(i, colunaBola);
            desenhaPlacar();
            delay(60);
            linhaBola = i;
          }
      }
      else if (direcao == "e"){
        int fimSprintBola = linhaBola + qtdPassosBola;
        for (int i = linhaBola; i < fimSprintBola; i++){
            colunaBola -= 1;
            display.clearDisplay();
            desenhaBoneco(linhaBoneco, colunaBoneco);
            desenhaBola(i, colunaBola);
            desenhaPlacar();
            delay(60);
            linhaBola = i;
          }
        }
    if (linhaBola >= 84 && (colunaBola >= colunaGol && colunaBola <= colunaGol + tamanhoGol)){
      display.println("GoOooOoOoOooool");
      display.display();
      delay(600);
      qtdGols++;
      reset();
    }
    else if (linhaBola >= 84 || colunaBola >= 48 || linhaBola < 0 || colunaBola < 0){
      display.println("Fora");
      display.display();
      delay(600);
      linhaBola = 12;
      colunaBola = 25;
      reset();
    }
  }
  else {
    display.clearDisplay();
    display.println("Que furada!");
    display.display();
    delay(600);
    display.clearDisplay();
  }
}

char* escolhe_direcao(){
  int value = random(0,3);
  if (value == 0){
    return "f";
  }
  else if (value == 1){
    return "d";
  }
  else if (value == 2){
    return "e";
  }
  else {
    return "p";
  }
}


void setup(){
  Serial.begin(9600);
  bluetooth.begin(9600);
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.setCursor(1, 15);
  display.setTextColor(BLACK);
  display.println("femf.com.br");
  display.display();
  delay(1000);
  display.clearDisplay();

  display.setCursor(1, 1);
  desenhaPlacar();
  desenhaGol();
  desenhaBoneco(linhaBoneco, colunaBoneco);
  display.display();
}



void loop(){
  desenhaBola(linhaBola, colunaBola);
  char bluetooth_value;
    bluetooth_value = bluetooth.read();
    switch(bluetooth_value){
      case 'f':
        direcao_v[0] = 'f';
        break;
      case 'd':
        direcao_h[0] = 'd';
        break;
      case 'e':
        direcao_h[0] = 'e';
        break;
      case 'b':
        direcao_v[0] = 'b';
        break;
      case 's':
        direcao_v[0] = ' ';
        direcao_h[0] = ' ';
        break;
      case 'c':
        chutar(escolhe_direcao());
        break;
      default: break;
    }
  andar_verticalmente(direcao_v);
  andar_horizontalmente(direcao_h);
  //qtd_interacoes++;
  if (velocidade_jogo.expired()){
    tempo++;
  }
  display.display();
}
