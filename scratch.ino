const int car_distance = 40;

// Entradas do Ultrassônico
const int TRIG = 3;
const int ECHO = 2;

// Pinos dos LEDs
const int ledGreen = 6;
const int ledRed = 8;

// Fotoresistor conectado ao pino A2
const int LDR_PIN = A2;

void setup() {
  Serial.begin(9600);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);

  pinMode(LDR_PIN, INPUT);
}

void loop() {
  int distance = sensor(TRIG, ECHO);
  int ldrValue = analogRead(LDR_PIN);

  float prox = proximity(distance);
  float mid = mediumDistance(distance);
  float far = farDistance(distance);

  float dark = lowLight(ldrValue);
  float medium = mediumLight(ldrValue);
  float bright = highLight(ldrValue);

  float danger = min(prox, bright);
  float warning = max(min(mid, bright), min(prox, medium));
  float safe = max(min(far, bright), max(min(mid, dark), min(far, medium)));

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.print(" cm | Luminosidade: ");
  Serial.println(ldrValue);

  if (danger > warning && danger > safe) {
    Serial.println("Alerta: LED VERMELHO");
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, HIGH);
  } 
  if (warning > danger && warning > safe) {
    Serial.println("Atenção: LED AMARELO");
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
  } 
  else {
    Serial.println("Seguro: LED VERDE");
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
  }

  delay(500);
}

int sensor(int pinotrig, int pinoecho) {
  digitalWrite(pinotrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinotrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinotrig, LOW);

  return pulseIn(pinoecho, HIGH) / 58;
}

float proximity(int distance) {
  if (distance <= 20) return 1;
  if (distance > 20 && distance <= 40) return (40 - distance) / 20.0;
  return 0;
}

float mediumDistance(int distance) {
  if (distance > 20 && distance <= 60) return (distance - 20) / 40.0;
  if (distance > 60 && distance <= 80) return (80 - distance) / 20.0;
  return 0;
}

float farDistance(int distance) {
  if (distance > 60) return 1;
  if (distance > 40 && distance <= 60) return (distance - 40) / 20.0;
  return 0;
}

float lowLight(int ldrValue) {
  if (ldrValue <= 400) return 1;
  if (ldrValue > 400 && ldrValue <= 650) return (650 - ldrValue) / 250.0;
  return 0;
}

float mediumLight(int ldrValue) {
  if (ldrValue > 400 && ldrValue <= 700) return (ldrValue - 400) / 300.0;
  if (ldrValue > 700 && ldrValue <= 900) return (900 - ldrValue) / 200.0;
  return 0;
}

float highLight(int ldrValue) {
  if (ldrValue > 700) return 1;
  if (ldrValue > 650 && ldrValue <= 700) return (ldrValue - 650) / 50.0;
  return 0;
}
