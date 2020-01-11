/*
 *  Created by TheCircuit
*/
//****************************************************
#include <ESP8266WiFi.h>
#define SERVER_PORT 8000    //กำหนด Port ใช้งานในที่นี้คือ 8000 ตั้งให้ตรงกันทั้ง NodeMCU และ LabVIEW
const char* ssid = "parduuuueep";      //ชื่อวง WiFi ที่จะให้ NodeMCU Login 
const char* password = "123456789"; //ใส่ Password วง WiFi ที่จะให้ NodeMCU Login 
int deep=0;
int deep_1=0;

WiFiServer server(SERVER_PORT);     //สร้าง object ชื่อ server และกำหนด port ซึ่งใช้ 8000
int i;       //ตัวแปรนับรอบ for และ voltage ที่อ่านจาก A0 เป็นระดับ 0-1023
int v=0;     //voltage ที่แปลงแล้วมีค่า 0V-3.3V
//****************************************************

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1

#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;

void setup() 
{
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

//**********************************************************
     pinMode(16,OUTPUT);
      pinMode(D8,OUTPUT);//กำหนดขา 16 Output เพื่อเป็น LED แสดงผลบน NodeMCU
     Serial.begin(115200);   //เปิดใช้ Serial Communication
     Serial.println("");
     Serial.println("");
     WiFi.begin(ssid, password); //Login ในวง WiFi ด้วยชื่อ และ password ที่กำหนดไว้ด้านบน
     while (WiFi.status() != WL_CONNECTED)  //รอการ Login ที่ Serial Monitor จะแสดง -> วิ่งจนกว่าจะ Login สำเร็จ
     { 
      Serial.print("->");
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
//***********************************************************
  
}
void loop() 
{
  //************************************************************
 WiFiClient client = server.available();  //รอการเชื่อมต่อกับ LabVIEW ในฐานะ Client เพื่อมาขอใช้บริการจาก NodeMCU
        if (client)                              //ถ้า LabVIEW ติดต่อเข้ามาในฐานะ Client คำสั่ง if จะเป็น จริง  
        {   
            Serial.println("Hi...New Client");            
            //แสดงข้อความต้อนรับ Client ที่ Serial Communication
            for(i=0;i<2;i++)                     //จะเห็น LED กระพริบที่ NodeMCU 3 ครั้ง เอาไว้ใช้ดูสถานะเมื่อ NodeMCU ไม่ได้ต่อกับ Computer
            { digitalWrite(16,HIGH);
              delay(50);
              digitalWrite(16,LOW);
              delay(50);
            }
            while(1)       
            { 
               
               //ตรวจสอบข้อมูลส่งมาจาก LabVIEW ถ้ามีข้อมูลส่งมาจะทำใน Loop
              while(client.available())  
               { 

              digitalWrite(16,HIGH);   //เมื่อมีข้อมูลส่งมา LED ที่ NodeMCU จะกระพริบ (สว่างและดับ)
                 delay(20);
                 digitalWrite(16,LOW);
                 delay(20);
                 uint8_t data =client.read(); 
                 Serial.write(data);           //แสดงข้อมูลที่รับเข้ามาทาง Serial Communication
                 switch (data) 
               
                 { 
                      case 'a':                   // data='a' ส่งข้อมูลกลับไปที่ LabVIEW เป็นข้อความเปล่า เพิ่ม case ได้เรื่อยๆหากต้องการ
                         client.print("     "); 
                         Serial.println(deep);
                         Serial.print("........."); 
                     
                                         if (deep<9000)
                                         {
                                                            
                                          if ( ! mfrc522.PICC_IsNewCardPresent()) 
                                          {
                                        //    return;
                                          }
                                        //  // Select one of the cards
                                          if ( ! mfrc522.PICC_ReadCardSerial()) 
                                          {
                                        //    return;
                                          }
                                          //Show UID on serial monitor
                                          Serial.println();
                                          Serial.print(" UID tag :");
                                          String content= "";
                                          byte letter;
                                              for (byte i = 0; i < mfrc522.uid.size; i++) 
                                              {
                                                 Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                                                 Serial.print(mfrc522.uid.uidByte[i], HEX);
                                                 content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
                                                 content.concat(String(mfrc522.uid.uidByte[i], HEX));
                                              }
                                          content.toUpperCase();
                                          Serial.println();
                                              if (content.substring(1) == "83 F1 3A 1D") //change UID of the card that you want to give access
                                              {
                                            
                                                Serial.println(" Welcome Mr.Pardeep ");
                                                digitalWrite(D8,HIGH);
                                                   
                                              }
                                             deep=deep+1;
                                             delay(100);                               
                                         }
                                             break;
                   case 'b':                   
                     i=analogRead(A0);
                     v=1023;
                     client.print(v);
                     if (deep_1<9000)
                               {
                                                  
                                if ( ! mfrc522.PICC_IsNewCardPresent()) 
                                {
                              //    return;
                                }
                              //  // Select one of the cards
                                if ( ! mfrc522.PICC_ReadCardSerial()) 
                                {
                              //    return;
                                }
                                //Show UID on serial monitor
                                Serial.println();
                                Serial.print(" UID tag :");
                                String content= "";
                                byte letter;
                                for (byte i = 0; i < mfrc522.uid.size; i++) 
                                {
                                   Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                                   Serial.print(mfrc522.uid.uidByte[i], HEX);
                                   content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
                                   content.concat(String(mfrc522.uid.uidByte[i], HEX));
                                }
                                content.toUpperCase();
                                Serial.println();
                               
                                if(content.substring(1) == "04 D5 26 83")  
                                {
                                  Serial.println(" light on ");
                                digitalWrite(D8,LOW);
                                }
                                
                                deep_1=deep_1+1;
                                  delay(100);                               
                                                }
                                                 break;
                 }        
              }
             if(server.hasClient())  //ตรวจสอบการเชื่อมต่อกับ LabVIEW ในฐานะ Client  
             { 
              return;               //ถ้าไม่มีการเชื่อมต่อแล้วให้ออกจากลูปแล้วไปรอรับ Client ใหม่
             }
           }
        } 

}
//********************************************************* 
