#include<Leanbot.h>

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
    for (int i = 0; i <= 90; i += 10) {
        LbGripper.moveToLR(i, i, time_angle);
    }
    LbMotion.stopAndWait();
    LbDelay(time_delay);
}

// Grip break 
void GripBreak(int time_angle)
{
    for (int i = 90; i >= 0; i -= 10) {
        LbGripper.moveToLR(i, i, time_angle);
    }
    LbMotion.stopAndWait();
    LbDelay(time_delay);
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
    int disOutCO2 = 100;                            // mm
    int disCO22 = 70;
    int speedup = 1500;                             // mm 
    int speedfl = 500;
    int speedrun = 500;
    int speedturn = 500;
    int distanceADD = 40;
    int distanceADD2 = 50;
    int distanceLIMIT = 850;
    int distanceFORWARD2 = 200;
    int distanceFORWARD1 = 130;

    int time_angle = 500;

public:
    void Processing();
};

void NhiemVu2::Processing()
{
    // finish nv1
    LbGripper.close();

    // Mode 1 
    while (LbTouch.read(TB2A) == 0)LbDelay(10);
    Forward(speedrun, disCO22);
    LbMotion.stopAndWait();

    // Mode 2 : 
    //while (LbTouch.read(TB2A) == 0)LbDelay(10);
    TurnLeft(speedturn, 90);
    Forward(speedrun, disOutCO2);

    // Mode 3 : go to junctio
    //while (LbTouch.read(TB2A) == 0)LbDelay(10);
    //while (!junction())
    //FollowLine(speedfl);
    Forward( speedrun , 100);
    LbMotion.stopAndWait();
    while (junction())
    Forward(speedrun, distanceADD);
    LbDelay(time_delay);

    //Mode 4  
    //while (LbTouch.read(TB2A) == 0)LbDelay(10);
    //long distance = LbMotion.getDistanceMm();
    TurnLeft(speedturn, 90);
    LbDelay(time_delay);
    while(LbMotion.getDistanceMm() < distanceLIMIT)
    FollowLine(speedfl);
    Forward(speedup, distanceFORWARD1);
    LbMotion.stopAndWait();
    LbDelay(time_delay);
    TurnRight( speedturn , 90) ;
    Forward(speedfl, distanceFORWARD2);
    while (!junction())FollowLine(speedfl);

    // Mode 5 
   // while (LbTouch.read(TB2A) == 0)LbDelay(10);
    while(junction()) {
    Forward(speedfl, distanceADD2);
    TurnLeft(speedturn, 90);
    break;
    }
    while (!whiteSpace())Forward(speedrun, 3);

    // Mode 6 
    //while (LbTouch.read(TB2A) == 0)LbDelay(10);
    GripBreak(time_angle);
    TurnLeft(speedturn, 180);
    LbDelay(time_delay);
}

void loop()
{
    NhiemVu2 nv2;
    nv2.Processing();
}
