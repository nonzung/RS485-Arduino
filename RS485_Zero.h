#ifndef RS485_Zero_H
#define RS485_Zero_H
#include <Arduino.h>
#include <SoftwareSerial.h>
//#include <WProgram.h>

const int maxMsgLen = 20;
const int STX = 2;
const int ETX = 3;
class RS485_Zero {
  private:
    byte _RX;
    byte _TX;
	byte _ENABLE_PIN;
    int _ADDRESS;
    int _UARTBAUD;
    SoftwareSerial* rs485;
  public:
	RS485_Zero(SoftwareSerial& _rs485);
	//void RS485_Zero(SoftwareSerial *_rs485, byte ENABLE_PIN);
    void RS485_init(byte ENABLE_PIN);
    void begin(int UARTBAUD);
	typedef void (*WriteCallback)  (const byte what);    // send a byte to serial port
	typedef int  (*AvailableCallback)  ();    // return number of bytes available
	typedef int  (*ReadCallback)  ();    // read a byte from serial port
	void fWrite (const byte what);
	int fAvailable ();
	int fRead ();
	byte crc8 (const char *addr);
	void EncodeMessage(char message[maxMsgLen+1], unsigned char data[maxMsgLen+1+3]);
	bool RS485_SendMessage(char message[maxMsgLen+1]);
	bool RS485_ReadMessage(char data[maxMsgLen+3+1]);
	bool RS485_ReadPlainMessage(AvailableCallback fAvailable, ReadCallback fRead, char message[maxMsgLen+3+1]);
};

#endif