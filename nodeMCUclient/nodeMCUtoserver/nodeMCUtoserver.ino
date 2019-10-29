
#include <ESP8266WiFi.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "HMC5883L.h"

MPU6050 accelgyro;
HMC5883L mag;


int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
#define SendKey 0  //Button to send data Flash BTN on NodeMCU

int port = 8888;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "Redmifake";  //Enter your wifi SSID
const char *password = "123456789";  //Enter your wifi Password

int count=0;
//=======================================================================
//                    Power on setup
//=======================================================================
void setup() 
{
  Wire.begin();
   accelgyro.setI2CMasterModeEnabled(false);
   accelgyro.setI2CBypassEnabled(true) ;
   accelgyro.setSleepEnabled(false);
   accelgyro.initialize();
   mag.initialize();
  Serial.begin(115200);
  pinMode(SendKey,INPUT_PULLUP);  //Btn to send data
  Serial.println();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi
 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {   
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);
}
//=======================================================================
//                    Loop
//=======================================================================

void loop() 
{
  WiFiClient client = server.available();
  
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      mag.getHeading(&mx, &my, &mz);
      int sumax = 0;
      int sumay = 0;
      int sumaz = 0;
      int16_t minax = ax;
      int16_t minay = ay;
      int16_t minaz = az;
      int16_t maxax = ax;
      int16_t maxay = ay;
      int16_t maxaz = az;
      int16_t sumgx = 0;
      int16_t sumgy = 0;
      int16_t sumgz = 0;
      int16_t mingx = gx;
      int16_t mingy = gy;
      int16_t mingz = gz;
      int16_t maxgx = gx;
      int16_t maxgy = gy;
      int16_t maxgz = gz;
      for(int i =  0; i< 100; i++){
   accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
   mag.getHeading(&mx, &my, &mz);
   minax = min(minax,ax);
   minay = min(minay,ay);
   minaz = min(minaz,az);
   maxax = max(maxax,ax);
   maxay = max(maxay,ay);
   maxaz = max(maxaz,az);

   mingx = min(mingx,gx);
   mingy = min(mingy,gy);
   mingz = min(mingz,gz);
   maxgx = max(maxgx,gx);
   maxgy = max(maxgy,gy);
   maxgz = max(maxgz,gz);
   
   sumax = sumax + ax;
   sumay = sumay + ay;
   sumaz = sumaz + az;
   sumgx = sumgx + gx;
   sumgy = sumgy + gy;
   sumgz = sumgz + gz;
   
    delay(20);
    }
    String data = String(mapfun(sumax/50,16384,-16384,1,-1),4)+ ' ' + String(mapfun(sumay/50,16384,-16384,1,-1),4)+ ' ' +
    String(mapfun(sumaz/50,16384,-16384,1,-1),4)+ ' ' + String(mapfun(minax,16384,-16384,1,-1),4)+ ' ' + 
    String(mapfun(minay,16384,-16384,1,-1),4)+ ' ' + String(mapfun(minaz,16384,-16384,1,-1),4)+ ' ' +String(mapfun(maxax,16384,-16384,1,-1),4)+ ' ' +String(mapfun(maxay,16384,-16384,1,-1),4)+ ' ' +String(mapfun(maxaz,16384,-16384,1,-1),4)+ ' ' +String(mapfun(sumgx/50,250,-250,1,-1),4)+ ' ' +String(mapfun(sumgy/50,250,-250,1,-1),4)+ ' ' +String(mapfun(sumgz/50,250,-250,1,-1),4)+ ' ' +String(mapfun(mingx,16384,-16384,1,-1),4)+ ' ' +String(mapfun(mingy,16384,-16384,1,-1),4)+ ' ' +String(mapfun(mingz,16384,-16384,1,-1),4)+ ' ' +String(mapfun(maxgx,16384,-16384,1,-1),4)+ ' ' +String(mapfun(maxgy,16384,-16384,1,-1),4)+ ' ' +String(mapfun(maxgz,16384,-16384,1,-1),4)+'111111111111111111111111111111111111111111111111111111111111';
//    Serial.println(data);



      

      char buf[136];
      data.toCharArray(buf, 136);
      for(int i = 0;i<136;i++){
        Serial.print(buf[i]);
        client.write(buf[i]);
        }
        Serial.println();
    }
    client.stop();
    Serial.println("Client disconnected");    
  }
}
float mapfun(int val,int upper, int lower, int maphigh, int maplow){
  return ((double)(val*(maphigh-maplow))/(double)((upper-lower)));
  }
  
float stdev(long total,long vals){
  float avg = total/vals;
  float avg2 = total*total/vals;
  float std = sqrt(avg2 - (avg*avg));
  return std;
  }

//=======================================================================
