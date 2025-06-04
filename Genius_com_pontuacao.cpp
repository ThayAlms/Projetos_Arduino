// Pinos dos LEDs
const int ledPins[3] = {2, 3, 4};

// Pinos dos botões
const int buttonPins[3] = {5, 6, 7};

// Pinos ligados as entradas A, B, C, D do CI 4511
const int bcdPins[4] = {8, 9, 10, 11}; // A = 8, B = 9, C = 10, D = 11

// Tamanho maximo da sequencia
const int maxSequence = 9; 

// Sequencia de LEDs gerada
int sequence[maxSequence];
int sequenceLength = 1; // comeca com 1 LED
bool playerTurn = false;

void setup() {
  Serial.begin(9600);

  // Configura LEDs como saída
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Configura botoes como entrada
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT);
  }

  // Configura os pinos BCD como saída
  for (int i = 0; i < 4; i++) {
    pinMode(bcdPins[i], OUTPUT);
  }

  randomSeed(analogRead(A0)); // inicializa randomização

  generateSequence();
  displayLevel(sequenceLength);
  delay(1000);
}

void loop() {
  showSequence();

  playerTurn = true;
  if (!getPlayerInput()) {
    // Se errar
    gameOver();
  } else {
    // Se acertar
    sequenceLength++;
    if (sequenceLength > maxSequence) {
      winGame();
    }
    displayLevel(sequenceLength); // atualiza display
    delay(100);
  }
}

void generateSequence() {
  for (int i = 0; i < maxSequence; i++) {
    sequence[i] = random(0, 3); // número entre 0 e 2
  }
}

void showSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    int led = sequence[i];
    digitalWrite(ledPins[led], HIGH);
    delay(500);
    digitalWrite(ledPins[led], LOW);
    delay(250);
  }
}

bool getPlayerInput() {
  Serial.println("getPlayerInput");
  for (int i = 0; i < sequenceLength; i++) {
    int input = waitForButton();
    Serial.println("botão pressionado");
    if (input != sequence[i]) {
      return false;
    }
  }
  return true;
}

int waitForButton() {
  while (true) {
    // Serial.println("waitForButton");
    for (int i = 0; i < 3; i++) {
      //  Serial.print(i);
      //  Serial.print(" :");
      // Serial.println(digitalRead(buttonPins[i]));
      if (digitalRead(buttonPins[i]) == HIGH) {
        Serial.println("A");
        flashLED(i);
        while (digitalRead(buttonPins[i]) == HIGH); // espera soltar
        delay(100); // debounce
        Serial.print("SAIUUUU");
        return i;
      }
    }
  }
}

void flashLED(int index) {
  digitalWrite(ledPins[index], HIGH);
  delay(300);
  digitalWrite(ledPins[index], LOW);
}

void gameOver() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[2], HIGH);
    delay(2000);
    digitalWrite(ledPins[0], LOW);
    digitalWrite(ledPins[1], LOW);
    digitalWrite(ledPins[2], LOW);
    delay(300);
  }
  sequenceLength = 1;
  generateSequence();
  displayLevel(sequenceLength); // mostra nível 1
  delay(1000);
}

void winGame() {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 3; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    delay(200);
    for (int j = 0; j < 3; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    delay(200);
  }
  sequenceLength = 1;
  generateSequence();
  displayLevel(sequenceLength); // reinicia no nível 1
  delay(1000);
}

void displayLevel(int level) {
  // Garante que o nível fique entre 1 e 9
  int value = constrain(level, 1, 9);
  for (int i = 0; i < 4; i++) {
    digitalWrite(bcdPins[i], (value >> i) & 1);
  }
}
