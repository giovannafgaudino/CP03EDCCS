//Circuito refere a 'CP03.2.jpg';
// Código para a parte LCD baseado nos dados do 'tinkercad1.c++'
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
String data;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dados do Outro Arduino:");
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == ':') {
      // Recebe um marcador de início de dado
      data = "";
      lcd.clear();
      lcd.setCursor(0, 1);
    } else if (receivedChar == '\n') {
      // Recebe um marcador de fim de linha
      lcd.print(data);
    } else {
      data += receivedChar;
    }
  }
}
