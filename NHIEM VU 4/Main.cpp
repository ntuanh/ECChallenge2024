#include<Leanbot.h>

const int time_delay = 500;
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
-> detect object
***************************************************************************************************************/
void TurnRight(int speed, int angle)
{
    LbMotion.runLR(speed, -1 * speed);                     // let Leanbot rotate right
    LbMotion.waitRotationDeg(angle);
    LbMotion.stopAndWait();
    LbDelay(time_delay);
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
}

// Grip :
void Grip(int time_angle)
{
    for (int i = 0; i <= 90; i += 10)
        LbGripper.moveToLR(i, i, time_angle);
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
the first challenge ( NHIEMVU4 )
**************************************************************************************************************/

class NhiemVu4 {
private:
    int speedrun = 500;
    int speedadd = 150;
    int speedturn = 500;

    int dis_forward = 45;                           // mm 

    int dis_angle = 35;
    int dis_unit = 30;

    int angle_turn = 40;                             // degree 
public:
    void Processing();
};
/*
void NhiemVu4::Processing()
{
  // Mode 1 : Move forward and turn left , move forward continue with double distance
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  Forward( speedrun , dis_forward );
  TurnLeft( speedturn , 90 );
  Forward( speedrun , dis_unit * 2 );
  LbMotion.stopAndWait() ;
  LbDelay(time_delay);

  // Mode 2 : Turn a unit angle and move forward
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  TurnRight( speedturn , angle_turn );
  Forward( speedrun , dis_angle ) ;
  LbMotion.stopAndWait() ;
  LbDelay(time_delay);

  // Mode 3 : note Canva
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  TurnLeft ( speedturn , angle_turn * 2 );
  Forward( speedrun , dis_unit );
  LbMotion.stopAndWait() ;
  LbDelay(time_delay);

  // Mode 4 : after Mode 3
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  TurnRight ( speedturn , angle_turn * 2 );
  Forward( speedrun , dis_unit );
  LbMotion.stopAndWait() ;
  LbDelay(time_delay);

  // Mode 5 :
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  TurnLeft ( speedturn , angle_turn  );
  while ( !whiteSpace() )Forward( speedrun , 5 );
  Forward( speedrun , 50);
  LbMotion.stopAndWait() ;
  LbDelay(time_delay);

}
*/
/*************************************************************
Solution 2
**************************************************************/


void NhiemVu4::Processing()
{
    Grip(10);
    // Mode 1 : Move forward and turn left , move forward continue with double distance
    /*
    while (LbTouch.read(TB2A) == 0)LbDelay(10);
    Forward( speedrun , dis_forward );
    TurnLeft( speedturn , 85 );
    Forward( speedrun , dis_unit * 2 );
    LbMotion.stopAndWait() ;
    LbDelay(time_delay);
    */
    int cnt = 0;

    // Mode 2 : follow wall until detect white space
    while (LbTouch.read(TB2A) == 0)LbDelay(10);
    while (cnt < 3) {
        if (whiteSpace())cnt++;
        int val_left = LbIRArray.read(ir6L);
        int val_right = LbIRArray.read(ir7R);
        if (val_left < 120)LbMotion.runLR(speedrun + speedadd + 50, speedrun - 50);else {
            if (val_right < 600 && val_left > 190)LbMotion.runLR(speedrun, speedrun + speedadd);
            else LbMotion.runLR(speedrun, speedrun);
        }
    }
}

/*
void loop(){
  int value ;
  value = LbIRArray.read(ir6L);    // read the sensor value
  Serial.print("Value of IR6L: ");
  Serial.println(value);
  value = LbIRArray.read(ir7R);    // read the sensor value
  Serial.print("Value of IR7R: ");
  Serial.println(value);
  LbDelay(1000);
}*/

void loop() {
    NhiemVu4 nv4;
    nv4.Processing();
}