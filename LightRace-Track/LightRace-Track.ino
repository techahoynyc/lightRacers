#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIN  7
#define NUMPIXELS 78

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

uint32_t GREEN1 = pixels.Color(0,255,0); //GREEN1
uint32_t RED = pixels.Color(64,0,0); //RED
uint32_t BLUE = pixels.Color(0,0,64); //BLUE
uint32_t GREEN3 = pixels.Color(0,32,0); //GREEN3
uint32_t YELLOW = pixels.Color(32,32,0); //YELLOW
uint32_t OFF = pixels.Color(0,0,0); //OFF


int pMax = 10; //number of power-up LEDs
int pSpeed = 100; //speed of player


RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
boolean button_state = 0;
int led_pin = 3;

void blink(){
  pixels.fill(pixels.Color(0,150,0));
  pixels.show();
  delay(20);
  pixels.fill(pixels.Color(0,0,0));
  pixels.show();
}

void readBTLE(int p){
  Serial.print("current state of button: ");
  Serial.println(button_state);
  if (radio.available()){              //Looking for the data.
    //button_state = HIGH;
    char text[32] = "";                 //Saving the incoming data
    radio.read(&text, sizeof(text));    //Reading the data
    radio.read(&button_state, sizeof(button_state));    //Reading the data
    if(button_state == HIGH){
      //digitalWrite(6, LOW);
      Serial.print("in low: ");
      Serial.println(button_state);
      blink();
    }
  }
}

void powerUP(int p[], int p1){
  radio.read(&button_state, sizeof(button_state));    //Reading the data
  if(button_state == HIGH){
    Serial.println("BUTTON PUSHED!");
    pixels.setPixelColor(p1,RED);
    for(int i=0;i<pMax;i++){
      //pixels.setPixelColor(p[i],YELLOW);
      if(p[i]==p1){ 
        pixels.setPixelColor(p1,BLUE);
        Serial.println("Grabbed a power-up!");
        pSpeed -= 2;
        Serial.print("Player speed now: ");
        Serial.println(pSpeed);
        //blink();
        break;
      }
      //pixels.setPixelColor(p[i],RED);
    }
  }
}

void race(){
  Serial.println("NEW RACE!");
  Serial.print("Num. power-ups: ");
  Serial.println(pMax);
  int tailLED = 0; //last LED in tail
  int pLEDs[pMax]; //power-up LED array
  pixels.clear();
  for(int p=0;p<5;p++){
    pLEDs[p] = random(0,pixels.numPixels());
    pixels.setPixelColor(pLEDs[p],YELLOW);
  }
  //powerUP(pLEDs);
  for(int i=0; i<pixels.numPixels(); i++){
    pixels.setPixelColor(tailLED,OFF);
    pixels.setPixelColor(i,GREEN1);
    powerUP(pLEDs,i);
    // create tail
    
    for(int t=1; t<5; t++){
      pixels.setPixelColor(i-t,GREEN3);
      tailLED = i-t;
    }
    pixels.show();
    delay(pSpeed); //20 is fast, 30 is ok
  }
  
}


void setup() {
  pixels.begin();
  pinMode(6, OUTPUT);
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver
}
void loop(){
  race();
  //readBTLE();
  delay(30); //wait and start next race
}
