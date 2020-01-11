#include <ESP8266WiFi.h>    //For ESP8266
#define SERVER_PORT 8000    //กำหนด Port ใช้งานในที่นี้คือ 8000 ตั้งให้ตรงกันทั้ง NodeMCU และ LabVIEW
const char* ssid = "parduuuueep";      //ชื่อวง WiFi ที่จะให้ NodeMCU Login 
const char* password = "123456789"; //ใส่ Password วง WiFi ที่จะให้ NodeMCU Login 

WiFiServer server(SERVER_PORT);     //สร้าง object ชื่อ server และกำหนด port ซึ่งใช้ 8000
int i;       //ตัวแปรนับรอบ for และ voltage ที่อ่านจาก A0 เป็นระดับ 0-1023
float v;     //voltage ที่แปลงแล้วมีค่า 0V-3.3V

int pulsepin = 5;  //กำหนดขา GPIO5 เป็น input ของการอ่านความถี่
int Htime;         //ช่วงที่สัญญาณเป็น HIGH
int Ltime;         //ช่วงที่สัญญาณเป็น LOW
float Ttime;       //เวลาทั้งรูปคลื่น = Htime+Ltime
float freq;        //ตัวแปรความถี่

const int trigPin = 2;  //กำหนดขา GPIO2 เป็น trigPin
const int echoPin = 4;  //กำหนดขา GPIO4 เป็น echoPin

long duration;
int distance;

void setup() 
{    pinMode(pulsepin, INPUT); 
     pinMode(16,OUTPUT);     //กำหนดขา 16 Output เพื่อเป็น LED แสดงผลบน NodeMCU
     pinMode(trigPin, OUTPUT);
     pinMode(echoPin, INPUT); 
     Serial.begin(115200);   //เปิดใช้ Serial Communication
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
}
void loop()  //ไฟ LED ขา 16 จะติดค้างเพื่อรอรับการเชื่อมต่อ
{       WiFiClient client = server.available();  //รอการเชื่อมต่อกับ LabVIEW ในฐานะ Client เพื่อมาขอใช้บริการจาก NodeMCU
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
                 { case 'a':                   // data='a' ส่งข้อมูลกลับไปที่ LabVIEW เป็นข้อความเปล่า เพิ่ม case ได้เรื่อยๆหากต้องการ
                     Serial.println("");
                     client.println("");      
                   break;
                   case 'b':    // data='b' ให้ NodeMCU อ่านความถี่จากขา GPIO5 แล้วส่งข้อมูลกลับไปที่ LabVIEW
                     Htime = pulseIn(pulsepin, HIGH);  //เวลาที่สัญญาณเป็น HIGH
                     Ltime = pulseIn(pulsepin, LOW);   //เวลาที่สัญญาณเป็น LOW
                     Ttime=Htime+Ltime;                //เวลาของสัญญาณใน 1 รูปคลื่น
                     freq=1000000/Ttime;               //สูตร f=(1/t) หน่วยเวลาเป็น us พอกลับขึ้นไปเป็น 1 ล้าน
                     Serial.println(freq);
                     client.println(freq);      //ส่งค่า voltage กลับไปยัง LabVIEW
                     digitalWrite(16,HIGH);
                     delay(20);
                     digitalWrite(16,LOW);
                     delay(20);
                   break;
                   case 'c':     // data='c' ให้ NodeMCU อ่าน Voltage จากขา A0 แล้วส่งข้อมูลกลับไปที่ LabVIEW
                      i=analogRead(0);
                      v=(i*3.3)/1023;
                      Serial.println(v);
                      client.println(v); 
                   break;
                   case 'd':    // data='d' ให้ NodeMCU อ่านระยะทาง จากขา Ultrasonic แล้วส่งข้อมูลกลับไปที่ LabVIEW
                      digitalWrite(trigPin, LOW); //Reset ขา trigPin
                      delayMicroseconds(2);
                      
                      digitalWrite(trigPin, HIGH); //ให้ขา trigPin เป็น HIGH
                      delayMicroseconds(10);
                      digitalWrite(trigPin, LOW); //ให้ขา trigPin เป็น LOW
                      
                      duration = pulseIn(echoPin, HIGH); //อ่านค่าจากขา echoPin ด้วย pulseIn ได้ค่าเวลาหน่าย ms
                      
                      distance= duration*0.034/2; //คำนวณระยะทางหน่วยของ distance เป็น cm
                      
                      Serial.print("Distance: "); Serial.println(distance);
                      client.println(distance); 
                      
                   break;                  
                 } 
              }
             if(server.hasClient())  //ตรวจสอบการเชื่อมต่อกับ LabVIEW ในฐานะ Client  
             { return;               //ถ้าไม่มีการเชื่อมต่อแล้วให้ออกจากลูปแล้วไปรอรับ Client ใหม่
             }
           }
        } 
}
