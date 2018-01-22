/* BLINKY.C - LED Flasher for the Keil MCBx51 Evaluation Board with 80C51 device*/
                  
//#include <REGX51.H>

#include "htc.h"
// When you have enabled the option Stop Program Execution with Serial
// Interrupt, the Monitor-51 uses the serial interrupt of the UART.
// It is therefore required to reserve the memory locations for the interrupt
// vector.  You can do this by adding one of the following code lines:

// char code reserve [3] _at_ 0x23;   // when using on-chip UART for communication
// char code reserve [3] _at_  0x3;   // when using off-chip UART for communication

 
 unsigned const int MAX = 9999;
 unsigned char code DIG_CODE[ ]={  0x40, //0
	                               0x79, //1
	                               0x24, //2
                                   0x30, //3
	                               0x19,  //4
	                               0x12,   //5
	                               0x2, //6
	                                0x78, //7
	                                0x0, //8
	                                0x10   //9
	
};
	                               
void activeDigitN(int n)
{
   switch(n)
  {
     case 1: P10=0;break;
     case 2: P11=0;break;
     case 3: P12=0;break;
     case 4: P13=0;break;
   }
}

void sleep100ms(void)
{
   unsigned int i;                                   /* only to delay for LED flashes */
	 for (i = 0; i < 7000; i++)  
	{   
	;
     }
}
	
void sleep1sec (void)  {                   /* wait function */
  unsigned int i;                                   /* only to delay for LED flashes */
	for (i = 0; i < 10; i++)  
	{   
	   sleep100ms();
    }
}




void showDigitInPositionN(unsigned char position , unsigned char digit)
{
    //if(n<=0) { return };

    unsigned char newValue;
    newValue = (0B1000 >> (position-1));
    P1=newValue;
     P1=8;
    P0 = DIG_CODE[digit];
//     printf("");
  //  printf( "%b %d",n , digit);
}



//maximum 9999
void showDigit(int digit)
{
    unsigned char num=0;
    int tenMod=1;
    int i;
    for(i=1;i<=4;i++)
    {
        num=digit/tenMod;
       
        showDigitInPositionN(num, i);
        tenMod*=10; 
     }
    
}


void loopAllDigits(void)
{
   unsigned int i;                     /* Delay var */
 
   int arraySize = sizeof(DIG_CODE);
   while (1) 
   {               /* Loop forever */
     for (i=0; i< arraySize; i++)  
	 {   
        P0 = DIG_CODE[i];                         
        sleep100ms();
     }
   }
}


void main (void)  {

   int i=0;
   //P1=0xff;
   //P0=0xff;
   /*for (i=0;i<=MAX;i++)
   {

       showDigit(i);
 
   }*/


    while(1)
    {
        //showDigit(1234);
        showDigitInPositionN(1,8);
    } 
  
}
