import processing.serial.*;

Serial myPort;

String angle = "";
String distance = "";
String data = "";
int iAngle = 0;
int iDistance = -1;

PFont orcFont;

void setup() {
  size(1366, 700);
  smooth();

  // Ajuste a porta e velocidade para combinar com o Arduino
  myPort = new Serial(this, "COM7", 9600);
  myPort.bufferUntil('\n'); // Espera uma linha inteira antes de processar
}

void draw() {
  fill(0, 20); // efeito de "rastro"
  noStroke();
  rect(0, 0, width, height);

  drawRadar();
  drawLine();
  drawObject();
  drawText();
}

// Recebe dados do Arduino
void serialEvent(Serial myPort) {
  data = myPort.readStringUntil('\n');
  if (data != null) {
    data = trim(data); // remove espaços e quebras
    int commaIndex = data.indexOf(',');
    if (commaIndex > 0) {
      angle = data.substring(0, commaIndex);
      distance = data.substring(commaIndex+1);
      iAngle = int(angle);
      iDistance = int(distance);
    }
  }
}

// Desenha a base do radar
void drawRadar() {
  pushMatrix();
  translate(683, 700);
  noFill();
  strokeWeight(2);
  stroke(98, 245, 31);

  arc(0, 0, 1300, 1300, PI, TWO_PI);
  arc(0, 0, 1000, 1000, PI, TWO_PI);
  arc(0, 0, 700, 700, PI, TWO_PI);
  arc(0, 0, 400, 400, PI, TWO_PI);

  for (int a = 0; a <= 180; a += 30) {
    line(0, 0, 700*cos(radians(a)), -700*sin(radians(a)));
  }
  popMatrix();
}

// Linha do scanner (usa ângulo do Arduino)
void drawLine() {
  pushMatrix();
  strokeWeight(3);
  stroke(30, 250, 60);
  translate(683, 700);
  line(0, 0, 700*cos(radians(iAngle)), -700*sin(radians(iAngle)));
  popMatrix();
}

// Obstáculo detectado
void drawObject() {
  if (iDistance <= 0) return; // nada detectado

  pushMatrix();
  translate(683, 700);
  strokeWeight(9);
  stroke(255, 10, 10);

  float pixsDistance = iDistance * 17; // ajuste de escala
  if (pixsDistance > 700) pixsDistance = 700; // não sai da tela

  line(pixsDistance*cos(radians(iAngle)), -pixsDistance*sin(radians(iAngle)),
       700*cos(radians(iAngle)), -700*sin(radians(iAngle)));
  popMatrix();
}

// Textos
void drawText() {
  fill(0);
  noStroke();
  rect(0, 660, width, 40);

  fill(98, 245, 31);
  textSize(20);
  text("Ângulo: " + iAngle + "°", 20, 680);
  if (iDistance > 0) {
    text("Distância: " + iDistance + " cm", 200, 680);
  } else {
    text("Sem obstáculo", 200, 680);
  }
}
