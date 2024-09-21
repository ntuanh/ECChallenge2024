


#include <Leanbot.h>                    // use Leanbot library
#define vL 400
#define vR 400
#define distance_green 60
#define distance_black 90

int time = 1000;

void setup() {
  Leanbot.begin();                      // initialize Leanbot
  xTaskCreate(Motion, "Move", 128, NULL, 1, NULL);
  xTaskCreate(Led, "Light", 128, NULL, 1, NULL);
  xTaskCreate(Sound, "Tone", 128, NULL, 1, NULL);
}

void Motion(void *para);
void Led(void *para);
void Sound(void *para);

void loop() {
  
}

void Sound(void *para) {             // Bật còi cảnh báo
  (void)para;
  void Buzzer() {                     
    const int freq[] = {1000, 1200, 1000, 1200, 1000};
    while(1) {
      for(int i = 0; i < 5; i++) {
      Leanbot.tone(freq[i], 500);
      LbDelay(300);
      }
      LbDelay(400);
    }
  }
}

void Led(void *para) {              // Bật đèn nhấp nháy theo nhịp
  (void)para;
  void EmergencySignal() {          
    byte LedRandom = BITMAP(ledA, ledF, ledE, ledD, ledC, ledB);
    LbRGB[LedO] = CRGB::Yellow;
    LbRGB.show();
    while(1) {
      LbRGB.fillColor(CRGB::Red, LedRandom);
      LbDelay(time);
      LbRGB.show();
    }
  }
}

void Motion(void *para) {
  (void)para;
  void Out_of_CO2() {         // Thoát khỏi ô CO2
    LbMotion.runLR(vL, vR);
    LbMotion.waitDistanceMm(distance_green);
    LbDelay(time);
    LbMotion.runLR(-vL, vR);
    LbMotion.waitRotationDeg(90);
    LbDelay(time);
    LbMotion.runLR(vL, vR);
    LbMotion.waitDistanceMm(distance_black);
  }

  void FollowLine() {         // Di chuyển đến ô thả bình cứu hỏa
    int line = LbIRLine.read();  
    static int count = 0;       
    while(LbIRLine.isBlackDetected()) {
      if (line == 0b1100) {  
        LbMotion.runLR(-vL, vR);   
        LbMotion.waitRotationDeg(90);
        LbMotion.runLR(vL, vR);  
      } 
      else if (line == 0b0011) {  
        LbMotion.runLR(vL, -vR);   
        LbMotion.waitRotationDeg(90);
        LbMotion.runLR(vL, vR);  
      } 
      else if (line == 0b0011 && line == 0b1100) { 
        if (count == 0) {  
          LbMotion.runLR(-vL, vR);
          LbMotion.waitRotationDeg(90);
          LbMotion.runLR(vL, vR);
          count++;
        } else { 
          LbMotion.runLR(vL, vR);
        }
      } 
      else {  
        LbMotion.runLR(vL, vR);
      }
    }
  }

  void FireZone() {          // Thả bình cứu hỏa
    int zone = LBIRLine.read();
    if(zone == 0) {
      LbGripper.open();
      Buzzer();
    } else {
      LbGripper.close();
    }
  }
}


