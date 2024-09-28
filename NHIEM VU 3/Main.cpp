#include<Leanbot.h>

const int time_delay = 1000 ;
const int threshold =  50 ;

void setup(){
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

void Forward ( int speed , int distance )                    // 
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
  for ( int i = 0 ; i <= 90  ; i += 10 )
    LbGripper.moveToLR(i, i , time_angle);
  LbMotion.stopAndWait() ;
  LbDelay(time_delay);
}

// Grip break 
void GripBreak( int time_angle)
{
  for (int i = 90; i >= 0; i -= 10) 
  LbGripper.moveToLR( i , i , time_angle);
}

bool junction()                                     // check junction road 
{
  byte line = LbIRLine.read(threshold); 
  if ( line == 0b1111 )return true ;
  if ( line == 0b1110 )return true ;
  if ( line == 0b0111 )return true ;
  return false ;
}

/***************************************************************************************************************
the first challenge ( NHIEMVU2 )
**************************************************************************************************************/

class NhiemVu3{
  private :
    int speedrun  = 500 ;                   // mm 
    int speedfl = 500 ;                     // mm 
    int speedturn = 500 ;
    int limit_angle =  60  ;
    int time_angle = 500 ;

    int distance_back = 50 ;
    unsigned int distance_human = 60 ;

    int distance_add1 = 15 ;
    int distance_add2 = 30 ;

  public :
    void Processing();
};

void NhiemVu3::Processing() 
{
  // Mode 1  
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  while ( !whiteSpace() ){
    if(!junction() )Forward(speedrun , 5);
    else {FollowLine(speedfl) ;}
  }
  LbMotion.stopAndWait();

  // Mode 2 
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  Forward(-1 * speedrun , distance_back);
  TurnLeft( speedturn , 180 );
  while ( !junction() ){
    FollowLine(speedfl);
  }

  bool check_human_right = false ;
  // Mode 3 : turn right 
  int angle = 0 ;
  while ( angle < limit_angle ){
    TurnRight(speedturn , 5 );
    angle += 5 ;
    if ( Leanbot.pingMm() <= distance_human){
      check_human_right = true ;
      TurnLeft(speedturn , angle) ;
      break ;
    }
  }

  // Mode 4 : safe :
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  while(!junction())FollowLine(speedfl) ;
  Forward(speedrun , distance_add1);
  if ( check_human_right){
    TurnRight(speedturn, 90);
  }else {
    TurnLeft(speedturn , 90 );
  }
  Forward(speedrun , distance_add2);

  // Mode 5 : Grip 
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  Grip(time_angle) ;
  TurnLeft(speedturn , 180) ;
  while (!junction())FollowLine(speedfl);

  //Mode 6 : 
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  if ( check_human_right ){
    TurnLeft(speedrun , 90);
  }else TurnRight( speedrun , 90);

  //Mode 7 :
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  while(!whiteSpace()){
    if(!junction())FollowLine(speedfl);else Forward(speedrun , 5);
  }

  // Mode 8: break grip 
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  GripBreak( time_angle) ;
  while(!junction() )FollowLine(speedfl);
  TurnRight( speedturn , 90);

  // Mode 9 : go to safe 
  while (LbTouch.read(TB2A) == 0)LbDelay(10);
  while ( !whiteSpace() )FollowLine(speedfl);
}

void loop(){
  NhiemVu3 nv3 ;
  nv3.Processing();
}