#include <Leanbot.h>                    // use Leanbot library

void setup()
{
  Serial.begin(9600);
  Leanbot.begin();
}


void turnLeft( int degree)                            // 
{
  LbMotion.runLR(-1*speed, speed );                   // let Leanbot rotate left
  LbMotion.waitRotationDeg( degree );                 // wait for Leanbot to rotate approximately 90°
}

void turnRight( int degree) 
{
  int speed = 500 ;
  LbMotion.runLR(speed , -1 * speed );               // let Leanbot rotate right
  LbMotion.waitRotationDeg(degree));                 // wait for Leanbot to rotate approximately 90°
}

void Forward( int distance_aim , int speed )         // go forward
{
  LbMotion.runLR( speed , speed );
  LbMotion.delayDistanceMm( distance_aim );
  LbMotion.stopAndWait();
}

void runFinish( int speed_add , int distance_aim)    //  
{
  int speed_add = 100 ;                   
  int time_forward = 200 ;
  int degree = 15 ;
  
  Forward(distance_aim , speed );
  turnRight( degree );
  
  Forward(distance_aim , speed) ;
  turnLeft( degree );
  turnLeft( degree );

  Forward ( distance_aim , speed ) ;
  turnRight( degree );
  turnRight(degree );

  Forward(distance_aim , speed) ;
  turnLeft (degree );

  Forward( distance_aim , speed ) ;

}

void NHIEMVU_4() {
  runFinish();
}

void loop(){
  NHIEMVU_4();
}