
#include <Leanbot.h>                    // sử dụng thư viện Leanbot
#define vL 400
#define vR 400
#define time 1000
void setup() {
  Leanbot.begin();                      // khởi động leanbot
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
  LbMotion.waitRotationDeg(90);
}
void reverse() // hàm quay ngược 180 độ
{
  LbMotion.runLR(vL, -vR);      
  LbMotion.waitRotationDeg(180);
}
void back()                             // Hàm lùi Leanbot đến ngã 4 2 Room
{
  backward();
  LbMotion.waitDistanceMm(65);
}
void checkRoom()                        // Hàm kiểm tra Room A hoặc Room B có người không
{
  turnright();
  LbDelay(100);         
  int distanceMm = Leanbot.pingMm();
  int limit = 30;
  int offset = 10;
  if(distanceMm <= 100)
  {
    distanceMm = Leanbot.pingMm();
    if (distanceMm > (limit + offset)){
      forward();
      LbDelay(100);
      LbMotion.waitDistanceMm(distanceMm - (litmit + offset), distanceMm - (litmit + offset));
      LbDelay(100);
      LbGripper.close();
      LbDelay(100);
      backward();
      LbMotion.waitDistanceMm(distanceMm - (litmit + offset), distanceMm - (litmit + offset));
      LbDelay(100);
      turnright(); 
    }
  }
  else if(distanceMm > 100)
  {
    reverse();
    LbDelay(100);
    distanceMm = Leanbot.pingMm();
    if (distanceMm > (limit + offset)){
      forward();
      LbDelay(100);
      LbMotion.waitDistanceMm(distanceMm - (litmit + offset), distanceMm - (litmit + offset));
      LbDelay(100);
      LbGripper.close();
      LbDelay(100);
      backward();
      LbMotion.waitDistanceMm(distanceMm - (litmit + offset), distanceMm - (litmit + offset));
      LbDelay(100);
      turnleft(); 
  }
  }
} 

void rescue() // Di chuyển đến Rescue
{
    while(LbIRLine.isBlackDetected())
    {
      forward();
    }
    LbDelay(100);
    LbMotion.stopAndWait();
    LbDelay(100);
    LbGripper.open();
    LbDelay(100);
    reverse();
}
void safe()
{
  int line = LbIRLine.read();
  int cnt = 0;
  while(LbIRLine.isBlackDetected())
  {
  if(line == 0b0110)
  {
    forward();
  }
  if (line == 0b1111 && cnt == 0)
  {
    cnt = 1;
    LbMotion.stopAndWait();
    LbDelay(100);
    turnright();
    LbDelay(100);
  }
  if(line == 0b0000)
  {
    turnright();
    LbDelay(100);
    forward();
  }
  if(line == 0b1111 && cnt == 1)
  {
    LbMotion.stopAndWait();
    LbDelay(100);
    turnleft();
    LbDelay(100);
  }
  }
  LbDelay(100);
  forward();
  LbMotion.waitDistanceMm(60);
  turnleft();
}
