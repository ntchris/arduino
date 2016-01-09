/*

 */


const int MinDigitPort = 2;
const int MaxDigitPort = 13;

// the setup function runs once when you press reset or power the board
void setup() {
    
    int i ;
   for(i= MinDigitPort; i <= MaxDigitPort; i++)
    { 
        pinMode(i, OUTPUT);
    }
    
}

 
 

// the loop function runs over and over again forever
void loop() {
   static   int i= MinDigitPort;
    if(i<MaxDigitPort)t
    {
       i++;
    }else
    {
       i = MinDigitPort;
    }
    digitalWrite(i, HIGH); 
    
    delay(500);  
    digitalWrite(i, LOW); 

    
}