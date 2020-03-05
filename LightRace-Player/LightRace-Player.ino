#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 10); // CE, CSN         
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.

int buttonPin = 7;
boolean buttonPress = 1;
boolean buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int counter = 0; //for debugging

void sendPress(){
  //const char text[] = "Your Button State is LOW";
  radio.write(&buttonPress, sizeof(buttonPress));  
  Serial.print("(");
  Serial.print(counter); 
  Serial.println(") Button pressed!");
  counter++;
}

void checkButton(){
  //button_state = digitalRead(button_pin);
  boolean reading = digitalRead(buttonPin);
  
  //if(button_state == HIGH)
  if(reading != lastButtonState){
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay){
    if(reading != buttonState){
      buttonState = reading;
      
      //only send when button is depressed (LOW)
      if(buttonState == LOW){
        sendPress();
        //Serial.print("button state is: ");
        //Serial.println(buttonState);
      }
    }
  }
  lastButtonState = reading;
  //delay(30);
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}
void loop(){
  //Serial.println("Checking button");
  checkButton();
}
  /**
  {
    const char text[] = "Your Button State is HIGH";
    radio.write(&text, sizeof(text));                  //Sending the message to receiver
    Serial.println("HIGH");
  }
  else
  {
    const char text[] = "Your Button State is LOW";
    radio.write(&text, sizeof(text));                  //Sending the message to receiver 
    Serial.println("LOW");
  }
  radio.write(&button_state, sizeof(button_state));  //Sending the message to receiver 
  delay(30);
}**/
