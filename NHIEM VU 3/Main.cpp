
#include <Leanbot.h>                    // sử dụng thư viện Leanbot

const int time_delay = 100;
const int threshold = 50;

void setup() {
    Leanbot.begin();
    Serial.begin(9600);
}
/***************************************************************************************************************
The common part
-> TurnRight and Turn Left  ( int speed , int angle)
-> FollowLine ( int speed ) ( 2 option )
-> Grip ( int unit_angle ) ( 1 option)
***************************************************************************************************************/
void TurnRight(int speed, int angle)
{
    LbMotion.runLR(speep, -1 * speed);
    LbMotion.waitRotationDeg(angle);
    LbMotion.stopAndWait();
    LbDelay(time_delay);
}

void TurnLeft(int speed, int angle)
{
    LbMotion.runLR(-1 * speep, speed);
    LbMotion.waitRotationDeg(angle);
    LbMotion.stopAndWait();
    LbDelay(time_delay);
}

void Forward(int speed, int distance)
{
    LbMotion.runLR(speed, speed);
    LbMotion.waitDistanceMm(distance);
    LbMotion.stopAndWait();
    LbDelay(time_delay);
}
void Forward1(int speed)
{
    LbMotion.runLR(speed, speed);
    LbDelay(time_delay);
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

// Grip :
void Grip(int time_angle)
{
    for (int i = 0; i <= 90; i += 10) {
        LbGripper.moveToLR(i, i, time_angle);
    }
    LbMotion.stopAndWait();
    LbDelay(time_delay);
}

void EscapeHose() {                 // Push the escape hose to the rescue point
    TurnRight(500, 180);
    while (isBlackDetected())
    {
        Forward1(500);
    }
    LbMotion.stopAndWait();
    LbDelay(time_delay);
    TurnRight(500, 180);
}

void CheckHuman()                       // Check human and pick up 
{
    FollowLine();
    Foward(500, 20);
    TurnRight(500, 25);
    int distance = Leanbot.pingMm();
    if (distance < 200)               // Occupied
    {
        TurnLeft(500, 25);
        FollowLine();
        Forward(500, 45);
        TurnRight(500, 90);
        Forward(500, 50);
        Grip(500);
        TurnRight(500, 180);
        FollowLine();
        Forward(500, 45);
        TurnLeft(500, 90);
    }
    else if (distance > 200)           // Unoccupied
    {
        TurnLeft(500, 25);
        FollowLine();
        Forward(500, 45);
        TurnLeft(500, 90);
        Forward(500, 50);
        Grip(500);
        TurnRight(500, 180);
        FollowLine();
        Forward(500, 45);
        TurnRight(500, 90);
    }
}

void HumanToRescuePoint()
{
    while (isBlackDetected())
    {
        Forward1();
    }
    LbMotion.stopAndWait();
    LbDelay(time_delay);
    LbGripper.open();
    LbDelay(time_delay);
    TurnRight(500, 180);
}

void Safe()
{
    FollowLine();
    Forward(500, 45);
    TurnRight();
    FollowLine();
    TurnLeft();
    Forward(500, 60);
    TurnLeft(500, 90);
}
