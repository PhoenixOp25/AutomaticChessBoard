//Created by Team !ABHIMANYU
#include <ESP8266WiFi.h>
 
#define ML1 D1
#define ML2 D2
#define MR1 D5
#define MR2 D6
 
String  str;
int c=0,d=0;
int v=200, vl1=0,vl2=0,vr1=0,vr2=0;
 
WiFiServer server(80);
 
float a = 0, b = 0;
 
void setup()
{
  str = "";
 
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  WiFi.disconnect();
  delay(2000);
  WiFi.begin("realme 8 Pro", "password.c");
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
 
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
 delay(5000);
}
 
 
void loop()
{
 
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  str = (client.readStringUntil('\r'));
  client.println(str);
 
  str.remove(0, 5);
  str.remove(str.length() - 9, 9);
 
 
 
}