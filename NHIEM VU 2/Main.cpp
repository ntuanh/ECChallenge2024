

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
}

void loop(){
  byte IRvalue = LbIRLine.read(threshold);
  int count_mode = 1;
  if(firsttime && IRvalue == 0b0000) {    // Đến vạch trắng lần đầu 
    Function(count_mode);
    count_mode++;
    firsttime = false;
  }
  if(IRvalue == 0b1110 || IRvalue == 0b1111) {   // Kiểm tra ngã rẽ
    Function(count_mode);
    count_mode++;
  }
}

void Buzzer() {                             // Bật còi cảnh báo    
  const int freq[] = {1000, 1200, 1000, 1200, 1000};
  for(int i = 0; i < 5; i++) {
    Leanbot.tone(freq[i], 500);
    LbDelay(300);
  }
  LbDelay(400);
}

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
}

void mode_2(){                          // Mode 2 rẽ trái và dò line
  if(check_mode2) {
    TurnLeft();
    FollowLine();
    check_mode2 = false;
  }
}

void mode_3(){                          // Mode 3 rẽ trái và dò line
  if(check_mode3) {
    TurnLeft();
    FollowLine();
    check_mode3 = false;
  }
}

void mode_4(){                          // Mode 4 là đi thẳng dò line
  if(check_mode4) {
    FollowLine();
    check_mode4 = false;
  }
}
