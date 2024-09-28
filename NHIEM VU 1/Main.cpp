#include<Leanbot.h>

const int time_delay = 100 ;
const int threshold =  50 ;

void setup(){
  Leanbot.begin();
  Serial.begin(9600);
}
/***************************************************************************************************************
The common part 
-> TurnRight and Turn Left  ( int speed , int angle)
-> FollowLine ( int speed ) ( 2 option )
-> Grip ( int unit_angle ) ( 1 option)
***************************************************************************************************************/
void TurnRight( int speed , int angle )
{
  LbMotion.runLR(speed , -1 * speed );                     // let Leanbot rotate right
  LbMotion.waitRotationDeg(angle);  
  LbMotion.stopAndWait() ;
  LbDelay(time_delay);
}

void TurnLeft( int speed , int angle )
{
  LbMotion.runLR(-1 * speed , speed );                     // let Leanbot rotate right
  LbMotion.waitRotationDeg(angle);  
  LbMotion.stopAndWait() ;
  LbDelay(time_delay);
}

void Foward ( int speed , int distance )                    // 
{
  LbMotion.runLR( speed , speed );                     // let Leanbot rotate right
  LbMotion.waitDistanceMm(distance)  ;
  LbMotion.stopAndWait() ;
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
void FollowLine ( int speed  )
{
  // div level speed 
  int speed_0 = 0  ;
  //int speed_1 = 0.2 * speed ;
  int speed_2 = 0.4 * speed ;
  int speed_3 = 0.6 * speed ;
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
  if ( line == 0b0000 )return true ;else return false ;
}

// Grip :
void Grip( int time_angle )
{
  for ( int i = 0 ; i <= 90  ; i += 10 ){
    LbGripper.moveToLR(i, i , time_angle);
  }
  LbMotion.stopAndWait() ;
  LbDelay(time_delay);
}
/***************************************************************************************************************
the first challenge ( NHIEMVU1 )
**************************************************************************************************************/
class NhiemVu1{
private :
  int vL = 500 ;
  int vR = 500 ;

  int speed_turn  = 500 ;             //speed use for turn

  int speed_fl = 500 ;                //speed use for follow line

  int speedrun = 800 ;

  int time_angle_unit = 500 ;              // ms

  int angle_1 = 45 ;
  int angle_2 = 45 ;

  unsigned int dis_object1 = 50;      // mm 
  unsigned int dis_object2 = 50;      // mm

  int dis_forward1 = 50 ;
  int dis_forward2 = 50 ;

  int dis2CO21 = 40 ;
  int dis2CO22 = 20 ;


  int cornerL = LbGripper.readL();
  int cornerR = LbGripper.readR();
public :
  void TurnRight() ;
  void TurnLeft() ;
  void FollowLine () ;
  void Grip() ;
  void Processing() ;
};

void NhiemVu1::TurnRight()              // This function uses the TurnRight function from common part 
{
  ::TurnRight(speedrun , angle_1 ); 
}

void NhiemVu1::TurnLeft ()              // the same TurnRight
{
  ::TurnLeft(speedrun , angle_2 );
}

void NhiemVu1::FollowLine()             // same 
{
  ::FollowLine(speed_fl);
}

void NhiemVu1::Grip() 
{
  ::Grip(time_angle_unit);
}

void NhiemVu1::Processing()
{
  // Mode 1 
  while ( LbTouch.read(TB1A) == 0)LbDelay(10); 
  while (!whiteSpace() ) ::Foward( speedrun , 5) ;
  while ( Leanbot.pingMm() < dis_object1 )FollowLine() ;
  LbMotion.stopAndWait();

  // Mode 2 
  while ( LbTouch.read(TB1A) == 0)LbDelay(10); 
  ::TurnRight( speed_turn  , angle_1 );
  ::Foward ( speedrun , dis_forward1 );
  while ( Leanbot.pingMm() < dis_object2 )::Foward(speedrun , 5 );
  LbMotion.stopAndWait();

  // Mode 3 
  while ( LbTouch.read(TB1A) == 0)LbDelay(10); 
  ::TurnLeft( speed_turn , angle_2 );
  ::Foward( speedrun , dis_forward2 );
  LbMotion.stopAndWait();

  //Mode 4 
  while ( LbTouch.read(TB1A) == 0)LbDelay(10); 
  while ( whiteSpace() ) ::Foward(speedrun , 5 );
  while ( !whiteSpace() ) FollowLine();
  LbMotion.stopAndWait();

  //Mode 5 
  ::Foward( speedrun , dis2CO21 );
  LbDelay(time_delay);
  Grip() ;
  // end duty 

}

void loop() 
{
  NhiemVu1 nv1 ;
  nv1.Processing();
}