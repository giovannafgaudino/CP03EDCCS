int pinoLDR = A0;
int tempSensor = A1;
int umidSensor = A2;
int pinoBuzzer = 13;
int pinoLedVerde = 10;
int pinoLedAmarelo = 9;
int pinoLedVermelho = 8;
int pinoLedEstoqueBaixo = 3; // Pino para o LED de estoque baixo
int pinoLedEstoqueMedio = 4; // Pino para o LED de estoque médio
int pinoLedEstoqueAlto = 5;  // Pino para o LED de estoque alto
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
int distance;
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

  Serial.begin(9600);
}

void loop() {
  valorLDR = analogRead(pinoLDR);
  char buffer[40];
  sprintf(buffer, "Resis. do LDR: %d ohm(s)", valorLDR);
  Serial.println(buffer);

  buzzerPitch = map(valorLDR, 1, 310, 100, 400);

  temp = analogRead(tempSensor);
  tempMap = map(((temp - 20) * 3.04), 0, 1023, -40, 125);
  char buffer2[40];
  sprintf(buffer2, "Temperatura: %d\xB0""C", tempMap);
  Serial.println(buffer2);

  buzzerPitch = map(tempMap, 1, 310, 100, 400);

  umid = analogRead(umidSensor);
  umidMap = map(umid, 0, 1023, 0, 100);
  char buffer3[40];
  sprintf(buffer3, "Umidade do ar: %d%%", umidMap);
  Serial.println(buffer3);

  buzzerPitch = map(umidMap, 1, 310, 100, 400);

  int condicoesInadequadas = 0;

  if (valorLDR < 1 || valorLDR > 103) {
    condicoesInadequadas++;
  }

  if (umidMap < 60 || umidMap > 80) {
    condicoesInadequadas++;
  }

  if (tempMap < 10 || tempMap > 15) {
    condicoesInadequadas++;
  }

  if (condicoesInadequadas == 1) {
    digitalWrite(pinoLedAmarelo, HIGH);
    digitalWrite(pinoLedVermelho, LOW);

    if (!buzzerAtivo) {
      tone(pinoBuzzer, buzzerPitch);
      buzzerAtivo = true;
      tempoBuzzer = millis();
    }

    digitalWrite(pinoLedVerde, LOW);

    if (millis() - tempoBuzzer >= 3000) {
      noTone(pinoBuzzer);
      buzzerAtivo = false;
    }

  } else if (condicoesInadequadas >= 2) {
    digitalWrite(pinoLedAmarelo, LOW);
    digitalWrite(pinoLedVermelho, HIGH);

    if (!buzzerAtivo) {
      tone(pinoBuzzer, buzzerPitch);
      buzzerAtivo = true;
    }

    digitalWrite(pinoLedVerde, LOW);

  } else {
    digitalWrite(pinoLedAmarelo, LOW);
    digitalWrite(pinoLedVermelho, LOW);

    digitalWrite(pinoLedVerde, HIGH);

    if (buzzerAtivo && millis() - tempoBuzzer >= 3000) {
      noTone(pinoBuzzer);
      buzzerAtivo = false;
    }
  }

  // Leitura do sensor ultrassônico
  digitalWrite(ultrasonicTriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTriggerPin, LOW);
  
  duration = pulseIn(ultrasonicEchoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < LIMIT_LOW) {
    // Estoque baixo
    digitalWrite(pinoLedEstoqueBaixo, HIGH);
    digitalWrite(pinoLedEstoqueMedio, LOW);
    digitalWrite(pinoLedEstoqueAlto, LOW);
  } else if (distance < LIMIT_MED) {
    // Estoque médio
    digitalWrite(pinoLedEstoqueBaixo, LOW);
    digitalWrite(pinoLedEstoqueMedio, HIGH);
    digitalWrite(pinoLedEstoqueAlto, LOW);
  } else {
    // Estoque alto
    digitalWrite(pinoLedEstoqueBaixo, LOW);
    digitalWrite(pinoLedEstoqueMedio, LOW);
    digitalWrite(pinoLedEstoqueAlto, HIGH);
  }

  delay(1000);
}