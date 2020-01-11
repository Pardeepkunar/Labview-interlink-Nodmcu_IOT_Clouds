//**************************************** 
#define IO_USERNAME    "pardeepnain" // in your adafruit.io account name opne nad see
#define IO_KEY         "4b578bbdc7aa4b938187744446f73867" // adafruit.io key generate 

#define WIFI_SSID       "Google"  // your phone wifi and other network
#define WIFI_PASS       "12345678899" // your wifi password

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

//**************************************** 


#include "config.h" // .h file include i this program help of #include
#define LED_PIN D2  // led pin decleare connect led in D0 pin of the hardware
AdafruitIO_Feed *digital = io.feed("digital");
//**************************************** 
#include <ESP8266WiFi.h>
#define SERVER_PORT 8000    //กำหนด Port ใช้งานในที่นี้คือ 8000 ตั้งให้ตรงกันทั้ง NodeMCU และ LabVIEW
const char* ssid = "Google";      //ชื่อวง WiFi ที่จะให้ NodeMCU Login 
const char* password = "12345678899"; //ใส่ Password วง WiFi ที่จะให้ NodeMCU Login 


WiFiServer server(SERVER_PORT);     //สร้าง object ชื่อ server และกำหนด port ซึ่งใช้ 8000
int i;       //ตัวแปรนับรอบ for และ voltage ที่อ่านจาก A0 เป็นระดับ 0-1023
float v;     //voltage ที่แปลงแล้วมีค่า 0V-3.3V
int p ;

void setup() 
{    
  //**************************************** 
  pinMode(LED_PIN, OUTPUT); 
 //**************************************** 
  pinMode(16,OUTPUT);     //กำหนดขา 16 Output เพื่อเป็น LED แสดงผลบน NodeMCU
  pinMode(D1,OUTPUT);  
     Serial.begin(115200);   //เปิดใช้ Serial Communication
  //****************************************    
     while(! Serial);

  Serial.print("Connecting to Adafruit IO");
  io.connect();

  digital->onMessage(handleMessage);// function
  while(io.status() < AIO_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  //**************************************** 
     Serial.println("");
     Serial.println("");
     WiFi.begin(ssid, password); //Login ในวง WiFi ด้วยชื่อ และ password ที่กำหนดไว้ด้านบน
     while (WiFi.status() != WL_CONNECTED)  //รอการ Login ที่ Serial Monitor จะแสดง -> วิ่งจนกว่าจะ Login สำเร็จ
     { Serial.print("->");
       delay(200);
     }
     Serial.println("");
     Serial.println("WiFi Successfully Connected");   //แสดงข้อความเมื่อ Login เข้าสู่วง WiFi สำเร็จ
     Serial.print("NodeMCU IP address: "); 
     Serial.println(WiFi.localIP());     //แสดงหมายเลข IP ที่ NodeMCU ได้รับมาจาก WiFi Router
     server.begin();             //เริ่มต้นทำงาน TCP ในฐานะ NodeMCU เป็น Server และ LabVIEW เป็น Client
     Serial.println("NodeMCU as a Server Role Started");//เริ่มทำงาน
     for(i=0;i<25;i++)   //ถ้าเชื่อมต่อสำเร็จจะเห็นไฟกระพริบที่ NodeMCU เอาไว้ใช้ดูสถานะเมื่อ NodeMCU ไม่ได้ต่อกับ Computer 
     { digitalWrite(16,HIGH);
       delay(40);
       digitalWrite(16,LOW);
       delay(40);
     }
//**************************************** 
     Serial.println("welcome");
  Serial.println(io.statusText());
  digital->get();
//****************************************    
}
void loop()  //ไฟ LED ขา 16 จะติดค้างเพื่อรอรับการเชื่อมต่อ
{       
  
  WiFiClient client = server.available();  //รอการเชื่อมต่อกับ LabVIEW ในฐานะ Client เพื่อมาขอใช้บริการจาก NodeMCU
        if (client)                              //ถ้า LabVIEW ติดต่อเข้ามาในฐานะ Client คำสั่ง if จะเป็น จริง  
        {   Serial.println("Hi...New Client");   //แสดงข้อความต้อนรับ Client ที่ Serial Communication
            for(i=0;i<2;i++)                     //จะเห็น LED กระพริบที่ NodeMCU 3 ครั้ง เอาไว้ใช้ดูสถานะเมื่อ NodeMCU ไม่ได้ต่อกับ Computer
            { digitalWrite(16,HIGH);
              delay(50);
              digitalWrite(16,LOW);
              delay(50);
            }
            while(1)       
            { while(client.available())   //ตรวจสอบข้อมูลส่งมาจาก LabVIEW ถ้ามีข้อมูลส่งมาจะทำใน Loop
              {  digitalWrite(16,HIGH);   //เมื่อมีข้อมูลส่งมา LED ที่ NodeMCU จะกระพริบ (สว่างและดับ)
                 delay(20);
                 digitalWrite(16,LOW);
                 delay(20);
                 
                 uint8_t data =client.read();  //อ่านข้อมูลที่ส่งมา
                 Serial.write(data);           //แสดงข้อมูลที่รับเข้ามาทาง Serial Communication
                 switch (data)                 //นำตัวแปร data มาตรวจสอบ
                 { 
                   case 'b':                   // data='b' ให้ NodeMCU อ่าน A0 คำนวน Volt แล้วส่งข้อมูลกลับไปที่ LabVIEW
                   i=analogRead(A0);
                   v=i*3.3/1023;
                   client.print(v);
                   break; 
                   case 'k':
                   if(p==1)
                   {
                    io.run();
                     digitalWrite(D1,HIGH);
                   }
                   delay(100);
                    client.println("..");
                    break;
                    case 'o':
                    if(p==0)
                    {
                    io.run();
                     digitalWrite(D1,LOW);
                     
                    }  
                    delay(100);
                    client.println("_");
                    break;
                             
                  } 
              }
             if(server.hasClient())  //ตรวจสอบการเชื่อมต่อกับ LabVIEW ในฐานะ Client  
             { return;               //ถ้าไม่มีการเชื่อมต่อแล้วให้ออกจากลูปแล้วไปรอรับ Client ใหม่
             }
           }
        } 
}

void handleMessage(AdafruitIO_Data *data) 
{
 
 if(data->toPinLevel() == HIGH)
    {
    Serial.println("HIGH");
     p=1;
       
    }  
  else
   {
    Serial.println("LOW");
    p=0;
    
  // write the current state to the led
     }
  digitalWrite(LED_PIN, data->toPinLevel());
  }
  

