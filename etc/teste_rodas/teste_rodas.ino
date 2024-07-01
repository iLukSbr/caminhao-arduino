// Programa: Arduino Motor Shield - Controle motores DC
// Autor: Arduino e Cia
 
// carrega a biblioteca AFMotor
#include <AFMotor.h>
//#include "Servo.h" // Inclui a Biblioteca Servo.h
 
//Servo garra, base;
// Define o motor1 ligado a conexao 1
AF_DCMotor motor1(1); 
AF_DCMotor motor2(2); 
AF_DCMotor motor3(3); 
AF_DCMotor motor4(4); 
 
void setup()
{
  // Define a velocidade maxima para os motores 1 e 2
  motor1.setSpeed(255); 
  motor2.setSpeed(255); 
  motor3.setSpeed(255); 
  motor4.setSpeed(255); 
  //garra.attach(10); // Declara o pino do servo
//base.attach(9); // Declara o pino do servo
}
 
void loop()
{
  // Aciona o motor 1 no sentido horario
  motor1.run(FORWARD); 
  // Aciona o motor 2 no sentido anti-horario
  motor2.run(FORWARD); 
  motor3.run(FORWARD); 
  motor4.run(FORWARD); 
  //garra.write(360);
  // Aguarda 5 segundos
  delay(500);
  // Desliga os 2 motores
  motor1.run(RELEASE); 
  motor2.run(RELEASE); 
  motor3.run(RELEASE); 
  motor4.run(RELEASE);
  //garra.write(0);
  delay(500);
  // Aciona o motor 1 no sentido anti-horario
  motor1.run(BACKWARD);
  // Aciona o motor 2 no sentido horario
  motor2.run(BACKWARD); 
  motor3.run(BACKWARD);
  // Aciona o motor 2 no sentido horario
  motor4.run(BACKWARD); 
  //base.write(360);
  // Aguarda 5 segundos
  delay(500);
  
  // Desliga os 2 motores
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE); 
  motor4.run(RELEASE);
  //base.write(0);
  delay(500);
  // // Aciona o motor 1 no sentido horario
  // motor1.run(FORWARD); 
  // // Aciona o motor 2 no sentido anti-horario
  // motor2.run(BACKWARD); 
  // motor3.run(FORWARD); 
  // motor4.run(FORWARD); 
  //   delay(5000);
  
  // // Desliga os 2 motores
  // motor1.run(RELEASE); 
  // motor2.run(RELEASE);
  // motor3.run(RELEASE); 
  // motor4.run(RELEASE);
  // delay(500);
}