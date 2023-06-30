#include <IRremote.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <VirtualWire.h>

/* ENDEREÇOS I2C */
#define ENDERECO_SERVO 0
#define ENDERECO_NANO 8
#define ENDERECO_UNO 9

/* PINOS */
#define IR_RECEIVE_PIN 9
#define PIN_BUZINA 6
#define PIN_SETA_DIREITA 4
#define PIN_SETA_ESQUERDA 5
#define PIN_FAROIS 7
#define PIN_LUZ_FREIO 8
#define PIN_RECEIVER 16

/* PWM DOS SERVOS */
#define GARRA_ABERTA 230
#define GARRA_FECHADA 460
#define BASE_ESQUERDA 490
#define BASE_DIREITA 110
#define LEVANTADO 110
#define ABAIXADO 490
#define PRA_FRENTE 490
#define PRA_TRAS 110
#define INCREMENTO_PWM 20
#define SERVO_FREQ 50// Hz
#define OSCIL_FREQ 27000000

/* PORTA DOS SERVOS DO BRAÇO ROBÓTICO */
#define SERVO_FRENTE_TRAS 2
#define SERVO_CIMA_BAIXO 3
#define SERVO_GARRA 0
#define SERVO_BASE 1

/* CÓDIGO I2C */
#define FREIO_ACESO 7
#define FREIO_APAGADO 8

uint8_t message[VW_MAX_MESSAGE_LEN];
uint8_t len = VW_MAX_MESSAGE_LEN;

uint16_t valor_recebido;
uint16_t valor_i2c;
uint16_t pulso_servo_base = (BASE_ESQUERDA+BASE_DIREITA)/2;
uint16_t pulso_servo_cima_baixo = LEVANTADO;
uint16_t pulso_servo_frente_tras = PRA_TRAS;
uint16_t pulso_servo_garra = GARRA_FECHADA;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Wire.begin(ENDERECO_NANO);
  Wire.onReceive(receiveEvent);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  pinMode(PIN_BUZINA, OUTPUT);
  pinMode(PIN_SETA_DIREITA, OUTPUT);
  pinMode(PIN_SETA_ESQUERDA, OUTPUT);
  pinMode(PIN_FAROIS, OUTPUT);
  pinMode(PIN_LUZ_FREIO, OUTPUT);
  digitalWrite(PIN_BUZINA, HIGH);
  digitalWrite(PIN_SETA_DIREITA, HIGH);
  digitalWrite(PIN_SETA_ESQUERDA, HIGH);
  digitalWrite(PIN_FAROIS, HIGH);
  digitalWrite(PIN_LUZ_FREIO, HIGH);
  pwm.begin();
  pwm.setOscillatorFrequency(OSCIL_FREQ);
  pwm.setPWMFreq(SERVO_FREQ);
  pwm.setPWM(SERVO_BASE, 0, pulso_servo_base);
  pwm.setPWM(SERVO_CIMA_BAIXO, 0, pulso_servo_cima_baixo);
  pwm.setPWM(SERVO_FRENTE_TRAS, 0, pulso_servo_frente_tras);
  pwm.setPWM(SERVO_GARRA, 0, pulso_servo_garra);
  vw_set_rx_pin(PIN_RECEIVER);
  vw_setup(2000);
  vw_rx_start();
}

void receiveEvent(int howMany){
  valor_i2c = Wire.read();
  if(valor_i2c == FREIO_ACESO)
    digitalWrite(PIN_LUZ_FREIO, HIGH);
  else if(valor_i2c == FREIO_APAGADO)
    digitalWrite(PIN_LUZ_FREIO, LOW);
}

void servoFrente(){
  pulso_servo_frente_tras += INCREMENTO_PWM;
  if(pulso_servo_frente_tras > PRA_FRENTE)
    pulso_servo_frente_tras = PRA_FRENTE;
  pwm.setPWM(SERVO_FRENTE_TRAS, 0, pulso_servo_frente_tras);
}

void servoTras(){
  pulso_servo_frente_tras -= INCREMENTO_PWM;
  if(pulso_servo_frente_tras < PRA_TRAS)
    pulso_servo_frente_tras = PRA_TRAS;
  pwm.setPWM(SERVO_FRENTE_TRAS, 0, pulso_servo_frente_tras);
}

void servoCima(){
  pulso_servo_cima_baixo -= INCREMENTO_PWM;
  if(pulso_servo_cima_baixo < LEVANTADO)
    pulso_servo_cima_baixo = LEVANTADO;
  pwm.setPWM(SERVO_CIMA_BAIXO, 0, pulso_servo_cima_baixo);
}

void servoBaixo(){
  pulso_servo_cima_baixo += INCREMENTO_PWM;
  if(pulso_servo_cima_baixo > ABAIXADO)
    pulso_servo_cima_baixo = ABAIXADO;
  pwm.setPWM(SERVO_CIMA_BAIXO, 0, pulso_servo_cima_baixo);
}

void servoGarra(){
  if(pulso_servo_garra == GARRA_ABERTA)
    pulso_servo_garra = GARRA_FECHADA;
  else
    pulso_servo_garra = GARRA_ABERTA;
  pwm.setPWM(SERVO_GARRA, 0, pulso_servo_garra);
}

void servoDireita(){
  pulso_servo_base -= INCREMENTO_PWM;
  if(pulso_servo_base < BASE_DIREITA)
    pulso_servo_base = BASE_DIREITA;
  pwm.setPWM(SERVO_BASE, 0, pulso_servo_base);
}

void servoEsquerda(){
  pulso_servo_base += INCREMENTO_PWM;
  if(pulso_servo_base > BASE_ESQUERDA)
    pulso_servo_base = BASE_ESQUERDA;
  pwm.setPWM(SERVO_BASE, 0, pulso_servo_base);
}

void loop() {
  if (IrReceiver.decode()) {
    valor_recebido = IrReceiver.decodedIRData.command;
    switch (valor_recebido){
      case 28:
        if(digitalRead(PIN_BUZINA))
          digitalWrite(PIN_BUZINA, LOW);
        else
          digitalWrite(PIN_BUZINA, HIGH);
        break;
      case 25:
        if(digitalRead(PIN_FAROIS))
          digitalWrite(PIN_FAROIS, LOW);
        else
          digitalWrite(PIN_FAROIS, HIGH);
        break;
      case 7:
        if(digitalRead(PIN_SETA_DIREITA))
          digitalWrite(PIN_SETA_DIREITA, LOW);
        else
          digitalWrite(PIN_SETA_DIREITA, HIGH);
        break;
      case 9:
        if(digitalRead(PIN_SETA_ESQUERDA))
          digitalWrite(PIN_SETA_ESQUERDA, LOW);
        else
          digitalWrite(PIN_SETA_ESQUERDA, HIGH);
        break;
      case 21:
        servoTras();
        break;
      case 70:
        servoFrente();
        break;
      case 68:
        servoEsquerda();
        break;
      case 67:
        servoDireita();
        break;
      case 69:
        servoBaixo();
        break;
      case 71:
        servoCima();
        break;
      case 64:
        servoGarra();
        break;
      default:
        if(valor_recebido == 22 || valor_recebido == 13 || valor_recebido == 24 || valor_recebido == 82 || valor_recebido == 8 || valor_recebido == 90){
          Wire.beginTransmission(ENDERECO_UNO);
          Wire.write(valor_recebido);
          Wire.endTransmission();
        }
        break;
    }
    IrReceiver.resume();
  }
  if (vw_get_message(message, &len)){
    if(message[0] == 5)
      servoDireita();
    else if(message[0] == 6)
      servoEsquerda();
    else if(message[0] == 7)
      servoFrente();
    else if(message[0] == 8)
      servoTras();
    else if(message[0] == 9){
      servoGarra();
      Wire.beginTransmission(ENDERECO_UNO);
      Wire.write(5);
      Wire.endTransmission();
    }
    else if(message[0] == 1 || message[0] == 2 || message[0] == 3 || message[0] == 4){
      Wire.beginTransmission(ENDERECO_UNO);
      Wire.write(message[0]);
      Wire.endTransmission();
    }
  }
  delay(100);
}