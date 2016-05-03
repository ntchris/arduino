/* BLINKY.C - LED Flasher for the Keil MCBx51 Evaluation Board with 80C51 device*/
                  
#include <REGX51.H>

// When you have enabled the option Stop Program Execution with Serial
// Interrupt, the Monitor-51 uses the serial interrupt of the UART.
// It is therefore required to reserve the memory locations for the interrupt
// vector.  You can do this by adding one of the following code lines:

// char code reserve [3] _at_ 0x23;   // when using on-chip UART for communication
// char code reserve [3] _at_  0x3;   // when using off-chip UART for communication


unsigned int DigitMAX = 9999;
#define   MAX_DIGITS_COUNT    4 
unsigned int MaxDiv=1000;

unsigned char code DIG_CODE[ ]={0x40, //0
	                               0x79, //1
	                               0x24, //2
                                 0x30, //3
	                               0x19, //4
	                               0x12, //5
	                               0x2, //6
	                               0x78, //7
	                               0x0, //8
	                               0x10  //9
	
};

unsigned char g_displayBuffer[MAX_DIGITS_COUNT];

unsigned int g_counter=0;
  
	

void allDigitOff()
{
  P1_0=1;P1_1=1;P1_2=1;P1_3=1;
   
}
	
void activeDigitN(unsigned int position)
{
  
   switch(position)
  {
     case 0: P1_0=0; break;
     case 1: P1_1=0; break;
     case 2: P1_2=0; break;
     case 3: P1_3=0; break;
   }
}




void showDigitInPositionN(unsigned char position , unsigned char digit)
{
   allDigitOff(); //so that the wrong number won't show!
   P0 = DIG_CODE[digit];
   activeDigitN(position);
}



//maximum 9999
 void showDigits(unsigned int digit)
{   
    unsigned char skipZero=1;
    unsigned int Div=MaxDiv;
    unsigned char i=MAX_DIGITS_COUNT;
    unsigned int digitsLeftToShow=digit;
    if(digit>DigitMAX) return;
 
    if(digit==0)
    {
       showDigitInPositionN(1, digit);
       return;
    }
   
	  do
		{  
       unsigned char showOneDigit=digitsLeftToShow/Div;
       if(showOneDigit!=0)skipZero=0; //from now on , all zero should be displayed (because 1000 ,  all 000 should be displayed
      
       digitsLeftToShow=digitsLeftToShow%Div;
		   
       if((showOneDigit!=0) || !skipZero) 
       {
          showDigitInPositionN(i, showOneDigit);
         
          
			 }
       Div/=10;

       i--;
       
    }while(i>=1);

}



//maximum 9999
//this function only changes the buffer, interrupt function shows the buffer.
void setDisplayBuffer(unsigned int digit)
{   
    //static unsigned int previousDigit ;
    unsigned char skipZero=1;
    unsigned int Div=MaxDiv;
    unsigned char i=MAX_DIGITS_COUNT-1;
    unsigned int digitsLeftToShow=digit;
    //if(digit == previousDigit)
    {
       //same, skip
      // return;
    }
    //previousDigit = digit;
    if(digit>DigitMAX) return;
  
	  while(1)
		{  
       unsigned char showOneDigit=digitsLeftToShow/Div;
       //if(showOneDigit!=0)skipZero=0; //from now on , all zero should be displayed (because 1000 ,  all 000 should be displayed
      
       digitsLeftToShow=digitsLeftToShow%Div;
		   g_displayBuffer[i]=showOneDigit;
       
       Div/=10;
       if(i==0)break;
       i--;
       
    } 

}

void testloopAllDigits(void)
{
   unsigned int i;                     /* Delay var */
 
   int arraySize = sizeof(DIG_CODE);
   while (1) 
   {               /* Loop forever */
     for (i=0; i< arraySize; i++)  
	   {   
        P0 = DIG_CODE[i];                         
      }
   }
}


void testP1(unsigned char value)
{
   P1=value;
}
  

void testall0()
{
  P0=0;
  P1=0;
  P2=0;
  P3=0;

}
  
 
void counterIncrease(unsigned int increase)
{
   g_counter+=increase;
   if(g_counter >=DigitMAX) 
   {
      g_counter=DigitMAX;
      ; //keep the MAX number
   }
   setDisplayBuffer(g_counter);
   return;
}
  
void interruptInitForDisplayRefreshTimer()
{
   PX0=1;  //external interrupt is high priority
   PT0=0;  //timer is low priority
   // T1 = 0b1001  external    T0: 0b 0001  internal timer
   TMOD = 0x50; //0b1001---0001  
   //init T0 timer  
   TH0 =  0x65;
   TL0 =  0x00;
   TR0 = 1;    //start timer0
   ET0 = 1;            // Enable timer0 interrupt
   PT0=0;  // this t0 timer has lower priority
    
   // init T1 ext interrupt
   TH1 =  0xFF;
   TL1 =  0xFF;
   
   EA = 1;             // Enable all interrupts
   TR1 = 1;    //start timer1
   EX0=1;
   ET1 = 1;            // Enable timer1 interrupt
   PT1=1;  // higher priority for external interrupt
  
}

 
void externalInterrupt3(void) interrupt 3  //Timer 1 Interrupt
{
     
   TH1 =  0xFF;
   TL1 =  0xFF; 
   counterIncrease(1 );
   setDisplayBuffer(g_counter);
   
}

     
void timer0_refreshDisplay_interrupt(void) interrupt 1  //Timer 0 Interrupt
{   
   static unsigned char digitIndex=0;
     
    ET0=0;
    TH0 =  0x65;
    TL0 =  0x00;
   
   showDigitInPositionN(digitIndex, g_displayBuffer[digitIndex]);
   digitIndex++;
   if(digitIndex>=MAX_DIGITS_COUNT) 
   { 
      digitIndex=0;
   }
   ET0=1;

 }
 

void resetDisplayBufferToZero()
{
  g_displayBuffer[0]=0;
  g_displayBuffer[1]=0;
  g_displayBuffer[2]=0;
  g_displayBuffer[3]=0;
}



void main ()
{
  
	int arraySize = sizeof(DIG_CODE);
	resetDisplayBufferToZero();
  interruptInitForDisplayRefreshTimer();
  g_counter = 0;
  
  
   while(1)
  {
      unsigned int i=0;
      
      for(  i=0;i<5000;i++)
      {
         int j=0;
         for( j=0;j<4;j++)
         {
           ;
         }
      }
  } 
}
