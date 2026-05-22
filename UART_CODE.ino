#include <PRIZM_PRO.h>
PRIZM prizm;

const int RX_PIN = 18;
const int TX_PIN = 17;

unsigned long lastSendTime = 0;
const int SEND_INTERVAL = 100;

// Buffer para recepción no bloqueante
String bufferRX = "";

void setup() {
  prizm.PrizmBegin();
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Listo");
}

void loop() {

  // --- ENVIAR cada 100ms sin bloquear ---
  if (millis() - lastSendTime >= SEND_INTERVAL) {
    lastSendTime = millis();

    float dist1 = 10.0 + (millis() % 50);
    float dist2 = 20.0 + (millis() % 30);
    float dist3 = 30.0 + (millis() % 20);

    Serial1.print('A');
    Serial1.print(dist1, 2);
    Serial1.print('B');
    Serial1.print(dist2, 2);
    Serial1.print('C');
    Serial1.print(dist3, 2);
    Serial1.print("D\r\n");

    //Serial.print("TX → I");
    //Serial.print(dist1, 2);
    //Serial.print("N");
    //Serial.print(dist2, 2);
    //Serial.print("X");
    //Serial.print(dist3, 2);
    //Serial.println("F");
  }

  // --- RECIBIR sin bloquear ---
  while (Serial1.available() > 0) {
      char c = Serial1.read();
      Serial.print(c);  // imprimir cada caracter directo sin procesar
      
      if (c == '\n') {
        // Trama completa: A3B2C5
        bufferRX.trim();
        if (bufferRX.length() > 0) {

          // Extraer valores entre separadores A, B, C
          int idxA = bufferRX.indexOf('A');
          int idxB = bufferRX.indexOf('B');
          int idxC = bufferRX.indexOf('C');

          if (idxA != -1 && idxB != -1 && idxC != -1) {
            int valorA = bufferRX.substring(idxA + 1, idxB).toInt();
            int valorB = bufferRX.substring(idxB + 1, idxC).toInt();
            int valorC = bufferRX.substring(idxC + 1).toInt();

            Serial.print("A="); Serial.println(valorA);
            Serial.print("B="); Serial.println(valorB);
            Serial.print("C="); Serial.println(valorC);
          }
        }
        bufferRX = "";
      } else {
        bufferRX += c;
      }
    }
delay(10);
  // Sin delay bloqueante
}
