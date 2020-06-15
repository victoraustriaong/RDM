
#include <Sim800l.h>
#include <SoftwareSerial.h> //is necesary for the library!! 
Sim800l Sim800l;  //to declare the library

String textSms,numberSms;
uint8_t index1;
bool error;

int receiverCh8=3;
int ch8=0;
int ls=1;
int lsLowOut=9;

int limitSwitch=10;
int R_en=7;
int L_en=8;
int RPWM=5;
int LPWM=6;
char num[13];
int forw=0;
int cstop=0;
void setup() 
{
  pinMode(receiverCh8,INPUT);
  pinMode(limitSwitch,INPUT_PULLUP);
  pinMode(lsLowOut,OUTPUT);
  digitalWrite(lsLowOut,LOW);
  
  pinMode(R_en, OUTPUT);
  pinMode(L_en, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);

  Serial.begin(9600);
    Sim800l.begin(); // initializate the library. 
    //Sim800l.reset();
    //don't forget to catch the return of the function delAllSms! 
    error=Sim800l.delAllSms(); //clean memory of sms;
    Serial.println(error);
}


void loop() {
digitalWrite(R_en, HIGH);
digitalWrite(L_en, HIGH);
ch8=pulseIn(receiverCh8,HIGH,25000);
Serial.println(ch8);

textSms=Sim800l.readSms(1); //read the first sms
Serial.println(textSms);
if (textSms.indexOf("OK")!=-1)
  {
    numberSms=Sim800l.getNumberSms(1);  // Here you have the number
    
    numberSms.toCharArray(num,13);
    Serial.println(num);
    textSms.toUpperCase();  // set all char to mayus ;)
    if (textSms.indexOf("TURNON")!=-1)
    {
      //Serial.println("System ON");
      Sim800l.sendSms(num,"System Now On");
      forw=1;
      cstop=0;
    }
    else if (textSms.indexOf("TURNOFF")!=-1){
      //Serial.println("System OFF");
      Sim800l.sendSms(num,"System Now Off");   
      cstop=1;
      forw=0;           
    }
    else{
      //Serial.println("Keyword not accepted");
      Sim800l.sendSms(num,"Nothing happened!!!");
    }

    Sim800l.delAllSms();
  }

if (ch8 >= 1898 && ch8 <=1950)
{
   ls=digitalRead(limitSwitch);
   Serial.println(ls);
   if (ls==0)
   {
      digitalWrite(RPWM, LOW);
      digitalWrite(LPWM, LOW);
   }
   else
   {
      digitalWrite(RPWM, LOW); 
      digitalWrite(LPWM, HIGH); 
      //Serial.print("Reverse");
   }
 }

if ((ch8 >= 1488 && ch8 <=1508) || cstop==1)
{
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, LOW);
 
  //Serial.print("Stops");
}

if ((ch8 >=1070 && ch8 <= 1090) || forw==1)
{
  digitalWrite(RPWM, HIGH);
  digitalWrite(LPWM, LOW);
 
  //Serial.print("Forward");
}

}


 
