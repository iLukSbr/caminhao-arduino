#include <VirtualWire.h>

int vert = 0;
int hori = 0;
int hozdead = 380;
int verdead = 360;

uint8_t pos[1] = {0};

void setup() {
  vw_set_tx_pin(3);
  vw_setup(2000);
  pinMode(2, INPUT);
  pinMode(4, OUTPUT);
  for(int i=9; i<13; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
}
 
void loop() {
  digitalWrite(4, LOW);
  if(digitalRead(2)){
    pos[0] = 9;
    vw_send((uint8_t*)pos, sizeof(pos));
    vw_wait_tx();
    digitalWrite(4, HIGH);
  }
  hori = analogRead(A0);
  vert = analogRead(A1);
  if (vert > verdead+300){
    pos[0]=2;
  }//dir 2
  else if (vert < verdead-300){
    pos[0]=1;
  }//esq 1
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
  hori = analogRead(A2);
  vert = analogRead(A3);
  if (vert > verdead+300){
    pos[0]=5;
  }//dir 5
  else if (vert < verdead-300){
    pos[0]=6;
  }//esq 6
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