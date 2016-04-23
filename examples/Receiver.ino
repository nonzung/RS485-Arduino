// Receive
#include <SoftwareSerial.h>
#include <RS485_Zero.h>


char message[maxMsgLen+3+1];
SoftwareSerial serial1(11,12);
RS485_Zero Rs485Z(serial1);

void setup()
{
  Rs485Z.RS485_init(10);
  Serial.begin(9600);
  Serial.println("System Startup - Receiver");

  Rs485Z.begin(28800);
}

void loop()
{
  //if(RS485_ReadPlainMessage(fAvailable,fRead,message))
  if(Rs485Z.RS485_ReadMessage(message))
  {
    Serial.print("Receiving:");
    Serial.println(message);
  }

}
