#include <Servo.h>
#include <Adafruit_LiquidCrystal.h>
 
Adafruit_LiquidCrystal lcd_1(0);
 
// --- Pinos ---
const int TRIGGER_PIN = 12;
const int ECHO_PIN = 11;
const int TRIGGER_PIN2 = 8;
const int ECHO_PIN2 = 7;
 
const int SERVO_PIN = 9;
const int SERVO_PIN2 = 3;
 
const int BUZZER_PIN = 4;
const int BUZZER_PIN2 = 2;
 
const int LED_VERMELHO_PIN = 10;
const int LED_VERMELHO_PIN2 = 1;
 
// --- Constantes de controle ---
const int FECHADO = 0;
const int ABERTO = 90;
 
const int DISTANCIA_MAX_VEICULO = 50;
const int DISTANCIA_MIN_VEICULO = 30;
const int DISTANCIA_ALERTA_PROXIMIDADE = 20;
 
int VagasDisponiveis = 10;
const unsigned long TEMPO_ESPERA_ABERTURA = 3000;
 
// --- Objetos Servo ---
Servo catraca1;
Servo catraca2;
 
// --- Controle de tempo ---
unsigned long tempoAbertura1 = 0;
bool catracaAberta1 = false;
unsigned long tempoAbertura2 = 0;
bool catracaAberta2 = false;
 
// --- Funções de distância ---
long calcularDistanciaCm() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
 
  long duracao = pulseIn(ECHO_PIN, HIGH);
  return duracao / 58;
}
 
long calcularDistanciaCm2() {
  digitalWrite(TRIGGER_PIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN2, LOW);
 
  long duracao2 = pulseIn(ECHO_PIN2, HIGH);
  return duracao2 / 58;
}
 
// --- Setup ---
void setup() {
  Serial.begin(9600);
  lcd_1.begin(16, 2);
 
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
 
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUZZER_PIN2, OUTPUT);
  pinMode(LED_VERMELHO_PIN, OUTPUT);
  pinMode(LED_VERMELHO_PIN2, OUTPUT);
 
  catraca1.attach(SERVO_PIN);
  catraca2.attach(SERVO_PIN2);
 
  catraca1.write(FECHADO);
  catraca2.write(FECHADO);
 
  digitalWrite(LED_VERMELHO_PIN, HIGH);
  digitalWrite(LED_VERMELHO_PIN2, HIGH);
 
  lcd_1.print("Sistema iniciado");
  delay(2000);
  lcd_1.clear();
 
  Serial.println("Sistema de Estacionamento Iniciado.");
}
 
// --- Função para controlar a catraca de entrada ---
void controlarCatraca1(long distancia1) {
  Serial.print("Distancia 1: ");
  Serial.println(distancia1);
 
  if (catracaAberta1 && (millis() - tempoAbertura1 >= TEMPO_ESPERA_ABERTURA)) {
    catraca1.write(FECHADO);
    digitalWrite(LED_VERMELHO_PIN, HIGH);
    catracaAberta1 = false;
    Serial.println("Catraca 1 fechada automaticamente.");
  }
 
  if (distancia1 > DISTANCIA_MAX_VEICULO) {
    noTone(BUZZER_PIN);
  } else if (distancia1 > DISTANCIA_MIN_VEICULO) {
    if (!catracaAberta1) {
      catraca1.write(ABERTO);
      digitalWrite(LED_VERMELHO_PIN, LOW);
      tempoAbertura1 = millis();
      catracaAberta1 = true;
      VagasDisponiveis--;
      if (VagasDisponiveis < 0) VagasDisponiveis = 0;
      if (VagasDisponiveis > 10) VagasDisponiveis = 10;
      Serial.println("Catraca 1 aberta (entrada).");
    }
  } else if (distancia1 <= DISTANCIA_MIN_VEICULO && distancia1 > DISTANCIA_ALERTA_PROXIMIDADE) {
    if (!catracaAberta1) {
      catraca1.write(ABERTO);
      digitalWrite(LED_VERMELHO_PIN, LOW);
      tempoAbertura1 = millis();
      catracaAberta1 = true;
      VagasDisponiveis--;
      if (VagasDisponiveis < 0) VagasDisponiveis = 0;
      if (VagasDisponiveis > 10) VagasDisponiveis = 10;
      Serial.println("Catraca 1 aberta por proximidade.");
    }
  } else {
    tone(BUZZER_PIN, 1000, 200);
    catraca1.write(FECHADO);
    digitalWrite(LED_VERMELHO_PIN, HIGH);
    catracaAberta1 = false;
  }
}
 
// --- Função para controlar a catraca de saída ---
void controlarCatraca2(long distancia2) {
  Serial.print("Distancia 2: ");
  Serial.println(distancia2);
 
  if (catracaAberta2 && (millis() - tempoAbertura2 >= TEMPO_ESPERA_ABERTURA)) {
    catraca2.write(FECHADO);
    digitalWrite(LED_VERMELHO_PIN2, HIGH);
    catracaAberta2 = false;
    Serial.println("Catraca 2 fechada automaticamente.");
  }
 
  if (distancia2 > DISTANCIA_MAX_VEICULO) {
    noTone(BUZZER_PIN2);
  } else if (distancia2 > DISTANCIA_MIN_VEICULO) {
    if (!catracaAberta2) {
    catraca2.write(ABERTO);
    digitalWrite(LED_VERMELHO_PIN2, LOW);
    tempoAbertura2 = millis();
    catracaAberta2 = true;
    VagasDisponiveis++;
    if (VagasDisponiveis < 0) VagasDisponiveis = 0;
    if (VagasDisponiveis > 10) VagasDisponiveis = 10;
    Serial.println("Catraca 2 aberta (saida).");
    }
  } else if (distancia2 <= DISTANCIA_MIN_VEICULO && distancia2 > DISTANCIA_ALERTA_PROXIMIDADE) {
    if (!catracaAberta2) {
    catraca2.write(ABERTO);
    digitalWrite(LED_VERMELHO_PIN2, LOW);
    tempoAbertura2 = millis();
    catracaAberta2 = true;
    VagasDisponiveis++;
    if (VagasDisponiveis < 0) VagasDisponiveis = 0;
    if (VagasDisponiveis > 10) VagasDisponiveis = 10;
    Serial.println("Catraca 2 aberta por proximidade.");
    }
  } else {
    tone(BUZZER_PIN2, 1000, 200);
    catraca2.write(FECHADO);
    digitalWrite(LED_VERMELHO_PIN2, HIGH);
    catracaAberta2 = false;
  }
}
 
void loop() {
  long distancia1 = calcularDistanciaCm();
  long distancia2 = calcularDistanciaCm2();
 
  controlarCatraca1(distancia1);
  controlarCatraca2(distancia2);
 
  lcd_1.setCursor(0, 0);
  lcd_1.print("Vagas: ");
  lcd_1.print(VagasDisponiveis);
  lcd_1.print("   ");
 
  delay(200);
}