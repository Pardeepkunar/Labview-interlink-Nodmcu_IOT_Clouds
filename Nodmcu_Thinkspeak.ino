#include<ESP8266WiFi.h>
#include<ThingSpeak.h>
#include<WiFiClient.h>


WiFiClient client ;
char ssid[]="Google";
char password[]="12345678899";
char WriteApi_Key[]="RAB5WM4U1I9MD4KA";
unsigned long ChannelId=774282;

void setup() {
  // put your setup code here, to run once:   
  Serial.begin(9600);
  delay(10);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    }
  Serial.println("Connected");
  ThingSpeak.begin(client);
}

void loop() {
  // put your main code here, to run repeatedly:
 float variable= random(25,100);
  ThingSpeak.writeField(ChannelId,1,variable,WriteApi_Key);
  delay(1000);
  }
