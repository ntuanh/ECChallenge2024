#include<Leanbot.h>

<<<<<<< HEAD
const int time_delay = 1000;
const int threshold = 50;

void setup() {
    Leanbot.begin();
    Serial.begin(9600);
}
/***************************************************************************************************************
The common part
-> TurnRight and Turn Left  ( int speed , int angle)
-> Forward
-> FollowLine ( int speed ) ( 2 option )
-> Whitespace
-> Grip ( int time_angle ) ( 1 option)
-> GripBreak ( int time_angle )
-> Junction
***************************************************************************************************************/
void TurnRight(int speed, int angle)
{
    LbMotion.runLR(speed, -1 * speed);                     // let Leanbot rotate right
    LbMotion.waitRotationDeg(angle);
    LbMotion.stopAndWait();
    LbDelay(time_delay);
=======

#include <Leanbot.h>                        // use Leanbot library

unsigned int distance1 = 60;
unsigned int distance2 = 100;
const int time_delay = 1000;                           // Thời gian delay
const int threshold = 50;                             // Giá trị của cảm biến IR (bổ sung sau) 
bool firsttime = true;
bool check_mode1 = true;
bool check_mode2 = true;
bool check_mode3 = true;
bool check_mode4 = true;

void setup() {
  Leanbot.begin();                          // initialize Leanbot
  Serial.begin(9600);
>>>>>>> 608520c1135e2d2febd375a579d81870dfeb33dc
}

void TurnLeft(int speed, int angle)
{
    LbMotion.runLR(-1 * speed, speed);                     // let Leanbot rotate right
    LbMotion.waitRotationDeg(angle);
    LbMotion.stopAndWait();
    LbDelay(time_delay);
}

void Forward(int speed, int distance)                    // 
{
    LbMotion.runLR(speed, speed);                     // let Leanbot rotate right
    LbMotion.waitDistanceMm(distance);
    LbMotion.stopAndWait();
    //LbDelay(time_delay);
}
<<<<<<< HEAD
// Follow Line : Solution 1  
/*
void FollowLine( int speed )
{
    int vL = speed ;
    int vR = speed ;
    byte line = LbIRLine.read(threshold);
    switch (line) {
    case 0b0100:
    case 0b1110:
        LbMotion.runLR(0, +vR);
        break;
    case 0b1100:
    case 0b1000:
        LbMotion.runLR(-vL, +vR);
        break;
    case 0b0010:
    case 0b0111:
        LbMotion.runLR(+vL, 0);
        break;
    case 0b0011:
    case 0b0001:
        LbMotion.runLR(+vL, -vR);
        break;
    default:
        LbMotion.runLR(+vL, +vR);
    }
}
*/
// Follow Line : Solution 2 
void FollowLine(int speed)
{
    // div level speed 
    int speed_0 = 0;
    //int speed_1 = 0.2 * speed ;
    int speed_2 = 0.4 * speed;
    int speed_3 = 0.6 * speed;
    //int speed_4 = 0.8 * speed ;

    byte line = LbIRLine.read(threshold);                   // Read the value of 4 bar sensors with a threshold of 50
    LbIRLine.println(line);                                 // transfer the results to the computer

    switch (line) {                                         // check the location of Leanbot
    case 0b0000:
    case 0b0110:
        LbMotion.runLR(speed, speed);
        break;


    case 0b0010:
        LbMotion.runLR(speed, speed_3);
        break;

    case 0b0011:
        LbMotion.runLR(speed, speed_2);
        break;

    case 0b0001:
        LbMotion.runLR(speed, speed_0);
        break;

    case 0b0100:
        LbMotion.runLR(speed_3, speed);
        break;

    case 0b1100:
        LbMotion.runLR(speed_2, speed);
        break;

    case 0b1000:
        LbMotion.runLR(speed_0, speed);
        break;


    case 0b1111:
        LbMotion.runLR(speed_0, speed_0);
        break;
    }
}

//Check white space 
bool whiteSpace()
{
    byte line = LbIRLine.read(threshold);
    if (line == 0b0000)return true;else return false;
=======

void EmergencySignal() {                    // Bật đèn nhấp nháy theo nhịp
  byte LedArray = BITMAP(ledA, ledF, ledE, ledD, ledC, ledB);
  LbRGB[ledO] = CRGB::Yellow;               // Đèn O hiện màu vàng
  LbRGB.show();
  for(int i = 0; i < 10; i++) {
    LbRGB.fillColor(CRGB::Red, LedArray);  // Các đèn còn lại bật thành hình tròn màu đỏ
    LbDelay(500);
    LbRGB.show();
    LbRGB.clear();
  }
}

void TurnRight(int speed, int angle) {
  LbMotion.runLR(speed, -1 * speed);                     // let Leanbot rotate right
  LbMotion.waitRotationDeg(angle);
  LbMotion.stopAndWait();
  LbDelay(time_delay);
}

void TurnLeft(int speed, int angle) {
  LbMotion.runLR(-1 * speed, speed);                     // let Leanbot rotate right
  LbMotion.waitRotationDeg(angle);
  LbMotion.stopAndWait();
  LbDelay(time_delay);
}

void Forward(int speed, int distance) {                 
  LbMotion.runLR(speed, speed);                     // let Leanbot rotate go ahead
  LbMotion.waitDistanceMm(distance);
  LbMotion.stopAndWait();
  LbDelay(time_delay);
}

void CO2() {                             // Thoát khỏi ô CO2
  Forward();
  LbMotion.waitDistanceMm(distance1);
  Buzzer();
  LbDelay(time_delay);
  TurnLeft();
  Forward();
  LbMotion.waitDistanceMm(distance2);
  Buzzer();
  LbDelay(time_delay);
}

void FollowLine(int speed) {
  // div level speed 
  int speed_0 = 0;
  int speed_1 = 0.2 * speed;
  int speed_2 = 0.4 * speed;
  int speed_3 = 0.6 * speed;
  int speed_4 = 0.8 * speed;

  byte line = LbIRLine.read(threshold);                   // Read the value of 4 bar sensors with a threshold of 50
  LbIRLine.println(line);                                 // transfer the results to the computer

  switch (line) {                                         // check the location of Leanbot
  case 0b0000:
  case 0b0110:
      LbMotion.runLR(speed, speed);
      break;


  case 0b0010:
      LbMotion.runLR(speed, speed_3);
      break;

  case 0b0011:
      LbMotion.runLR(speed, speed_2);
      break;

  case 0b0001:
      LbMotion.runLR(speed, speed_0);
      break;


  case 0b0100:
      LbMotion.runLR(speed_3, speed);
      break;

  case 0b1100:
      LbMotion.runLR(speed_2, speed);
      break;

  case 0b1000:
      LbMotion.runLR(speed_0, speed);
      break;


  case 0b1111:
      LbMotion.runLR(speed_0, speed_0);
      break;
    }
}

void Grip_FireZone(int time_angle) {
    for (int i = 90; i >= 0; i -= 10) {
      LbGripper.moveToLR(unit_angle, unit_angle, time_angle);
    }
    LbMotion.stopAndWait();
    LbDelay(time_delay);
}

class NhiemVu2 {
private:
  int speed = 600;

  int time_angle = 500;

  unsigned int distance1 = 60;
  unsigned int distance2 = 100;
public:
  void TurnRight();
  void TurnLeft();
  void Forward();
  void FollowLine();
  void CO2();
  void EmergencySignal();
}

























































void Function(int n) {                   // Thực hiện nhiệm vụ 2
  switch(n) {
    case 1:
      mode_1();
      break; 
    case 2:
      mode_2();
      break;
    case 3:
      mode_3();
      break; 
    case 4:
      mode_4();
      break;
  }
}   

void mode_1(){                          // Mode 1 là đi thẳng dò line
  if(check_mode1) {
    FollowLine();
    check_mode1 = false;
  }
>>>>>>> 608520c1135e2d2febd375a579d81870dfeb33dc
}

// Grip :
void Grip(int time_angle)
{
    for (int i = 0; i <= 90; i += 10) {
        LbGripper.moveToLR(i, i, time_angle);
    }
    LbMotion.stopAndWait();
    LbDelay(time_delay);
}

// Grip break 
void GripBreak(int time_angle)
{
    for (int i = 90; i >= 0; i -= 10)
        LbGripper.moveToLR(i, i, time_angle);
}

bool junction()                                     // check junction road 
{
    byte line = LbIRLine.read(threshold);
    if (line == 0b1111)return true;
    if (line == 0b1110)return true;
    if (line == 0b0111)return true;
    return false;
}

/***************************************************************************************************************
the first challenge ( NHIEMVU2 )
**************************************************************************************************************/

class NhiemVu2
{
private:
    int disOutCO2 = 40;                            // mm
    int disCO22 = 40;
    int speedup = 1000;                             // mm 
    int speedfl = 500;
    int speedrun = 500;
    int speedturn = 500;

    int time_angle = 500;

public:
    void Processing();
};

void NhiemVu2::Processing()
{
    // Mode 1 
    while (LbTouch.read(TB2A) == 0)LbDelay(10);
    Forward(speedrun, disCO22);
    LbMotion.stopAndWait();

    // Mode 2
    while (LbTouch.read(TB2A) == 0)LbDelay(10);
    TurnLeft(speedturn, 90);
    Forward(speedrun, disOutCO2);

    // Mode 3 
    while (LbTouch.read(TB2A) == 0)LbDelay(10);
    while (!junction())FollowLine(speedfl);
    LbMotion.stopAndWait();
    LbDelay(time_delay);

    //Mode 4  
    while (LbTouch.read(TB2A) == 0)LbDelay(10);
    TurnLeft(speedturn, 90);
    while (!junction())FollowLine(speedup);

    // Mode 5 
    while (LbTouch.read(TB2A) == 0)LbDelay(10);
    TurnLeft(speedturn, 90);
    while (!whiteSpace())Forward(speedrun, 5);

    // Mode 6 
    while (LbTouch.read(TB2A) == 0)LbDelay(10);
    GripBreak(time_angle);
    TurnLeft(speedturn, 180);
    LbDelay(time_delay);
}

void loop()
{
    NhiemVu2 nv2;
    nv2.Processing();
}