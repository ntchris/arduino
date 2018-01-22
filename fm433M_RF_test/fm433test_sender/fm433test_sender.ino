

int data_out = 2;
int internal_LED = 13;



void setup() {
  // put your setup code here, to run once:
  pinMode(data_out, OUTPUT); //设置13脚为数据发送口
  pinMode( internal_LED, OUTPUT); //

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(data_out, HIGH); //发送信号为方波周期1s
  digitalWrite(internal_LED, HIGH);
  delay(100);

  digitalWrite(data_out, LOW);
  digitalWrite(internal_LED, LOW);
  delay(100);
}
