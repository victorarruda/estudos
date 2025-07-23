#include <Servo.h>

// === Pinos ===
const int pinoMicrofone = A0;
const int pinoServo = 9;
const int trigPin = 10;
const int echoPin = 11;
const int pinoLed = 8;            // LED indicador portão aberto
const int pinoLedAmarelo = 2;     // LED amarelo para sinalização
const int pinoLedVermelho = 3;    // LED vermelho para sinalização

// === Configurações ===
const int limiarSom = 15;                    // Sensibilidade à variação sonora
const float distanciaLimite = 25.0;         // Distância máxima (em cm)
const unsigned long tempoBloqueio = 3000;   // Tempo mínimo entre comandos (ms)
const unsigned long tempoLed = 5000;        // LED permanece ligado por 5s após fechar
const unsigned long intervaloSerial = 500;  // Intervalo entre prints no monitor serial
const unsigned long intervaloPisca = 500;   // Intervalo para piscar LEDs (ms)

// === Variáveis ===
Servo servo;
bool portaoAberto = false;
bool ledAposFechamento = false;
bool somJaProcessado = false;

int somAnterior = 0;

unsigned long tempoUltimaAcao = 0;
unsigned long tempoFechamento = 0;
unsigned long ultimoPrint = 0;
unsigned long ultimoPisca = 0;

bool ledAmareloLigado = false;
bool piscaAtivo = false;   // Se LEDs de pisca estão piscando (ou seja, portão em movimento)

// Protótipo para a função de mover servo com pisca LEDs
void moverServoGradualmenteComPisca(Servo &servo, int inicio, int destino, int velocidade);

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pinoLed, OUTPUT);
  pinMode(pinoLedAmarelo, OUTPUT);
  pinMode(pinoLedVermelho, OUTPUT);
  servo.attach(pinoServo);
  servo.write(0); // Portão fechado
  digitalWrite(pinoLed, LOW);
  digitalWrite(pinoLedAmarelo, LOW);
  digitalWrite(pinoLedVermelho, LOW);
}

void loop() {
  unsigned long agora = millis();

  // === Som ===
  int somAtual = analogRead(pinoMicrofone);
  int variacaoSom = abs(somAtual - somAnterior);
  somAnterior = somAtual;
  bool somDetectado = variacaoSom > limiarSom;

  // === Distância ===
  float distancia = medirDistancia();
  bool presenca = (distancia > 0 && distancia <= distanciaLimite);

  // === Debug controlado ===
  if (agora - ultimoPrint >= intervaloSerial) {
    Serial.print("Som: ");
    Serial.print(somAtual);
    Serial.print(" | Variação: ");
    Serial.print(variacaoSom);
    Serial.print(" | Distância: ");
    Serial.print(distancia);
    Serial.print(" cm | Presença: ");
    Serial.print(presenca ? "Sim" : "Não");
    Serial.print(" | Som: ");
    Serial.println(somDetectado ? "Sim" : "Não");
    ultimoPrint = agora;
  }

  // Pisca LEDs enquanto portão estiver em movimento ou aberto
  if (piscaAtivo && (agora - ultimoPisca >= intervaloPisca)) {
    ultimoPisca = agora;
    if (ledAmareloLigado) {
      digitalWrite(pinoLedAmarelo, LOW);
      digitalWrite(pinoLedVermelho, HIGH);
      ledAmareloLigado = false;
    } else {
      digitalWrite(pinoLedAmarelo, HIGH);
      digitalWrite(pinoLedVermelho, LOW);
      ledAmareloLigado = true;
    }
  }

  // === Abrir portão ===
  if (!portaoAberto && somDetectado && presenca && !somJaProcessado && (agora - tempoUltimaAcao > tempoBloqueio)) {
    digitalWrite(pinoLed, HIGH);
    piscaAtivo = true;
    moverServoGradualmenteComPisca(servo, 0, 120, 30);
    portaoAberto = true;
    tempoUltimaAcao = millis();
    somJaProcessado = true;
    Serial.println(">> Bem vindo! Abrindo portão...");
  }

  // === Fechar portão ===
  else if (portaoAberto && somDetectado && !somJaProcessado && (agora - tempoUltimaAcao > tempoBloqueio)) {
    moverServoGradualmenteComPisca(servo, 120, 0, 15);
    portaoAberto = false;
    tempoUltimaAcao = agora;
    tempoFechamento = agora;
    ledAposFechamento = true;
    somJaProcessado = true;
    piscaAtivo = false;
    digitalWrite(pinoLedAmarelo, LOW);
    digitalWrite(pinoLedVermelho, LOW);
    Serial.println("<< Portão FECHADO.");
  }

  // === Desliga LED principal 5s após fechar ===
  if (!portaoAberto && ledAposFechamento && (agora - tempoFechamento > tempoLed)) {
    digitalWrite(pinoLed, LOW);
    ledAposFechamento = false;
    Serial.println("-- LED apagado após 5s");
  }

  if (!somDetectado) {
    somJaProcessado = false;
  }
}

// === Função de medição de distância ===
float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH, 30000); // Timeout de 30ms
  if (duracao == 0) return -1; // Sem leitura

  float distancia = duracao * 0.034 / 2;
  return distancia;
}

// === Função que move servo gradualmente e mantém LEDs piscando ===
void moverServoGradualmenteComPisca(Servo &servo, int inicio, int destino, int velocidade) {
  if (inicio < destino) {
    for (int pos = inicio; pos <= destino; pos++) {
      servo.write(pos);

      // Pisca LEDs manual dentro do loop da movimentação
      unsigned long agora = millis();
      static unsigned long ultimoPiscaLocal = 0;
      static bool ledAmareloLigadoLocal = false;

      if (agora - ultimoPiscaLocal >= 500) {
        ultimoPiscaLocal = agora;
        if (ledAmareloLigadoLocal) {
          digitalWrite(pinoLedAmarelo, LOW);
          digitalWrite(pinoLedVermelho, HIGH);
          ledAmareloLigadoLocal = false;
        } else {
          digitalWrite(pinoLedAmarelo, HIGH);
          digitalWrite(pinoLedVermelho, LOW);
          ledAmareloLigadoLocal = true;
        }
      }

      delay(velocidade);
    }
  } else {
    for (int pos = inicio; pos >= destino; pos--) {
      servo.write(pos);

      // Pisca LEDs manual dentro do loop da movimentação
      unsigned long agora = millis();
      static unsigned long ultimoPiscaLocal = 0;
      static bool ledAmareloLigadoLocal = false;

      if (agora - ultimoPiscaLocal >= 500) {
        ultimoPiscaLocal = agora;
        if (ledAmareloLigadoLocal) {
          digitalWrite(pinoLedAmarelo, LOW);
          digitalWrite(pinoLedVermelho, HIGH);
          ledAmareloLigadoLocal = false;
        } else {
          digitalWrite(pinoLedAmarelo, HIGH);
          digitalWrite(pinoLedVermelho, LOW);
          ledAmareloLigadoLocal = true;
        }
      }

      delay(velocidade);
    }
  }
}
