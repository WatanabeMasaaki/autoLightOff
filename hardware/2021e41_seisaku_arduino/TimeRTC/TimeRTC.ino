// Arduino DS3232RTC Library
// https://github.com/JChristensen/DS3232RTC
//
// Example sketch illustrating Time library with Real Time Clock.
// This example is similar to the example provided with the Time Library.
// The #include statement has been changed to include the DS3232RTC library,
// a DS3232RTC object has been defined (myRTC) and the begin() method is called.

//RTCの内部時計は実際より10秒ほど遅れ
#include <DS3232RTC.h>
#include <Servo.h>

DS3232RTC myRTC;
Servo myservo;

int sensorPin = 0;
int servoPin = 3;
int buttonPin = 12;

int hourSet = 1;
int minSet = 12;

void setup()
{
    Serial.begin(115200);
    Serial1.begin(115200);
    
    myRTC.begin();
    
    setSyncProvider(myRTC.get);   // the function to get the time from the RTC
//    setTime(23, 40, 20, 30, 12, 2022);
//    myRTC.set(now());
//    if(timeStatus() != timeSet)
//        Serial.println("Unable to sync with the RTC");
//    else
//        Serial.println("RTC has set the system time");
  myservo.attach(servoPin);
  pinMode(sensorPin,INPUT);
  pinMode(buttonPin,INPUT);

  myservo.write(0);
}

void loop()
{
  
  
//  digitalClockDisplay();
  //時刻になったらライト消す
  if(hour() == hourSet && minute() == minSet && second() == 0){
    //session start
    int valB = checkBrightness();
    if(valB > 10){
      toggleLight();
      delay(3000);
      valB = checkBrightness();
      if(valB > 10){
        Serial.print("電気が消えませんでした");
        //スマホに通知
      }
    }
  }

  
  //時刻変更
  if(Serial1.available() > 0) {
    int val[6];
    int index = 0;
    
    while(Serial1.available() > 0) {
      val[index] = Serial1.read();
      Serial.println(val[index]);
      index++;
    }
    
    int digit = 0;
    for(int i = 0; i < 6; i++){
      if(val[i] == 13){
        break;
      }else{
        digit++;
        val[i] = val[i] - 48;
//        Serial.println(val[i]);
      }
    }
//    Serial.println();
    
    int timeGet = 0;
    for(int i = 0; i < digit; i++){
      int ad = val[i] * pow(10,digit-i-1);
      if(ad >= 99){
        ad++;
      }
      timeGet += ad;
      
//      Serial.println(ad);
//      Serial.println();
    }
    hourSet = timeGet / 60;
    minSet = timeGet % 60;
    Serial.print("time set to: ");
    Serial.print(hourSet);
    Serial.print(" : ");
    Serial.print(minSet);
    
//    String t = Serial.readStringUntil(",");
//    int check = t.indexOf(",");
//    String data = t.substring(0,check);
//    int timeGet = data.toInt();
//    Serial.print(timeGet);
//    minSet = timeGet % 60;
//    hourSet = timeGet / 60;
//    Serial.println(timeGet);
//    Serial.println(minSet);
//    Serial.println(hourSet);
  }

  //ボタン押したらライトオン・オフ
  if(digitalRead(buttonPin) == HIGH) {
    toggleLight();
  }
  
  delay(50);
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(' ');
  Serial.print(day());
  Serial.print(' ');
  Serial.print(month());
  Serial.print(' ');
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(':');
  if(digits < 10)
      Serial.print('0');
  Serial.print(digits);
}

void toggleLight(){
  myservo.write(180);
  delay(1000);
  myservo.write(0);
}

int checkBrightness(){
  int val = analogRead(sensorPin);
  Serial.print(" brightness:");
  Serial.print(val);
  Serial.println();

  return val;
}
