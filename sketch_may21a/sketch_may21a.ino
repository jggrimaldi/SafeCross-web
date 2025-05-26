#define TRIG 9
#define ECHO 10
#define LED_VERDE 2
#define LED_VERMELHO 3
#define LED_AMARELO 4

unsigned long tempoInicio = 0;
bool pessoaDetectando = false;
bool pessoaConfirmada = false;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  Serial.begin(9600);

  // Estado inicial: verde ligado
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
}

void loop() {
  long duracao, distancia;

  // Leitura da distância
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duracao = pulseIn(ECHO, HIGH);
  distancia = duracao * 0.034 / 2;

  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia > 0 && distancia < 50) {
    if (!pessoaDetectando) {
      pessoaDetectando = true;
      tempoInicio = millis();
    } else {
      // Pessoa ficou mais de 5 segundos na frente do sensor
      if (millis() - tempoInicio >= 5000 && !pessoaConfirmada) {
        pessoaConfirmada = true;

        // Transição de verde -> amarelo -> vermelho
        digitalWrite(LED_VERDE, LOW);
        digitalWrite(LED_AMARELO, HIGH);
        Serial.println("Atenção! Sinal prestes a fechar.");
        delay(2000); // amarelo por 2 segundos

        digitalWrite(LED_AMARELO, LOW);
        digitalWrite(LED_VERMELHO, HIGH);
        Serial.println("Sinal fechado para carros.");
        delay(12000); // vermelho por 12 segundos

        // Volta ao normal: verde
        digitalWrite(LED_VERMELHO, LOW);
        digitalWrite(LED_VERDE, HIGH);
        pessoaDetectando = false;
        pessoaConfirmada = false;
      }
    }
  } else {
    pessoaDetectando = false;
    pessoaConfirmada = false;
  }

  delay(200);
}