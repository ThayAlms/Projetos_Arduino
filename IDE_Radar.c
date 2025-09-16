#include <Servo.h>

Servo myServo;

const int trigPin = 7;
const int echoPin = 6;
const int servoPin = 9;

// Limites de varredura
const int MIN_ANGLE = 40;   // ângulo mínimo
const int MAX_ANGLE = 172;  // ângulo máximo

const unsigned long measureDelay = 20; // tempo para servo estabilizar antes da medição
const int sweepStep = 2;              // passo em graus

const unsigned long timeoutMicros = 30000UL; // timeout do pulseIn (30ms)

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  delay(200);
}

long measureDistanceCM() {
  // Pulso de trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Captura eco
  unsigned long duration = pulseIn(echoPin, HIGH, timeoutMicros);
  if (duration == 0) return -1; // sem eco

  return duration / 58; // converte para cm
}

void loop() {
  // Sweep de ida
  for (int angle = MIN_ANGLE; angle <= MAX_ANGLE; angle += sweepStep) {
    myServo.write(angle);
    delay(measureDelay);

    long d1 = measureDistanceCM();
    delay(20);
    long d2 = measureDistanceCM();

    long dist;
    if (d1 < 0 && d2 < 0) dist = -1;
    else if (d1 < 0) dist = d2;
    else if (d2 < 0) dist = d1;
    else dist = (d1 + d2) / 2;

    Serial.print(angle);
    Serial.print(",");
    Serial.println(dist);
    delay(5);
  }

  // Sweep de volta
  for (int angle = MAX_ANGLE; angle >= MIN_ANGLE; angle -= sweepStep) {
    myServo.write(angle);
    delay(measureDelay);

    long d1 = measureDistanceCM();
    delay(20);
    long d2 = measureDistanceCM();

    long dist;
    if (d1 < 0 && d2 < 0) dist = -1;
    else if (d1 < 0) dist = d2;
    else if (d2 < 0) dist = d1;
    else dist = (d1 + d2) / 2;

    Serial.print(angle);
    Serial.print(",");
    Serial.println(dist);
    delay(5);
  }
}
