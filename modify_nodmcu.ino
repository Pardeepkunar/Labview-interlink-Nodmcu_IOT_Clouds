
/************************ Adafruit IO Config *******************************/


#define IO_USERNAME    "pardeepnain" // in your adafruit.io account name opne nad see
#define IO_KEY         "4b578bbdc7aa4b938187744446f73867" // adafruit.io key generate 


#define WIFI_SSID       "parduuuueep"  // your phone wifi and other network
#define WIFI_PASS       "123456789" // your wifi password

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

#include "config.h" // .h file include i this program help of #include
#define LED_PIN D1  // led pin decleare connect led in D0 pin of the hardware
AdafruitIO_Feed *digital = io.feed("digital");

// on time declear
void setup() {
pinMode(LED_PIN, OUTPUT); 
 
  Serial.begin(115200);
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");
  io.connect();

  digital->onMessage(handleMessage);// function

  while(io.status() < AIO_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("welcome");
  Serial.println(io.statusText());
  digital->get();
}

void loop() 
{
  io.run();
}
// Start function
void handleMessage(AdafruitIO_Data *data) 
{
 // when we press a switch on adafruit.io led on and off in harware
 if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");
  // write the current state to the led
  digitalWrite(LED_PIN, data->toPinLevel());
  }
  
