// TEA5767 Example

#include <Wire.h>
#include <TEA5767Radio.h>
#include <LiquidCrystal_I2C.h>

TEA5767Radio radio = TEA5767Radio();

LiquidCrystal_I2C lcd(0x27, 16,2);

#define CLK 2
#define DT 3
#define SW 4

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

float frequencia = 76;
float frequencia_ant = 0;

void setup()
{ 
  Serial.begin(9600);
  Wire.begin();
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print("Carrinho ChatGPT");
  lcd.setCursor(0, 1);
  lcd.print("Freq: ");
  lcd.setCursor(6, 1);
  lcd.print("      MHz");
  radio.setFrequency(frequencia); // pick your own frequency
  lcd.setBacklight(HIGH);
	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);
	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);
}

void loop()
{
  Serial.println(frequencia);
  if(frequencia_ant != frequencia){
    lcd.setCursor(6, 1);
    lcd.print(frequencia, 1);
    frequencia_ant = frequencia;
  }
	// Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			counter --;
			currentDir ="CCW";
      if(frequencia > 76)
        frequencia -= 0.1;
		} else {
			// Encoder is rotating CW so increment
			counter ++;
			currentDir ="CW";
      if(frequencia < 108)
        frequencia += 0.1;
		}

		Serial.print("Direction: ");
		Serial.print(currentDir);
		Serial.print(" | Counter: ");
		Serial.println(counter);
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;

	// Read the button state
	int btnState = digitalRead(SW);

	//If we detect LOW signal, button is pressed
	if (btnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastButtonPress > 50) {
			Serial.println("Button pressed!");
		}

		// Remember last button press event
		lastButtonPress = millis();
	}

	// Put in a slight delay to help debounce the reading
   radio.setFrequency(frequencia);

   delay(1);
}
