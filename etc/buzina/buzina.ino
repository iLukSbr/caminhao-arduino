// Change this to the output pin you are using
#define TICK_PIN 5

void setup()
{
  pinMode(TICK_PIN,OUTPUT);

}
  
void loop()
{
    for(int i = 0;i < 5;i++) {
    tone(TICK_PIN,1000,1);
    delay(350);
    tone(TICK_PIN,500,1);
    delay(350);
  }
}