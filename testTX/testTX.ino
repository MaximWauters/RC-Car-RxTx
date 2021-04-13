/* Tranmsitter code for the Arduino Radio control for RC Car project
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

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL; //Remember that this code should be the same for the receiver

RF24 radio(9, 10);

struct Data_to_be_sent {                       // The sizeof this struct should not exceed 32 bytes
  byte ch1;
  byte ch2; 
};

Data_to_be_sent sent_data;

void setup()
{
  //Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(my_radio_pipe);  
  sent_data.ch1 = 127;
  sent_data.ch2 = 127;
  
}

void loop()
{
  /*If your channel is reversed, just swap 0 to 255 by 255 to 0 below
  EXAMPLE:
  Normal:    data.ch1 = map( analogRead(A0), 0, 1024, 0, 255);
  Reversed:  data.ch1 = map( analogRead(A0), 0, 1024, 255, 0);  */
  //Serial.println(analogRead(A0));
  sent_data.ch1 = map( analogRead(A0), 0, 1024, 0, 255);
  sent_data.ch2 = map( analogRead(A1), 0, 1024, 0, 255);                  // 2 channel receiver
    
  radio.write(&sent_data, sizeof(Data_to_be_sent));
}
