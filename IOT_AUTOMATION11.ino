

 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7); // RX, TX

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
const int IN1=10;
const int IN2=11;
const int GAS_PIN = A5;
const int led1=3;
const int led2=A2;
const int ldrpin=A1;
//const int current=
const int buzzer=A3;
//int ldrstatus=0;



int GAS_VALUE;
const int buzzerPin = 2;
const int MotorPump = 8;
const int FAN = 5;
float sample,data,exceed;
 int temp1;
 const int irsensor=4;
 void rfid(void);
 char ch=0,temp[20];
String str="";
const int fireSensorPin = 12;
const int tempSensor=A0;
int max_temp = 60;
int lrdstatus=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
mySerial.begin(9600);
  
pinMode(GAS_PIN,INPUT);
pinMode(IN1,OUTPUT);
pinMode(IN1,OUTPUT);
pinMode(buzzer,OUTPUT);
pinMode(MotorPump,OUTPUT);
pinMode(FAN,OUTPUT);
  //sample=(Q2HX711.read()/100);
  
  pinMode(IN1,LOW);
  pinMode(IN2,LOW);
  //digitalWrite(buzzer,LOW);
  pinMode(FAN,HIGH);
   pinMode(GAS_PIN,LOW);
    pinMode(ldrpin,INPUT);
     pinMode(led1,INPUT);
      pinMode(led2,INPUT);
   pinMode(irsensor,INPUT);
   
  
  
  pinMode(fireSensorPin, INPUT);
  //pinMode(fireSensorPin,LOW);
   WIFI();
      Serial.write("AT+CIPSEND=0,11\r\n"); // MULTIPLE MODE SELECTION 
      delay(50);
      Serial.write("IOT BASED INDUSTRIAL PRODUCTION"); 
      delay(50);  
      Serial.write("\n\r\r"); // MULTIPLE MODE SELECTION     
      delay(1000);
  
}

void loop()
{
  //lcd.setCursor(0, 0);
   

  
  // put your main code here, t8o run repeatedly:
  Serial.println(tempSensor);
  Serial.println("Degree Celcious");
//  digitalWrite(buzzer,HIGH);
//  delay(1000);
//  digitalWrite(buzzer,LOW);
  
  RFID_MONITOR();
  TEMP_MONITOR();
   MOTOR_STP();
  IR1();
  Fire_Monitor();

  
  GAS_MONITOR();
 
  LDR_MONITOR();
  //delay(1000);
  
}
 void TEMP_MONITOR(void)
 {
int  TEMP_VALUE= analogRead(tempSensor);
  Serial.println(TEMP_VALUE);
 if(analogRead(TEMP_VALUE) == HIGH)
   {
    Serial.println(TEMP_VALUE);
        
//        digitalWrite(buzzer, HIGH);
//        delay(1000);
//        digitalWrite(buzzer, LOW);
//        delay(100);
        
        
        Serial.write("AT+CIPSEND=0,11\r\n"); // MULTIPLE MODE SELECTION 
      delay(50);
      Serial.write("Temperature Alert"); 
      delay(50);  
      Serial.write("\n\r\r"); // MULTIPLE MODE SELECTION     
      delay(1000);
  
         // gas alert message
        digitalWrite(buzzer, LOW);
        delay(3000);
        digitalWrite(buzzer, LOW);
        delay(3000);
       digitalWrite(FAN, HIGH);
       
   }
 }
 void LDR_MONITOR()
 {
  int ldrstatus=analogRead(ldrpin);
  Serial.println(ldrstatus);
    
  if(ldrstatus<=1000)
  {
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    
    Serial.println("First led is OFF");
    Serial.println("Second led is OFF");
    Serial.println(ldrstatus);
    
  }
  else 
  {
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    Serial.println("First led is ON");
    Serial.println("Second led is ON");
    Serial.println(ldrstatus);
    delay(100);
  }
 }
 
 
void Fire_Monitor (void)
{
  if(digitalRead(fireSensorPin) == LOW) 
   {
                
        Serial.write("AT+CIPSEND=0,11\r\n"); // MULTIPLE MODE SELECTION 
      delay(50);
      Serial.write("FIRE ALERT MESSAGE"); 
      delay(50);  
      Serial.write("\n\r\r"); // MULTIPLE MODE SELECTION     
      delay(1000);
  digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        
         
        digitalWrite(MotorPump, HIGH);
        delay(1000);
        digitalWrite(MotorPump, LOW);
        
        
        Serial.write("AT+CIPSEND=0,13\r\n"); // MULTIPLE MODE SELECTION 
        delay(50);
        Serial.write("Fire Detected"); 
        delay(50);  
        Serial.write("\n\r\r");
        delay(1000);
        
        Serial.write('E');
       // WIFISerial.print(myWIFISerial.write('E'));
       // fire alert message
        digitalWrite(buzzer, HIGH);
        delay(3000);
        digitalWrite(buzzer, LOW);
        delay(3000);
        digitalWrite(FAN, LOW);
        
        digitalWrite(MotorPump, LOW);
      
        digitalWrite(IN1,HIGH);
        delay(1000);
        digitalWrite(IN2,LOW);
        
    }
}
 void RFID_MONITOR(void)
 {
  if(mySerial.available()>0) //RFID
    {
    //Serial.print(ch=Serial.read());
      str=mySerial.readString();
    //delay(1000);
      //Serial.print(str);
     if(str!="")
     {//Serial.print(str);
      str.toCharArray(temp, 13);
      temp[11]='\0';
      Serial.write("AT+CIPSEND=0,16\r\n"); // MULTIPLE MODE SELECTION 
      delay(50);
      Serial.write(temp); 
      delay(50); 
      Serial.write("rfid loop"); 
       delay(50);
      Serial.write("\n\r\r"); // MULTIPLE MODE SELECTION     
      delay(1000);
  
      Serial.print(temp);
      Serial.println("rfid loop");
      
      if(!strcmp(temp,"$0001304985"))
      {
           Serial.println("rfid matched");
           Serial.write("rfid matched"); 
           delay(50);
           
          MOTOR_FWD();
          
          
          //Serial.println("waitng for sensor value");
          while( !digitalRead(irsensor) );
          //Serial.println("after while loop");
          MOTOR_FWD();
          delay(10);
          MOTOR_BWD();
          
      }
      //Serial.print("comp");
      else
      {
            Serial.println("rfid mismatched");
            Serial.write("rfid mismatched"); 
            delay(50);
          
            digitalWrite(buzzerPin, LOW);
            delay(1000);
            digitalWrite(buzzerPin, HIGH); 
      }
      //Serial.print("not comp");
        str="";}
     //rfid();
    }
  

 }

void GAS_MONITOR(void)
{
  GAS_VALUE= digitalRead(GAS_PIN);
  Serial.print(GAS_VALUE);
   Serial.write("AT+CIPSEND=0,17\r\n"); // MULTIPLE MODE SELECTION 
      delay(50);
      Serial.write("SMOKE"); 
      delay(50);  
      
  Serial.print("SMOKE:");
  Serial.println(GAS_VALUE);
if(GAS_VALUE == LOW)
{
  
  Serial.println("SMOKE DETECTED");
  delay(50);
  
  
  delay(100);
  digitalWrite(buzzerPin, HIGH);
  
  Serial.write("DETECTED");
  delay(50);
  Serial.println("DETECTED");
//  digitalWrite(buzzerPin, HIGH);
//  delay(1000);
//   digitalWrite(buzzerPin, LOW);
//  delay(100);
//  lcd.print("DETECTED");
 // lcd.clear();
  
  for(int i=0;i<5;i++)
  {
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
  delay(500);
  }
  MOTOR_Condition();
  Fan_Light();
//  SendSms("9632871904","SMOKE DETECTED");
  delay(2000);
  
  }
}
void MOTOR_FWD()
{
  digitalWrite(IN1,HIGH);
  //delay(10);
  digitalWrite(IN2,LOW);
  delay(100);
  Serial.println("GATE IS OPEN");
  Serial.write("GATE IS OPEN");
  
  
  
  }
  void MOTOR_BWD()
{
  digitalWrite(IN1,LOW);
  
  digitalWrite(IN2,LOW);
  delay(100);
  Serial.println("GATE IS CLOSING");
   Serial.write("GATE IS CLOSING");
//  lcd.print("GATE IS CLOSING");
 // lcd.clear();
  
  
  }
    void MOTOR_STP()
{
  digitalWrite(IN1,LOW);
  
  digitalWrite(IN2,LOW);
  Serial.println("GATE IS CLOSED");
   Serial.write("GATE IS CLOSED");
 
  
  
  }
  void MOTOR_Condition()
  {
    MOTOR_BWD();
    delay(2000);
    MOTOR_STP();
    delay(10);
    
    
    }
   void Fan_Light() 
   {
    digitalWrite(FAN,HIGH);
    delay(10000);
    digitalWrite(FAN,LOW);
    
    
    digitalWrite(MotorPump,HIGH);
    delay(1000);
    digitalWrite(MotorPump,LOW);
    Serial.println("PUMP is ON");
     Serial.write("PUMP IS ON");
    Serial.println("Exhaust fan is ON");
     Serial.write("EXHAUST FAN IS ON");
    
    Serial.write("PUMP IS ON");
    
     
    
    MOTOR_FWD();
    delay(1000);
    Serial.println("GATE IS OPEN");
    Serial.write("GATE IS OPEN");
     
    
      
    }
    void IR1 (void)
    {
      if(digitalRead(irsensor)== LOW)
      {
    
      Serial.write("NO HUMAN DETECTED");
      Serial.println("No HUMAN DETECTED");
      MOTOR_Condition();
  
  
    }
    else
    {
      
      Serial.println(" HUMAN DETECTED");
      Serial.write("HUMAN DETECTED");
       MOTOR_FWD();
       digitalWrite(buzzer,HIGH);
       delay(1000);
       digitalWrite(buzzer,LOW);
  
      
      
    }
      
    }
    void WIFI(void)
{
  String BUFF, buff_1;
  char ch;

  Serial.print('A');
  delay(10);
  Serial.print('T');
  delay(10);
  Serial.print('E');
  delay(10);
  Serial.print('0');
  delay(10);
  Serial.print("\r\n");

  Serial.print("1");

  Serial.print(Serial.readString());
  delay(50);
  Serial.write("AT\r\n");
   Serial.print("2");
  Serial.print(Serial.readString());
  delay(50);

  Serial.write("AT+CWMODE=2\r\n");
   Serial.print("3");
  Serial.print(Serial.readString());
  delay(50);

  Serial.write("AT+CIPMUX=1\r\n");
   Serial.print("8");
  Serial.print(Serial.readString());
  delay(50);

  Serial.write("AT+CIPSERVER=1,80\r\n");
 Serial.print("9");
  Serial.print(Serial.readString());
  delay(50);

  Serial.write("AT+CIFSR\r\n");
  Serial.print("10");
  Serial.print(Serial.readString());
  delay(50);

}
//    void GAS_weight()
//    {
//       scale.set_scale(calibration_factor); //Adjust to this calibration factor
//  Serial.print("Reading: ");
//  Serial.print(scale.get_units(), 1);
//  Serial.print(" lbs"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
//  Serial.print(" calibration_factor: ");
//  Serial.print(calibration_factor);
//  Serial.println();
//  if(Serial.available())
//  {
//    char temp = Serial.read();
//    if(temp == '+' || temp == 'a')
//      calibration_factor += 10;
//    else if(temp == '-' || temp == 'z')
//      calibration_factor -= 10;
//  if(temp>3.0)
//  {
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("GAS=");
//    lcd.print(temp);
//
//    if(data<100)
//    {
//      lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("CYLINDER IS");
//    lcd.setCursor(0, 1);
//     lcd.print("LOW"); 
// //    intGsm("       ","CYLINDER IS LOW");
//
//     lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("BOOKING GAS");
//   //  intGsm("        ","CUSTOMSER ID");
//     lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("GAS BOOKED");
//     delay(2000);
//      }
//  }
//  else
//  {
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("GAS=0.0");
//    lcd.print(temp);   
//    }
//  
//  delay(500);
//    }
//    }  

//   void SendSms( char *num1, char * str1 )
//{
//  Serial.println("DOOR I");
//    char buff[10],i=0;
//    Serial.write('A');
//    delay(100);
//    Serial.write('T');
//    delay(100);
//    Serial.write('E');
//    delay(100);
//    Serial.write('0');
//    delay(100);
//    Serial.write('\r');
//    
//    //*****************************************
//    Serial.write("AT+CMGF=1\r"); //Initialize GSM For mobile
//    delay(2000);
//    Serial.write("AT+CMGS=\"");
//    delay(2000);
//    Serial.write(num1);
//    delay(2000);
//    Serial.write("\"\r");
//    Serial.write(str1);
//    delay(2000);
//    Serial.write(26);
//    delay(2000);
//    Serial.print("sms sent");
//
//}
