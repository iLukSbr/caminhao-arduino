
#include "Servo.h" // Inclui a Biblioteca Servo.h
 
Servo garra, base; // Cria o objeto servo para programação
int angulo = 0; // Ajusta o ângulo inicial do Servo
 
void setup() {
garra.attach(10); // Declara o pino do servo
base.attach(9); // Declara o pino do servo
garra.write(0);
base.write(0);
}
 
void loop() {
 //angulo = 360;
//for (angulo = 0; angulo < 360; angulo += 1) { // Comando que muda a posição do servo de 0 para 180°
garra.write(360); // Comando para angulo específico
delay(2000); 
garra.write(0);
delay(100);
base.write(360);
//delay(15);
//}
 
delay(2000); // Tempo de espera para próximo movimento
 //angulo = 0;
//for (angulo = 360; angulo >= 1; angulo -= 5) { // Comando que muda a posição do servo de 180 para 0°
 // Comando para angulo específico
base.write(0);
//delay(5);
//}
 
delay(2000); // Tempo de espera para próximo movimento
}