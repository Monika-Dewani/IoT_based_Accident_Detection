
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>   // library for extra rx & tx
SoftwareSerial mySerial(D1,D2);
#include <TinyGPS.h>

TinyGPS gps;

const char *ssid = "MDD";  //ENTER YOUR WIFI SETTINGS
const char *password = "MDD123456";


int ir1=D0;
int ir2=A0;
int ir3=D7;
int acc1=D5;
int acc2=D4;
int sw=D6;
int entry_ir11=0;
int entry_ir22=0;

int buz=D3;

int all_ok=0;
float flat=0, flon=0;


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(ir1,INPUT);
  //pinMode(ir2,INPUT);
  pinMode(ir3,INPUT);
  pinMode(acc1,INPUT);
  pinMode(acc2,INPUT);
  pinMode(sw,INPUT);
    pinMode(buz,OUTPUT);

  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

Serial.println("IoT based Accident Detection");
digitalWrite(buz,1);
delay(500);
digitalWrite(buz,0);
 




  
  // put your setup code here, to run once:

}

void loop() {
  HTTPClient http1,http3, http4,http;
  Serial.println("Code Working; ");

  int ir11=digitalRead(ir1);
  int ir22=analogRead(ir2);
  int ir33=digitalRead(ir3);

  Serial.println(ir11);
  Serial.println(ir22);
  Serial.println(ir33);
  //delay(2000);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////For Highway////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if(ir11==0)   /////////////// Highway
  {
    entry_ir11=1;   
    }
    if(entry_ir11==1)   ////highway entry
    {
      if(digitalRead(acc1)==0 ||  digitalRead(acc2)==0)       //////accident
       {
        Serial.print("Accident");
        http.begin("http://mahavidyalay.in/AcademicDevelopment/ServerDemo/Led18.php?status=Accident");     //Specify request destination
        int httpCode = http.GET();            //Send the request
        String payload = http.getString();    //Get the response payload
        //Serial.println(httpCode);   //Print HTTP return code
        Serial.println(payload);    //Print request response payload
        //Serial.println("first Data Send Succesfully to Field49 = ");
        Serial.println("Send Notification to govt hospital");
        Serial.println("");
        gpsdata();

        Serial.print("LAT=");
        Serial.print(flat);// == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
        Serial.print(" LON=");
        Serial.print(flon);// == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
       
        String mylat=String(flat);
        String mylon=String(flon);
        http3.begin("http://mahavidyalay.in/AcademicDevelopment/ServerDemo/Led20.php?status="+mylat);
        int http3Code = http3.GET();            //Send the request
        String payload3 = http3.getString();    //Get the response payload
        Serial.println(payload3);    //Print request response payload
        
        http4.begin("http://mahavidyalay.in/AcademicDevelopment/ServerDemo/Led21.php?status="+mylon);
        int http4Code = http4.GET();            //Send the request
        String payload4 = http4.getString();    //Get the response payload
        Serial.println(payload4);    //Print request response payload

        


    
        delay(1000);
        
        }

        Serial.println("On Highway");
      }

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////For Any Road////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
      
      if(ir22<900 || ir33==0)   /////////////// other road
  {
    entry_ir22=1;   
    }
    if(entry_ir22==1)   ////other road  entry
    {
      if(digitalRead(acc1)==0 or  digitalRead(acc2)==0)       //////accident
       {
        Serial.print("Accident");

         
    http1.begin("http://mahavidyalay.in/AcademicDevelopment/ServerDemo/Led19.php?status=Accident");     //Specify request destination
   int http1Code = http1.GET();            //Send the request
 String payload1 = http1.getString();    //Get the response payload
  Serial.println(payload1);    //Print request response payload
 Serial.println("Send Notification to pvt  hospital");
        Serial.println("");
        gpsdata();
        Serial.print("LAT=");
        Serial.print(flat);// == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
        Serial.print(" LON=");
        Serial.print(flon);// == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
       
        
        String mylat=String(flat);
        String mylon=String(flon);
        http3.begin("http://mahavidyalay.in/AcademicDevelopment/ServerDemo/Led22.php?status="+mylat);
        int http3Code = http3.GET();            //Send the request
        String payload3 = http3.getString();    //Get the response payload
        Serial.println(payload3);    //Print request response payload
        
        http4.begin("http://mahavidyalay.in/AcademicDevelopment/ServerDemo/Led23.php?status="+mylon);
        int http4Code = http4.GET();            //Send the request
        String payload4 = http4.getString();    //Get the response payload
        Serial.println(payload4);    //Print request response payload

        
        //entry_ir22=0;
        delay(1000);
        
        }

        Serial.println("not on highway");
      }

      
//////////////switch to inform all is well//////////////// 
if(digitalRead(sw)==0)
{
  all_ok=1;
  
  Serial.println(" all is well");
  Serial.println(" send data to cloud to change status");

  if(entry_ir22==1)
{ http1.begin("http://mahavidyalay.in/AcademicDevelopment/ServerDemo/Led19.php?status=NoAccident");     //Specify request destination
   int http1Code = http1.GET();            //Send the request
 String payload1 = http1.getString();    //Get the response payload
  Serial.println(payload1);    //Print request response payload
 Serial.println("Send Notification to pvt  hospital");
  }
  else if(entry_ir11==1)
{ http1.begin("http://mahavidyalay.in/AcademicDevelopment/ServerDemo/Led18.php?status=NoAccident");     //Specify request destination
   int http1Code = http1.GET();            //Send the request
 String payload1 = http1.getString();    //Get the response payload
  Serial.println(payload1);    //Print request response payload
 Serial.println("Send Notification to pvt  hospital");
  }
  
  }      

      

  
  // put your main code here, to run repeatedly:

}

void gpsdata()
{
  bool newData = false;
                                  unsigned short sentences, failed;
                                                       unsigned long chars;
           
                                  // For one second we parse GPS data and report some key values
                                  for (unsigned long start = millis(); millis() - start < 1000;)
                                  {
                                    while (mySerial.available())
                                    {
                                      char c = mySerial.read();
                                     // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
                                      if (gps.encode(c)) // Did a new valid sentence come in?
                                        newData = true;
                                    }
                                  }
                                
                                  if (newData)
                                  {
                                    
                                    unsigned long age;
                                    gps.f_get_position(&flat, &flon, &age);
                                    Serial.print("LAT=");
                                    Serial.print(flat);// == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
                                    Serial.print(" LON=");
                                    Serial.print(flon);// == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
//                                    lcd.setCursor(0,0);
//                                    lcd.print("LAT=                     ");
//                                    lcd.setCursor(4,0);
//                                    lcd.print(flat);
//                                    lcd.setCursor(0,1);
//                                    lcd.print("LON=                    ");
//                                    lcd.setCursor(4,1);
//                                    lcd.print(flon);

                                     
                                    
               
 
                                    }
                                  
                                  gps.stats(&chars, &sentences, &failed);
                                  Serial.print(" CHARS=");
                                  Serial.print(chars);
                                  Serial.print(" SENTENCES=");
                                  Serial.print(sentences);
                                  Serial.print(" CSUM ERR=");
                                  Serial.println(failed);
  
  }
