#include "RS485_Zero.h"
RS485_Zero::RS485_Zero(SoftwareSerial& _rs485)
{
  rs485 = &_rs485;
}
/*
void RS485_Zero::RS485_Zero(SoftwareSerial *_rs485, byte ENABLE_PIN)
{
  //_RX = RX;
  //_TX = TX;
  _ENABLE_PIN = ENABLE_PIN;
  rs485 = _rs485;
}
*/
//<<constructor>> setup the LED, make pin 13 an OUTPUT
void RS485_Zero::RS485_init( byte ENABLE_PIN){
  //_RX = RX;
  //_TX = TX;
  _ENABLE_PIN = ENABLE_PIN;
  
  pinMode(_ENABLE_PIN, OUTPUT);  
	//rs485 =_rs485; 
  //digitalWrite(_ENABLE_PIN, RS485Receive);
  //rs485 = new SoftwareSerial(_RX, _TX);
}

//turn the LED on
void RS485_Zero::begin(int UARTBAUD){
  _UARTBAUD = UARTBAUD;
  //Serial.println(_UARTBAUD);
  //Serial.println(_ENABLE_PIN);
  rs485->begin(_UARTBAUD);
}
void RS485_Zero::fWrite (const byte what)
{
  rs485->write (what);  
  //Serial.print((int)what);
  //Serial.print(' ');
}

int RS485_Zero::fAvailable ()
{

  return rs485->available ();  
}

int RS485_Zero::fRead ()
{
  return rs485->read ();  
}


byte RS485_Zero::crc8 (const char *addr)
{
  byte len = 0;
  while((byte)addr[len] != '\0')
  {
    len++;
  }
  
  byte crc = 0;
  while (len--) 
    {
    byte inbyte = *addr++;
    for (byte i = 8; i; i--)
      {
      byte mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix) 
        crc ^= 0x8C;
      inbyte >>= 1;
      }  // end of for
    }  // end of while
  return crc;
}  // end of crc8

void RS485_Zero::EncodeMessage(char message[maxMsgLen+1], unsigned char data[maxMsgLen+1+3])
{
  int index=0;
  int index2 = 0;
  
  data[index] = STX;
  
  while(message[index2] != '\0')
  {
    index++;
    data[index] = (char)message[index2];
    index2++;
  }
  
  index++;
  data[index] = ETX;
  
  index++;
  data[index] = (char)crc8(message);
  
  index++;
  data[index] = '\0';
}
/*
void RS485_Zero::RS485_Begin(int baud)
{
  pinMode(ENABLE_PIN, OUTPUT);  // driver output enable
  _UARTBAUD = baud;
	rs485.begin(baud);
}
*/
/*
void RS485_Setup(byte _TX,byte _RX,byte _EN){
	TX_PIN = _TX;
	RX_PIN = _RX;
	ENABLE_PIN = _EN;
	rs485(RX_PIN, TX_PIN);
}
*/
bool RS485_Zero::RS485_SendMessage(char message[maxMsgLen+1])
{  
  unsigned char temp[maxMsgLen+1+2];
  
  EncodeMessage(message,temp);
 
  digitalWrite (_ENABLE_PIN, HIGH);  // enable sending

  int counter =0;
  while(temp[counter] != '\0')
  {
    fWrite(temp[counter]);
	//Serial.println(temp[counter] - '0');
	counter++;
  }  
  
  //delay(1); // Delay abit or else the RS485 will switch off before the last byte has been sent.
  delayMicroseconds (660);
  digitalWrite (_ENABLE_PIN, LOW);  // disable sending  
  return true;
}

bool RS485_Zero::RS485_ReadMessage(char data[maxMsgLen+3+1])
{
  bool bSTX = false;
  bool bETX = false;

  byte index = 0;
  int input = -1;

  byte CRC=0;

  if(fAvailable())
  {
	
    while(fAvailable())
    {
      input = fRead();

      switch(input)
      {
        // Start of message
      case STX : 
        bSTX = true;
        index = 0;
        break;

        // End of message  
      case ETX : 
        bETX = true;
        break;

      default:

        if(!bSTX)
          // Don't read if the message has not started
          break;
        else
        {

          // While not the end of the message keep collection bytes
          if(!bETX)
          {
            data[index] = input;
            index++;
            if(index >= maxMsgLen+3+1)
            {
			  Serial.println("OUT OF RANGE");
              return false;
            }
            data[index] = '\0';
			//Serial.println(data);
          }
          else
          {
			//Serial.println(input);
			
			if(bETX == true && CRC == 0){
				CRC = input;
				break;
			}
			//Serial.println(CRC);
          }  
        }            
      }
      delay(1); // Delay a bit or you will reach the end of the buffer before its even been filled.
    }

    if(CRC != crc8(data))
    {
	  //Serial.println(CRC);
	  //Serial.println(crc8(data));
	  //Serial.println("Data is not True.");
      return false;
    }
    return true;
  }
  return false;
}

// For test the bytes comming in
bool RS485_Zero::RS485_ReadPlainMessage(AvailableCallback fAvailable, ReadCallback fRead, char message[maxMsgLen+3+1])
{
  if(fAvailable())
  {
    int input = -1;
    int counter=0;
    while(fAvailable())
    {
      input = fRead();
      message[counter] = input;
      counter++;
      message[counter] = '\0';
      delay(1); // Delay a bit or you will reach the end of the buffer before its even been filled.
    }
    return true;   
  }  
  return false;
}