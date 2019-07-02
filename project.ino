/* BANNY */
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 53
#define RST_PIN 49
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int pos = 0;
const int ServoRestPosition   = 20;  //Starting position
const int ServoTargetPosition = 150; //Position when event is detected



#include <HX711.h>

#define DOUT  A2
#define CLK  A3

HX711 scale(DOUT, CLK);
float calibration_factor = -476650; // calibration factor

int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;
int val = 0;
String preId = "";

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

void setup() {
  Serial1.begin(115200);
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  SPI.begin();       // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  myservo.attach(7);
  servoclose();

  Serial.println("Press T to tare");
  scale.set_scale(-476650);  //Calibration Factor obtained from first sketch
  scale.tare();             //Reset the scale to 0

  Serial.begin(9600);
}

void loop() {
  String id = "";
  float weightneed = 0;
  float weightcurrent = 0;
  float weightate = 0;
  float netweight = 0;

  val = digitalRead(inputPin);  // read input value
  if (val == 1) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);

    //delay(100);// turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected");
      //rfid();
      pirState = HIGH;
      //Serial.println(val);
    }

    id = rfid();

    if ((id == "c0bdb556" || id=="76a78589") && id!=preId) { //Check db id
      //Get details from db
      netweight = 0.10; //DB net weight
      weightate = 0.05; //DB ate weight
      weightcurrent = weightscale();

      if (weightate == 0 ) {
        weightneed = netweight - weightcurrent; //calculation of weight needed

        if (weightneed > 0) {
          servoopen();
          while(1){
            //netweight== ;
           
           // Serial.println(weightscale());
            if (weightscale()>=netweight)
              break;
            }
             Serial1.println(id);
             Serial1.println(netweight);
          servoclose();
        }
      }
      else if (weightate >0 ) {
        weightneed = (netweight - weightate) - weightcurrent; //calculation of weight needed

        if (weightneed > 0) {
          servoopen();
          while(1){
            //netweight== ;
           
           // Serial.println(weightscale());
            if (weightscale()>=weightneed)
              break;
            }
             Serial1.print(id);
            
             Serial1.print(netweight);
          servoclose();
        }
      }
      preId = id;
    }
  }

  else {
    digitalWrite(ledPin, LOW);// turn LED OFF
    //delay(200);
    if (pirState == HIGH) {
      // we have just turned of
      Serial.println("Motion ended!");
      //Serial.println(val);
      //Serial1.println(id+","+netweight);
      pirState = LOW;
    }
  }
}

String rfid() {
  String tag = "";
  if ( mfrc522.PICC_IsNewCardPresent())
  {
    if ( mfrc522.PICC_ReadCardSerial())
    {
      Serial.print("Tag UID:");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        tag = tag + String(mfrc522.uid.uidByte[i], HEX);

      }

      Serial.println();
      mfrc522.PICC_HaltA();
      Serial.print(tag);
      Serial.println();



    }
  }
  return tag;
}

float weightscale() {
  float weight = 0;

  //Serial.print("Weight: ");
  weight = scale.get_units();
  //Serial.print(scale.get_units(), 3);  //Up to 3 decimal points
  //Serial.println(" kg"); //Change this to kg and re-adjust the calibration factor if you follow lbs

  if (Serial.available())
  {
    char temp = Serial.read();
    if (temp == 't' || temp == 'T')
      scale.tare();  //Reset the scale to zero
  }
  Serial.println(weight);
  return weight;
}

void servoclose() {
  for (pos = 0; pos <= 180; pos += 1) {

    myservo.write(pos);
    //delay(15);
  }

}

void servoopen() {
  for (pos = 180; pos >= 142; pos -= 1) {
    myservo.write(pos);
    //delay(15);
  }

}
