#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // Pinos RS, Enable, D4, D5, D6, D7 do LCD

int pinoLDR = A0;
int tempSensor = A1;
int umidSensor = A2;
int pinoBuzzer = 13;
int pinoLedVerde = 10;
int pinoLedAmarelo = 9;
int pinoLedVermelho = 8;
int pinoLedEstoqueBaixo = 6; // Pino para o LED de estoque baixo
int pinoLedEstoqueMedio = 7; // Pino para o LED de estoque médio
int pinoLedEstoqueAlto = 8;  // Pino para o LED de estoque alto
int temp;
int tempMap;
int umid;
int umidMap;
int valorLDR;
int buzzerPitch;
int tempoBuzzer = 0;
bool buzzerAtivo = false;
int estoqueLowLimit = 30; // Defina o limite de estoque baixo
int estoqueMedLimit = 60; // Defina o limite de estoque médio
const int ultrasonicTriggerPin = 2; // Pino do sensor ultrassônico TRIGGER
const int ultrasonicEchoPin = 3;  // Pino do sensor ultrassônico ECHO
long duration;
float distance_cm;  // Distância em centímetros
int LIMIT_LOW = 10;  // Limite para estoque baixo (ajuste conforme necessário)
int LIMIT_MED = 20;  // Limite para estoque médio (ajuste conforme necessário)

void setup() {
  pinMode(pinoLDR, INPUT);
  pinMode(pinoBuzzer, OUTPUT);
  pinMode(pinoLedVerde, OUTPUT);
  pinMode(pinoLedAmarelo, OUTPUT);
  pinMode(pinoLedVermelho, OUTPUT);
  pinMode(pinoLedEstoqueBaixo, OUTPUT);
  pinMode(pinoLedEstoqueMedio, OUTPUT);
  pinMode(pinoLedEstoqueAlto, OUTPUT);
  pinMode(ultrasonicTriggerPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);

  lcd.begin(16, 2);  // Inicializa o LCD com 16 colunas e 2 linhas
  lcd.clear();
  lcd.print("Monitor de Estoque");
  lcd.setCursor(0, 1);
  lcd.print("e Condic. Ambient.");
  
  Serial.begin(9600);
}

void loop() {
  valorLDR = analogRead(pinoLDR);
  char buffer[40];
  sprintf(buffer, "Resis. do LDR: %d ohm(s)", valorLDR);
  lcd.setCursor(0, 0);
  lcd.print(buffer);

  // O restante do código permanece o mesmo...

  if (distance_cm < LIMIT_LOW) {
    // Estoque baixo
    digitalWrite(pinoLedEstoqueBaixo, HIGH);
    digitalWrite(pinoLedEstoqueMedio, LOW);
    digitalWrite(pinoLedEstoqueAlto, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Estoque: Baixo    ");
  } else if (distance_cm < LIMIT_MED) {
    // Estoque médio
    digitalWrite(pinoLedEstoqueBaixo, LOW);
    digitalWrite(pinoLedEstoqueMedio, HIGH);
    digitalWrite(pinoLedEstoqueAlto, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Estoque: Médio    ");
  } else {
    // Estoque alto
    digitalWrite(pinoLedEstoqueBaixo, LOW);
    digitalWrite(pinoLedEstoqueMedio, LOW);
    digitalWrite(pinoLedEstoqueAlto, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Estoque: Alto     ");
  }

  delay(1000);
}