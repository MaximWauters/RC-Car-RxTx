/* Receiver code for the Arduino Radio control for RC car project
 * Install the NRF24 library to your IDE
 * Upload this code to the Arduino UNO, NANO, Pro mini (5V,16MHz)
 * Connect a NRF24 module to it:
 
    Module // Arduino UNO,NANO
    
    GND    ->   GND
    Vcc    ->   3.3V
    CE     ->   D9
    CSN    ->   D10
    CLK    ->   D13
    MOSI   ->   D11
    MISO   ->   D12
*/

#include <FastLED.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define LED_PIN     6
#define NUM_LEDS    3

const uint64_t pipeIn = 0xE8E8F0F0E1LL;       //Remember that this code is the same as in the transmitter
RF24 radio(9, 10);                            //CSN and CE pins
Servo esc;
Servo servo;
CRGB leds[NUM_LEDS];
                                          
struct Received_data {                        // The sizeof this struct should not exceed 32 bytes
  byte ch1;
  byte ch2;
};

int ch1_value = 0;
int ch2_value = 0;

Received_data received_data;

void setup()
{
  Serial.begin(9600);
  received_data.ch1 = 127;
  received_data.ch2 = 127;
  
  radio.begin();                              //Once again, begin and radio configuration
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);  
  radio.openReadingPipe(1,pipeIn);
 
  radio.startListening();                     //We start the radio comunication

  esc.attach(3);
  esc.writeMicroseconds(1000);                //initialize the signal to 1000

  servo.attach(5);
  servo.write(90);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

}

unsigned long last_Time = 0;

void receive_the_data()                       //We create the function that will read the data each certain time
{
  while ( radio.available() ) {
  radio.read(&received_data, sizeof(Received_data));
  last_Time = millis();                       //Here we receive the data
  }
}

void loop()
{
  receive_the_data();                         //Receive the radio data

  ch1_value = map(received_data.ch1,177,70,100,2800);
  Serial.print(ch1_value); //received_data
  Serial.print(" ----------------------- CH1 ");
  Serial.println();

  ch2_value = map(received_data.ch2,0,255,30,210);
  //Serial.print(ch2_value); //ch1_value
  //Serial.print(" -========---------------------- CH2 ");
  //Serial.println();
  
  esc.writeMicroseconds(ch1_value); 
  servo.write(ch2_value);

  leds[0] = CRGB(255, 0, 255);                 //Leds RGB for below
  FastLED.show();
  leds[1] = CRGB(255, 0, 255);
  FastLED.show();
  leds[2] = CRGB(255, 0, 255);
  FastLED.show();
}
