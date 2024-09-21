#include<Define.h>

unsigned int time_delay  ; // LbDelay( time_delay ) 

int speed_right  ;
int speed_left  ; //LbMotion.runLR( speed_right , speed_left )
long distanceStep ; // LbMotion.waitDistance(distanceStep)
int distanceMm ; // LbMotion.waitDistanceMm(distanceMm)
long rotationStep ; //LbMotion.waitRotation(rotationStep)
int rotationDeg ; //LbMotion.waitRotationDeg(rotationDeg)
long distance = LbMotion.getDistance()
long distance = LbMotion.getDistanceMm()
long rotation = LbMotion.getRotation()
long rotationDegree = LbMotion.getRotationDeg()


