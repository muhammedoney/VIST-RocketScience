#include <SPI.h>
#include <RH_RF95.h>
#include <string.h>

//In this instructable I don't use a sensor to generate data. If you wan't to
//send sensordata you need to attach a sensor and write this to a variable
//Set the temp variable.
float sat = 12.0;
float lat = 38.7730000;
float lon = 33.2150000;
float alt = 10.0; //m cinsinden
int s=0;

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

//construct the lora connection
RH_RF95 rf95(NSS, DIO0);

void setup()
{
  //Reset the module
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);

  //Start serial communication
  while(!Serial);
  Serial.begin(9600);
  delay(100);

  digitalWrite(RESET, LOW);
  delay(10);
  digitalWrite(RESET, HIGH);
  delay(10);
  
  //initialize the lora connection
  while(!rf95.init()) {
  Serial.println("Initializing...");
  while (1);
  }
  Serial.println("Succeeded");

  //set the lora frequency
  if (!rf95.setFrequency(FREQ)) {
    Serial.println("Cant set frequency");
    while (1);
  }
  Serial.print("Set freq to: "); Serial.println(FREQ);
  //set the transmission power from 5 to 23
  rf95.setTxPower(5, false);
}

void loop()
{
  while(true){
    //send the value of the variable variable to the server
    Serial.println("sending to receiver");
    char radiopacket[25]= "";
    char packet0[2]="";
    char packet1[6]="";
    char packet2[6]="";
    char packet3[4]="";
    //convert a float value to a char
  
    dtostrf(sat,1,0,packet0);       //fonksiyon detayları : http://nongnu.org/avr-libc/user-manual/group__avr__stdlib.html#ga060c998e77fb5fc0d3168b3ce8771d42
    strcat(radiopacket, packet0);   //append char fonksiyonu
    dtostrf(lat,8,6,packet1);
    strcat(radiopacket, packet1);
    dtostrf(lon,8,6,packet2);
    strcat(radiopacket, packet2);
    dtostrf(alt,4,0,packet3);
    strcat(radiopacket, packet3);
    
    Serial.println(sizeof(radiopacket));
    
    Serial.print("Sending "); 
    Serial.print(sat,1);
    Serial.print("  ");
    Serial.print(lat,6);
    Serial.print("  ");
    Serial.print(lon,6);
    Serial.print("  ");
    Serial.print(alt,1);
    Serial.print("  ");
    Serial.println(radiopacket);
    Serial.println("Sending..."); 
    delay(10);
    rf95.send((uint8_t *) radiopacket, 25);
  
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Gonderilen veri paketi 1238.77344933.2152791500 ilk 2 digit uydu sayısı,sonraki 9digit lat, sonraki 9 digit lon, sonraki 4 digit altitude
    //  uydu sayısı : 12
    //  lat : 38.773449
    //  lon : 33.215279
    //  alt : 1500 m cinsinden
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    Serial.println("waiting for packet to complete....");
    delay(10);
    rf95.waitPacketSent();
  
    //waiting for reply
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
  
    Serial.println("Waiting for reply...");
    lat+=0.00001;
    lon+=0.00001;
    if (alt<1000){
      if(s==0){alt+=50;}
      else if(s==1){alt-=50;
        if (alt<=0){
          s=2;
          alt=0;}
        }
      }
    if (alt<1400 and alt>=1000){
      if(s==0){alt+=30;}
      else{alt-=30;}
      }
    if (alt<1500 and alt>=1400){
      if(s==0){alt+=10;}
      else{alt-=10;}
      }
    if (alt>=1500){
      s=1;
      alt-=10;
    }  
    delay(10);
    
    if(rf95.waitAvailableTimeout(1000))
    {
      //Print the message
      if(rf95.recv(buf, &len))
      {
        Serial.print("Got reply: ");
        Serial.println((char*)buf);
        //If you like to print the signal strength
        //Serial.println("RSSI: " rf95.lastRssi(), DEC);
      }
      else
      {
        Serial.println("failed");
      }
    }
    else
    {
      Serial.println("no reply");
    }
    delay(1000);
  }    
}
