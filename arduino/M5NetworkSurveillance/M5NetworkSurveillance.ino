#include <M5Stack.h>
#include "M5StackUpdater.h"
#include <WiFi.h>
#include <ESP32Ping.h>
// configuration
char * SSIDList[] = {"ATC00030PC","BeBoX_Home","CNW1"};
char * SSIDPass[] = {"novellini2020","8201000501","abc023d4e1"};
char * IPSurv[] = {"Mantova:10.1.1.234&Contres:10.65.37.4&Thenay:10.65.50.102&Paris:10.65.25.2&Google:216.58.215.36","Router:192.168.2.1&Google:216.58.215.36","Mantova:10.1.1.234&Contres:10.65.37.4&Vineuil:10.65.44.1&Paris:10.65.25.2&Google:216.58.215.36"};
#define NbPass (sizeof(SSIDPass)/sizeof(char *)) //array size  
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
// usable functions 
// *********** Some of Split function ****************
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
// transform string in IP *************************
IPAddress str2IP(String str) {
    IPAddress ret( getIpBlock(0,str),getIpBlock(1,str),getIpBlock(2,str),getIpBlock(3,str) );
    return ret;
}
int getIpBlock(int index, String str) {
    char separator = '.';
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = str.length()-1;
  
    for(int i=0; i<=maxIndex && found<=index; i++){
      if(str.charAt(i)==separator || i==maxIndex){
          found++;
          strIndex[0] = strIndex[1]+1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
    }    
    return found>index ? str.substring(strIndex[0], strIndex[1]).toInt() : 0;
}
// *****************************************************
int Conf = -1; // configuration number -1 none (return index for SSID and pass etc
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    //Serial.println("Failed to obtain time");
    return;
    }
  char timeHour[30];
  strftime(timeHour,30, " %d %B %Y %H:%M", &timeinfo);
  M5.Lcd.fillRect(0, 24, 320, 24, BLACK);
  M5.Lcd.drawString(String(timeHour), 0, 24);
  //display.drawString(0, 17, String(timeHour));
}
void setup() {
  // initialize the M5Stack object
  M5.begin();
  checkSDUpdater();
  M5.Power.begin();  
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.printf("Connecting to  WIFI ...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  while ( Conf == -1){
      // while nbo found perform Scan for know Access point
      int n = WiFi.scanNetworks();
      if (n == 0) {
          M5.Lcd.drawString("No network found ...", 0, 30);
          delay(1000);
      } else {
          M5.Lcd.drawString("Found "+ String(n)+" Network(s)", 0, 30);
          for (int i = 0; i < n; ++i) {
                for(int x=0; x<NbPass; ++x){
                  if (String(SSIDList[x]) == WiFi.SSID(i)) {
                    Conf = x;
                    // find pass
                    //M5.Lcd.printf(String(WiFi.SSID(i))+ " is Found");   
                    String AP = WiFi.SSID(i);
                    M5.Lcd.drawString(AP + " is Found", 0, 50);   
                    delay(1000);          
                  }
                }
                if(Conf != -1) { break; }           
            }
      }  
  }
  M5.Lcd.fillScreen(BLACK);
  // We start by connecting to a WiFi network
  M5.Lcd.drawString("Connecting", 0, 24);
  WiFi.begin(SSIDList[Conf], SSIDPass[Conf]);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.drawString("Connected to AP", 0, 0);
  //Serial.println(WiFi.localIP());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  delay(1000);
  M5.Lcd.fillScreen(BLACK);
}
void loop() {
  // put your main code here, to run repeatedly:
  //M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.drawString("*** Network Monitor ***", 0, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  printLocalTime();
  M5.Lcd.setTextSize(2);
  int index = 0;
  int ligne= 48; // start line for display
  while(getValue(String(IPSurv[Conf]),'&',index)!="") {
    String nom = getValue(getValue(String(IPSurv[Conf]),'&',index),':',0);
    String ip = getValue(getValue(String(IPSurv[Conf]),'&',index),':',1);
    //Serial.println("IP  : "+ ip);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawString(getValue(getValue(String(IPSurv[Conf]),'&',index),':',0), 0, ligne);
    //display.drawString(0, ligne, getValue(getValue(String(IPSurv[Conf]),'&',index),':',0));
    if(Ping.ping(str2IP(ip),3)) 
      {
        M5.Lcd.setTextColor(GREEN);
        M5.Lcd.fillRect(100, ligne, 320, 24, BLACK);
        M5.Lcd.drawString(String(Ping.averageTime())+" ms", 100, ligne);
        //display.drawString(75, ligne, String(Ping.averageTime())+" ms"); 
        //delay(100);      
        } else { 
        M5.Lcd.setTextColor(RED);
        M5.Lcd.fillRect(100, ligne, 320, 24, BLACK);
        M5.Lcd.drawString("is Down", 100, ligne);         
        //display.drawString(75, ligne,"is Down");
        //delay(100);
      }
      ligne=ligne+24;
    ++index;
    //delay(100);
    }
  delay(3000);

}
