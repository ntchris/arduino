int datain = 12;
int LED = 13;


void setup() {
  // put your setup code here, to run once:
  pinMode(datain, INPUT); //设置12脚为数据接收（datain）脚

  pinMode(LED, OUTPUT); //设置13脚为LED显示管脚
}

void loop() {
  // put your main code here, to run repeatedly:
   int value = digitalRead(datain);
   digitalWrite(LED,value );//用12脚接收到的无线电平信号 //控制13脚上的LED 
}
