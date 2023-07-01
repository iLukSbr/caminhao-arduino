#include <Wire.h>
#include <AFMotor.h>
#include <Servo.h>
#include <Ultrasonic.h>

/* Endereços I2C */
#define ENDERECO_SERVO 0
#define ENDERECO_NANO 8
#define ENDERECO_UNO 9

/* PINOS */
#define PIN_OLHOS 9
#define PIN_TRIGGER A0
#define PIN_ECHO A1

/* SENSOR ULTRASSÔNICO */
#define ANGULO_INI_OLHOS 25
#define ANGULO_FIM_OLHOS 80
#define PASSO_ANGULO_OLHOS 5
#define DISTANCIA_COLISAO 20// cm

/* CÓDIGOS I2C */
#define FREIO_ACESO 7
#define FREIO_APAGADO 8

/* MOTORES */
AF_DCMotor dianteiro_esq(2); 
AF_DCMotor dianteiro_dir(3); 
AF_DCMotor traseiro_esq(1); 
AF_DCMotor traseiro_dir(4);

Servo olhos;
Ultrasonic ultrasonic(PIN_TRIGGER, PIN_ECHO);

int pos = ANGULO_INI_OLHOS;
int contador = PASSO_ANGULO_OLHOS;
int distancia;

uint16_t valor_recebido = 0;

bool marcha_frente = false;
bool ponto_morto = false;
bool freio_acendeu = false;
bool estersa_esq = false;
bool estersa_dir = false;

int velocidade = 0;

void setup() {
  Wire.begin(ENDERECO_UNO);
  Wire.onReceive(receiveEvent);
  pontoMorto();
  olhos.attach(PIN_OLHOS);
  olhos.write((ANGULO_INI_OLHOS+ANGULO_FIM_OLHOS)/2);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_TRIGGER, OUTPUT);
}

void pontoMorto(){
  velocidade = 0;
  dianteiro_esq.run(RELEASE); 
  dianteiro_dir.run(RELEASE); 
  traseiro_esq.run(RELEASE); 
  traseiro_dir.run(RELEASE);
  dianteiro_esq.setSpeed(velocidade);
  dianteiro_dir.setSpeed(velocidade);
  traseiro_esq.setSpeed(velocidade);
  traseiro_dir.setSpeed(velocidade);
  ponto_morto = true;
  estersa_esq = false;
  estersa_dir = false;
}

void frente(){
  dianteiro_esq.run(FORWARD);
  dianteiro_dir.run(FORWARD);
  traseiro_esq.run(FORWARD);
  traseiro_dir.run(FORWARD);
  marcha_frente = true;
  ponto_morto = false;
  estersa_esq = false;
  estersa_dir = false;
}

void tras(){
  dianteiro_esq.run(BACKWARD);
  dianteiro_dir.run(BACKWARD);
  traseiro_esq.run(BACKWARD);
  traseiro_dir.run(BACKWARD);
  marcha_frente = false;
  ponto_morto = false;
  estersa_esq = false;
  estersa_dir = false;
}

void direita(){
  if(marcha_frente){
    dianteiro_esq.run(FORWARD);
    traseiro_esq.run(FORWARD);
  }
  else{
    dianteiro_esq.run(BACKWARD);
    traseiro_esq.run(BACKWARD);
  }
  dianteiro_dir.run(RELEASE);
  traseiro_dir.run(RELEASE);
  estersa_dir = true;
}

void esquerda(){
  if(marcha_frente){
    dianteiro_dir.run(FORWARD);
    traseiro_dir.run(FORWARD);
  }
  else{
    dianteiro_dir.run(BACKWARD);
    traseiro_dir.run(BACKWARD);
  }
  dianteiro_esq.run(RELEASE);
  traseiro_esq.run(RELEASE);
  estersa_esq = true;
}

void acelerar(){
  if(!ponto_morto){
    velocidade += 10;
    if(velocidade > 255)
      velocidade = 255;
    setVelocidade();
  }
}

void desacelerar(){
  if(!ponto_morto){
    velocidade -= 10;
    if(velocidade < 0)
      velocidade = 0;
    setVelocidade();
  }
}

void setVelocidade(){
  dianteiro_esq.setSpeed(velocidade);
  dianteiro_dir.setSpeed(velocidade);
  traseiro_esq.setSpeed(velocidade);
  traseiro_dir.setSpeed(velocidade);
}

void giraOlhos(){
  if(estersa_dir)
    olhos.write(ANGULO_INI_OLHOS);
  else if(estersa_esq)
    olhos.write(ANGULO_FIM_OLHOS);
  else{
    pos += contador;
    if(pos > ANGULO_FIM_OLHOS){
      pos = ANGULO_FIM_OLHOS;
      contador *= -1;
    }
    else if(pos < ANGULO_INI_OLHOS){
      pos = ANGULO_INI_OLHOS;
      contador *= -1;
    }
    olhos.write(pos);
  }
}

/* SENSOR ULTRASSÔNICO */
void hcsr04(){
  distancia = ultrasonic.read(CM);
  if(distancia < DISTANCIA_COLISAO)
    pontoMorto();
}

void loop() {
  if(marcha_frente && velocidade){
    giraOlhos();
    hcsr04();
  }
  if(!velocidade && !freio_acendeu){
    Wire.beginTransmission(ENDERECO_NANO);
    Wire.write(FREIO_ACESO);
    Wire.endTransmission();
    freio_acendeu = true;
  }
  else if(velocidade){
    Wire.beginTransmission(ENDERECO_NANO);
    Wire.write(FREIO_APAGADO);
    Wire.endTransmission();
    freio_acendeu = false;
  }
  delay(100);
}

/* RECEPTOR I2C */
void receiveEvent(int howMany){
  valor_recebido = Wire.read();
  switch (valor_recebido){
    case 24:
      if(marcha_frente && !ponto_morto){
        pontoMorto();
      }
      else
        frente();
      break;
    case 82:
      if(!marcha_frente && !ponto_morto){
        pontoMorto();
      }
      else
        tras();
      break;
    case 90:
      direita();
      break;
    case 8:
      esquerda();
      break;
    case 22:
      desacelerar();
      break;
    case 13:
      acelerar();
      break;
    case 4:
      frente();
      velocidade = 255;
      setVelocidade();
      break;
    case 3:
      tras();
      velocidade = 255;
      setVelocidade();
      break;
    case 1:
      direita();
      velocidade = 255;
      setVelocidade();
      break;
    case 2:
      esquerda();
      velocidade = 255;
      setVelocidade();
      break;
    case 5:
      pontoMorto();
      break;
    default:
      break;
  }
  valor_recebido = 0;
}
