#include<ESP8266WiFi.h>
#include<ThingSpeak.h>
#include<WiFiClient.h>
//**************************************

#define SERVER_PORT 8000    //กำหนด Port ใช้งานในที่นี้คือ 8000 ตั้งให้ตรงกันทั้ง NodeMCU และ LabVIEW
const char* ssid = "parduuuueep";      //ชื่อวง WiFi ที่จะให้ NodeMCU Login 
const char* password = "123456789";
WiFiServer server(SERVER_PORT);     //สร้าง object ชื่อ server และกำหนด port ซึ่งใช้ 8000
int i;       //ตัวแปรนับรอบ for และ voltage ที่อ่านจาก A0 เป็นระดับ 0-1023
float v;     //voltage ที่แปลงแล้วมีค่า 0V-3.3V

//**************************************

WiFiClient client ;
char ssidc[]="parduuuueep";
char passwordc[]="123456789";
char WriteApi_Key[]="RAB5WM4U1I9MD4KA";
unsigned long ChannelId=774282;

void setup() {
  // put your setup code here, to run once:  
  
  Serial.begin(115200);
  delay(10);
  //**************************************
  pinMode(16,OUTPUT); 
  Serial.println("");
     Serial.println("");
     WiFi.begin(ssid, password); //Login ในวง WiFi ด้วยชื่อ และ password ที่กำหนดไว้ด้านบน
     
//************************************** 
  WiFi.begin(ssidc,passwordc);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    }
  Serial.println("Connected");
  ThingSpeak.begin(client);
//************************************** 
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
  //************************************** 
}

void loop() {
  // put your main code here, to run repeatedly:


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
                      
                      ThingSpeak.writeField(ChannelId,1,v,WriteApi_Key);
                      delay(1000);
                   //ส่งค่า voltage กลับไปยัง LabVIEW
                   break;
                 } 
              }
             if(server.hasClient())  //ตรวจสอบการเชื่อมต่อกับ LabVIEW ในฐานะ Client  
             { return;               //ถ้าไม่มีการเชื่อมต่อแล้วให้ออกจากลูปแล้วไปรอรับ Client ใหม่
             }
           }
        } 

  
  }
