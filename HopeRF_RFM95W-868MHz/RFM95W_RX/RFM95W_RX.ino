#include <SPI.h>
#include <RH_RF95.h>
//In this instructable I don't use a sensor to generate data. If you wan't to
//send sensordata you need to attach a sensor and write this to a variable
//Set the temp variable.
float variable = 5.0;

//Pin 11 on the arduino is het MOSI pin master out slave in for data from arduino to radio
//Pin 12 on the arduino is the MISO pin master in slave out for data from radio to the arduino 
//Pin 13 on the arduino is the sck pin is the clock pin

//Following pins can be any digital pin on the arduino
//Pin 2 is the reset pin for the radio low sets it into reset
#define RESET 2

//Pin 3 used for interrupt request notification
#define DIO0 3

//Pin 4 is dropped low to start SPI transaction
#define NSS 4

//Define your frequency.
//Check the allowed frequencies in your country and the 
//frequency on the back of your board.
//#define FREQ 868.0 //433.0 //915.0
#define FREQ 433.0

//define the ledpin
#define LED 13

//construct the lora connection
RH_RF95 rf95(NSS, DIO0);



void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);

  while(!Serial);
  Serial.begin(9600);
  delay(100);

  digitalWrite(RESET, LOW);
  delay(10);
  digitalWrite(RESET, HIGH);
  delay(10);

  //initialize the lora connection
  while(!rf95.init()) {
  //Serial.println("Initializing...");
  while (1);
  }
  //Serial.println("Succeeded");

  //set the lora frequency
  if (!rf95.setFrequency(FREQ)) {
    //Serial.println("Cant set frequency");
    while (1);
  }
  //Serial.print("Set freq to: "); Serial.println(FREQ);
  //set the transmission power from 5 to 23
  rf95.setTxPower(5, false);
}

void loop()
{
  if (rf95.available())
  {
    //read the message())
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {
      //receive message
      digitalWrite(LED, HIGH);
      //RH_RF95::printBuffer("Received: " , buf, len);
      //Serial.print("Got: ");
      Serial.println((char*)buf);

      //Send reply
      uint8_t data[] = "Ack";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      //Serial.println("Reply");
      digitalWrite(LED, LOW);
    }
    else
    {
      //if receive failed
      Serial.println("failed");
    }
  }
}
