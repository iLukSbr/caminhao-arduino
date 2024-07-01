#include <VirtualWire.h>
 
uint8_t message[VW_MAX_MESSAGE_LEN];
uint8_t len = VW_MAX_MESSAGE_LEN;

void setup() {
  Serial.begin(9600);
  Serial.println("Dispositivo pronto para receber dados!");
  vw_set_rx_pin(11);
  vw_setup(2000);
  vw_rx_start();
}
 
void loop() {
  if (vw_get_message(message, &len)){
    Serial.println(message[0]);
  }
}