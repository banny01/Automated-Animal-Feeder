#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
// Set these to run example.
#define FIREBASE_HOST "animal-feeder-94c.firebaseio.com"
#define FIREBASE_AUTH "X9WYQ4AA32e6K2zwcx1Oi2yCiRxwPs6n601TuZxw"
//Change line with your WiFi router name and password
#define WIFI_SSID "SDialog"  
#define WIFI_PASSWORD "ashi322pro*"
#include <time.h>

int timezone = (5.5) * 3600;
int dst = 0;
String s="";
int retval=0;

void getdata(){
  s="";
  if (Serial.available()) {
    //Serial.write(Serial.read());
    while (Serial.available() > 0){
    s+=char(Serial.read());
    }
      //Serial.println("data = "+s);
      //delay(1000);
//return s;
  //s="";
  }
  retval+=1;
  
}
int i=1;
void TimeandDate(){
getdata();  

  String s1,s2,s3,d1,d2,d3,S1,S2;
   time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);

// s1=p_tm->tm_mday+"-"+p_tm->tm_mon + 1+"-"+p_tm->tm_year + 1900+" "+p_tm->tm_hour+":"+p_tm->tm_min+p_tm->tm_sec;

 s1=p_tm->tm_mday;
 s2=p_tm->tm_mon + 1;
 s3=p_tm->tm_year + 1900;

 d1=p_tm->tm_hour;
 d2=p_tm->tm_min;
 d3=p_tm->tm_min+p_tm->tm_sec;

 S1=d1+":"+d2+":"+d3;
 S2=S1+"/data";
  Serial.println(S1);
// set value
//Firebase.setFloat(S2, 42.0);
Firebase.setString(S2, s);

// handle error
if (Firebase.failed()) {
Serial.print("setting /number failed:");
Serial.println(Firebase.error());  
}

}

void setup() {
  Serial.begin(115200);
  
// connect to wifi.
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("connecting");
while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}
Serial.println();
Serial.print("connected: ");
Serial.println(WiFi.localIP());

configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK");

Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  
  delay(1000); 

//

while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
 //String a=getdata();
//Serial.println(s);
}


void loop(){
  //getdata();
  //Serial.println(s);
  if(Serial.available()>0){
    //getdata();
    //if(retval==2){
    TimeandDate();
    //s="";
   // retval=0;
   // }
    
    
  }
}



