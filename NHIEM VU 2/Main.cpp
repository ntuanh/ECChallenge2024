

#include <Leanbot.h>                        // use Leanbot library
#define vL 750                              // vận tốc bánh trái
#define vR 750                              // vận tốc bánh phải
#define distance1 60
#define distance2 100

int time = 1000;                           // Thời gian delay
int threshold;                             // Giá trị của cảm biến IR (bổ sung sau) 
bool firsttime = true;
bool check_mode1 = true;
bool check_mode2 = true;
bool check_mode3 = true;
bool check_mode4 = true;

void setup() {
  Leanbot.begin();                          // initialize Leanbot
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

void TurnRight() {                        // Rẽ phải
  LbMotion.runLR(vL, -vR);   
  LbMotion.waitRotationDeg(90);
  LbDelay(time); 
}

void TurnLeft() {                         // Rẽ trái
  LbMotion.runLR(-vL, vR);   
  LbMotion.waitRotationDeg(90);
  LbDelay(time);
}

void Forward() {                         // Đi thẳng
  LbMotion.runLR(vL, vR);
}

void CO2() {                             // Thoát khỏi ô CO2
  Forward();
  LbMotion.waitDistanceMm(distance1);
  Buzzer();
  LbDelay(time);
  TurnLeft();
  Forward();
  LbMotion.waitDistanceMm(distance2);
  Buzzer();
  LbDelay(time);
}

void FollowLine() {                       // Dò line
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
