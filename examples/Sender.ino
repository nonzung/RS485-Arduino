// Send
#include <SoftwareSerial.h>
#include <RS485_Zero.h>


char message[maxMsgLen+3+1];
char Message[maxMsgLen+1] ;
SoftwareSerial serial1(11,12);
RS485_Zero Rs485Z(serial1);

void setup()
{
  Rs485Z.RS485_init(10);
  //Rs485Z= new RS485_Zero(10,11,3);
  Serial.begin(9600);
  Serial.println("System Startup - Sender");

  Rs485Z.begin(28800);
}

void loop()
{
  strcpy(Message,"Msg12345678901234567");

  if(Rs485Z.RS485_SendMessage(Message))
  {
    //Serial.print("Sending:");
    //Serial.println(Message);
  }  
  
  delay(1000);  
  
}
