float set1 = 29;
float set2 = 28;
float set3 = 27;
float set4 = 26;
float set5 = 10;
float set6 = 10;
float set7 = 10;
float set8 = 10;

long onInterval = 10000, minOffInterval = 10000, maxOffInterval = 60000, offInterval = 10000; // time pump can run
unsigned long time1 = 0, time2 = 0, time3 = 0, time4 = 0, time5 = 0, time6 = 0, time7 = 0, time8 = 0;
void Pump(int pinNum, float set, float tempr, unsigned long *timeMs);
long TimeToStop(float tempr);

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,2); // pin 2 = TX, pin 3 = RX (unused)

#define ONE_WIRE_BUS 4 // Data wire is plugged into port 4 on the Arduino

OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)

DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 

void setup(void)
{
  mySerial.begin(9600); // set up serial port for 9600 baud
  delay(500); // wait for display to boot up
  // start serial port
  Serial.begin(9600);

  sensors.begin(); // Start up the library
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

    mySerial.write(254); 
  mySerial.write(128);
  mySerial.write("8 ch temp       "); 
  mySerial.write("Moty Ohevia     ");
  delay(10000);
}

void loop(void)
{ 
 // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.println("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures

  float T1 = sensors.getTempCByIndex(0);
  float T2 = sensors.getTempCByIndex(1);
  float T3 = sensors.getTempCByIndex(2);
  float T4 = sensors.getTempCByIndex(3);
  float T5 = sensors.getTempCByIndex(4);
  float T6 = sensors.getTempCByIndex(5);
  float T7 = sensors.getTempCByIndex(6);
  float T8 = sensors.getTempCByIndex(7);
  
  Pump(5, set1, T1 - 0.8, &time1);
  Pump(6, set2, T2 - 0.8, &time2);
  Pump(7, set3, T3 - 0.8, &time3);
  Pump(8, set4, T4, &time4);
  Pump(9, set5, T5, &time5);
  Pump(10, set6, T6, &time6);
  Pump(11, set7, T7, &time7);
  Pump(12, set8, T8, &time8);
  
  mySerial.write(254); 
  mySerial.write(128);
  mySerial.write("1       3       "); 
  mySerial.write("2       4       ");
  mySerial.write(254); 
  mySerial.write(130);
  mySerial.print(T1);
  mySerial.write(254); 
  mySerial.write(194);
  mySerial.print(T2);
  mySerial.write(254); 
  mySerial.write(138);
  mySerial.print(T3);
  mySerial.write(254); 
  mySerial.write(202);
  mySerial.print(T4);
  delay(1500);
  
  mySerial.write(254); 
  mySerial.write(128);
  mySerial.write("5       7       "); 
  mySerial.write("6       8       ");
  mySerial.write(254); 
  mySerial.write(130); 
  mySerial.print(T5);
  mySerial.write(254); 
  mySerial.write(194);
  mySerial.print(T6);
  mySerial.write(254); 
  mySerial.write(138); 
  mySerial.print(T7);
  mySerial.write(254); 
  mySerial.write(202);
  mySerial.print(T8);
  delay(1500);
}

void Pump(int pinNum, float set, float tempr, unsigned long *timeMs){
  Serial.print("For pin #");
  Serial.print(pinNum);
  Serial.print(" set temp is: ");
  Serial.print(set);
  Serial.print(" and temp atm is: ");
  Serial.println(tempr);

  unsigned long timeNow = millis();
  offInterval = TimeToStop(set, tempr);

  // water too cold
  if (set >= tempr){
    if ((digitalRead(pinNum) == LOW) && (timeNow >= *timeMs)){
      *timeMs = (unsigned long)(timeNow + onInterval);  // time 2 w8
      digitalWrite(pinNum, HIGH);
    }  
    else if ((digitalRead(pinNum) == HIGH) && (timeNow >= *timeMs)){
      *timeMs = (unsigned long)(timeNow + offInterval);  // time 2 w8
      digitalWrite(pinNum, LOW);
    }  
  }
  // water warmer/equal to set
  else {
    digitalWrite(pinNum, LOW);
  }
}

long TimeToStop(float set, float tempr){
  float difference = set - tempr;

  if (difference < 1.5)
    return maxOffInterval;
  else
    return minOffInterval;
}


