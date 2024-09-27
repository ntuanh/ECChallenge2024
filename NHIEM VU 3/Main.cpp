
#include <Leanbot.h>                    // sử dụng thư viện Leanbot

const int vL = 1000;
const int vR = 1000;
const int time = 100;

void setup() {
  Leanbot.begin();                   
}
void forward() // hàm đi thẳng
{
  LbMotion.runLR(vL, vR);
}
void backward() // hàm lùi
{
  LbMotion.runLR(-vL, -vR);
}
void turnleft() // hàm quay trái 90 độ 
{
  LbMotion.runLR(-vL, vR);      
  LbMotion.waitRotationDeg(90);
}
void turnright() // hàm quay phải 90 độ
{
  LbMotion.runLR(vL, -vR);      
  LbMotion.waitRotationDeg();
}

void reverse() // hàm quay ngược 180 độ sang phải
{
  LbMotion.runLR(vL, -vR);      
  LbMotion.waitRotationDeg(180);
}


void twoto4()
{
  while(true)
  {
    int line = LbIRLine.read();
    if(line == 0b0110)
    {
      forward();
    }
    if(line == 0b1111)
    {
      LbMotion.stopAndWait();
      break;
    }
  }
}
void rescuepoint1()
{
  reverse();
  while(true)
  {
    int line = LbIRLine.read();
    if(line == 0b0110 || line == 0b1111)
    {
      forward();
    }
    if(LbIRLine.isBlackDetected())
    {
      LbMotion.stopAndWait();
      break;
    }
  }
  reverse();
}
void checkroom()
{
  twoto4();
  forward();
  LbMotion.waitDistanceMm(20);
  LbMotion.runLR(vL, -vR);
  LbMotion.waitRotationDeg(25);
  int d = Leanbot.pingMm();
  if(d < 200)
  {
    LbMotion.runLR(-vL, vR);
    LbMotion.waitRotationDeg(25);
    twoto4();
    forward();
    LbMotion.waitDistanceMm(25);
    turnright();
    forward();
    LbMotion.waitDistanceMm(50);
    LbGripper.moveToLR(90, 90, 2000); // kẹp 90 độ trong 2s
    // for(int i = 1; i <= 6; i++)
    // {
    //   LbGripper.moveTo(15);
    //   LbDelay(time);
    // }
    reverse();
    twoto4();
    forward();
    LbMotion.waitDistanceMm(25);
    turnleft();
  }
  if(d > 200)
  {
    LbMotion.runLR(-vL, vR);
    LbMotion.waitRotationDeg(25);
    twoto4();
    forward();
    LbMotion.waitDistanceMm(25);
    turnleft();
    forward();
    LbMotion.waitDistanceMm(50);
    LbGripper.moveToLR(90, 90, 2000); // kẹp 90 độ trong 2s
    // for(int i = 1; i <= 6; i++)
    // {
    //   LbGripper.moveTo(15);
    //   LbDelay(time);
    // }
    reverse();
    twoto4();
    forward();
    LbMotion.waitDistanceMm(25);
    turnright();
  }
}
void rescuepoint2()
{
  while(true)
  {
    int line = LbIRLine.read();
    if(line == 0b0110 || line == 0b1111)
    {
      forward();
    }
    if(LbIRLine.isBlackDetected())
    {
      LbMotion.stopAndWait();
      break;
    }
  }
  LbGripper.open();
  reverse();
}
void safe()
{
  twoto4();
  while(LbIRLine.isBlackDetected())
  {
  int line = LbIRLine.read();
  if(line == 0b0110)
  {
    forward();
  }
  if(line == 0b0000)
  {
    turnright();
    LbDelay(100);
    forward();
  }
  if(line == 0b1111)
  {
    LbMotion.stopAndWait();
    LbDelay(100);
    LbMotion.runLR(vL, vR);
    LbMotion.waitDistanceMm(25);
    turnright();
    LbDelay(100);
  }
  }
  LbDelay(100);
  forward();
  LbMotion.waitDistanceMm(60);
  turnleft();
}
