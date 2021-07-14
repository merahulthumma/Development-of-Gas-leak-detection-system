#include <LPC214X.H> 

#include "lcd.h" 

#include  "uart.h"  

 
 

main() 

{ 

	VPBDIV=0X02; 					//set the clock speed for 30 mhz 

	PINSEL0=0X00000005;				//selecting uart o port pins 

	IODIR1 |= 0X003F0000;           //calling direction for lcd pins 

	uartini();				//calling uart inilization function 

	delay(50); 

	lcdini();					//calling lcd inilization function 

	delay(50); 

	cmd(0x80); 

	lcdstr("GAS  LEAK DETECTION AND"); 

	cmd(0xc0); 

	lcdstr("LOCATION SYSTEMS"); 

	cmd(0x01);			//lcd clear command 

	delay(100); 

	while(1) 

	{ 

		if(!((IOPIN0)&(1<<6))) 

		{ 

			cmd(0x01); 

			lcdstr("GAS-1 IS DETECTED"); 

			sendstring("GAS-1 IS DETECTED\r\n"); 

		} 

		else if(!(( IOPIN0)&(1<<7))) 

		{ 

			cmd(0x01); 

			lcdstr("GAS-2 IS DETECTED"); 

			sendstring("GAS-2 IS DETECTED\r\n");	 

		} 

		else 

		{ 

			cmd(0x01); 

			lcdstr("GAS-1 AND GAS-2"); 

			cmd(0xc0); 

			lcdstr(" NOT  DETECTED"); 

			sendstring("NOT DETECTED\r\n");	 

		} 

	} 

} 

 
 

 
 

#define LCD 0X003C0000 

#define RS  0X00010000 

#define EN  0X00020000 

 
 

void lcdini(void); 

void lcd_display(unsigned char x); 

void cmd(unsigned char x); 

void lcdstr(unsigned char *str); 

void delay(int count); 

 
 

void lcdini()   // lcd initilization function 

{ 

	cmd(0X02);  // to initialize LCD in 4-bit mode. 

	cmd(0X28);  //to initialize LCD in 2 lines, 5X7 dots and 4bit mode. 

	cmd(0x0e);  // cursor ON 

	cmd(0X06); 

	cmd(0X01);  // clear lcd 

	cmd(0X80);  // cursor indicating first line first position 

} 

 
 

//----------------------------------------------------------------- 

 
 

void lcd_display(unsigned char x)  // lcd display funtion 

{ 

	unsigned int temp;    //initilize variable 

	delay(700);        // calling delay 

	IOCLR1 |= RS; 

	IOCLR1 |= EN; 

	IOCLR1 |= LCD; 

	temp=(x>>4)&0x0f;   // rotating value of x by 4 and anding with 0x0f 

	IOSET1 |=(temp<<18);  //put value of temp at on lcd pins 

	IOSET1 |=RS;    // set re pin 

	IOSET1 |=EN;    // set enable pin 

	delay(10); // hault for some time 

	IOCLR1 |=EN;   // clear enable pin 

 
 

	delay(1000); // calling delay function 

	IOCLR1 |= RS; 

	IOCLR1 |= EN; 

	IOCLR1 |= LCD; 

	temp=x&0x0f;    // anding value of x with 0x0f 

	IOSET1 |=(temp<<18); // putting value of temp on lcd data pin 

	IOSET1 |=RS;  // set re pin  

	IOSET1 |=EN;  // set enable pin 

	delay(10); // hault for some time 

	IOCLR1 |=EN;  // clear enable pin 

	delay(100); // calling delay function 

} 

//--------------------------------------------------------------------- 

void cmd(unsigned char x)   // lcd command funtion 

{ 

	unsigned int temp;      //initilize variable for msb 

	IOCLR1 |= RS; 

	IOCLR1 |= EN; 

	IOCLR1 |= LCD; 

	temp=(x>>4)&0x0f;   // rotating value of x by 4 and anding with 0x0f 

	IOSET1 |=(temp<<18);  //put value of temp at on lcd pins 

	IOCLR1 |=RS;  // clear re pin 

	IOSET1 |=EN;  // set enable pin 

	delay(100); // hault for some time 

	IOCLR1 |=EN;  // clear enable pin 

 
 

	delay(100);  // calling delay function 

	IOCLR1 |= RS; 

	IOCLR1 |= EN; 

	IOCLR1 |= LCD; 

	temp=x&0x0f;     // anding value of x with 0x0f 

	IOSET1 |=(temp<<18);  // putting value of temp on lcd data pin 

	IOCLR1 |=RS;  // clear re pin  

	IOSET1 |=EN;  // set enable pin 

	delay(100); // hault for some time 

	IOCLR1 |=EN; // clear enable pin 

	delay(100);	 // calling delay function 

} 

//--------------------------------------------------------------------- 

void lcdstr(unsigned char *str)  // funtion to write sting on lcd 

{ 

	while(*str!='\0')  // check str for NULL 

	{ 

		lcd_display(*str);  // write one characture from string 

		str++;     // increament string 

	} 

} 

//------------------------------------------------------------------------ 

void delay(int count) 

{ 

	int j=0,i=0; 

	for(j=0;j<count;j++) 

	{ 

		for(i=0;i<500;i++); 

	} 

} 

//--------------------------------------------------------------------------------------------- 

 
 

 
 

 
 

void uartini(void); 

void transmituart0(unsigned char val); 

unsigned char receiveuart0(void); 

void sendstring(unsigned char *str); 

 
 

void uartini() 

{ 

 
 

U0LCR=0x83; //8-N-	1, enable divisors 

U0DLL=0xC3; //9600 baud (9615) 

U0DLM=0x00; 

U0LCR=0x03; //8-N-1, disable divisors 

} 

//------------------------------------------------------------------------ 

void transmituart0(unsigned char val)				//sends a byte through Uart 

{ 

	while(!(U0LSR & 0x20));				  // checking TXD Buffer Empty 

	U0THR =val;				// Write Character 

}  

//--------------------------------------------------------------------------- 

unsigned char receiveuart0(void) 

{ 

	while(!(U0LSR & 0x01));			   // Wait RXD Receive Data Ready 

	return(U0RBR);						   // Get Receice Data & Return 

}  

 
 

//--------------------------------------------------------------------------- 

void sendstring(unsigned char *str)			 //Sends a string of data through UART1 

{ 

 unsigned int i=0; 

 while(str[i]!='\0') 

 { 

   transmituart0(str[i]); 

   i++; 

 } 

} 

 
