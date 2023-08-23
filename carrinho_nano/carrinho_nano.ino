/*
Para o Arduino Nano no joystick RF.
Códigos de um caminhão guincho de Arduino com braço robótico,
controles infravermelho e joystick RF, buzina, setas direcionais,
faróis, luzes de freio e sensor de obstáculos.

MIT License

Copyright (c) 2023 Lucas Yukio Fukuda Matsumoto

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <VirtualWire.h>

int vert = 0;
int hori = 0;
int hozdead = 380;
int verdead = 360;

uint8_t pos[1] = {0};

void setup() {
  vw_set_tx_pin(3);//pino do transmissor RF
  vw_setup(2000);
  pinMode(2, INPUT);//leitura do módulo push button
  pinMode(4, OUTPUT);//led se está transmitindo ou não
  for(int i=9; i<13; i++){//fornecer energia aos módulos, 5V em 9 a 12
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
}
 
void loop() {
  digitalWrite(4, LOW);

  /* Push button */
  if(digitalRead(2)){
    pos[0] = 9;
    vw_send((uint8_t*)pos, sizeof(pos));
    vw_wait_tx();
    digitalWrite(4, HIGH);
  }

  /* Joystick do caminhão */
  hori = analogRead(A0);
  vert = analogRead(A1);
  if (vert > verdead+300){
    pos[0]=2;
  }//direita 2
  else if (vert < verdead-300){
    pos[0]=1;
  }//esquerda 1
  else if (hori > hozdead+300){
    pos[0]=4;
  }//cima 4
  else if (hori < hozdead-300){
    pos[0]=3;
  }//baixo 3
  else {
    pos[0] = 0;
  }
  if(pos[0]){
    vw_send((uint8_t*)pos, sizeof(pos));
    vw_wait_tx();
    digitalWrite(4, HIGH);
  }

  /* Joystick do braço robótico */
  hori = analogRead(A2);
  vert = analogRead(A3);
  if (vert > verdead+300){
    pos[0]=5;
  }//direita 5
  else if (vert < verdead-300){
    pos[0]=6;
  }//esquerda 6
  else if (hori > hozdead+300){
    pos[0]=7;
  }//cima 7
  else if (hori < hozdead-300){
    pos[0]=8;
  }//baixo 8
  else {
    pos[0] = 0;
  }
  if(pos[0]){
    vw_send((uint8_t*)pos, sizeof(pos));
    vw_wait_tx();
    digitalWrite(4, HIGH);
  }
  delay(100);
}
