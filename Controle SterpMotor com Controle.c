#include <IRremote.hpp>  // Biblioteca IR Remote versão nova

// === CONFIGURAÇÃO DO RECEPTOR IR ===
const int RECV_PIN = 11;  

// === CONFIGURAÇÃO DO MOTOR DE PASSO ===
int pins[] = {8, 9, 10, 12};  
const int STEPS = 30;        
const int DELAY_STEP = 5;     

// === FUNÇÕES DO MOTOR ===
void stepClockwise(int steps) {
  for (int s = 0; s < steps; s++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(pins[i], HIGH);
      delay(DELAY_STEP);
      digitalWrite(pins[i], LOW);
    }
  }
}

void stepCounterClockwise(int steps) {
  for (int s = 0; s < steps; s++) {
    for (int i = 3; i >= 0; i--) {
      digitalWrite(pins[i], HIGH);
      delay(DELAY_STEP);
      digitalWrite(pins[i], LOW);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // Inicializa pinos do motor
  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }

  // Inicializa receptor IR
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("✅ IR Receiver inicializado!");
  Serial.println("Aponte o controle remoto e pressione um botão...");
}

void loop() {
  if (IrReceiver.decode()) {
    uint32_t codigo = IrReceiver.decodedIRData.decodedRawData;

    Serial.print("Código recebido: 0x");
    Serial.println(codigo, HEX);

    if (codigo == 0xBF40FF00) {
      Serial.println("Seta para CIMA detectada - Motor para frente");s
      stepClockwise(STEPS);
    }
    else if (codigo == 0xE619FF00) {
      Serial.println("▼ Seta para BAIXO detectada - Motor para trás");
      stepCounterClockwise(STEPS);
    }

    IrReceiver.resume();  // pronto para próximo comando
  }
}
