#include <Leanbot.h>                    // use Leanbot library
#define vL 500                              // v?n t?c bánh trái
#define vR 500                              // v?n t?c bánh ph?i
int speed = 500;
int threshold;                             // Giá tr? c?a c?m bi?n IR (b? sung sau)
unsigned int distance_4 = Leanbot.pingMm();
int obstacle;
int cornerL = LbGripper.readL();
int cornerR = LbGripper.readR();
void setup() {
    Leanbot.begin();                      // initialize Leanbot

}


void TurnRight() {                        // R? ph?i
    LbMotion.runLR(vL, -vR);
    LbMotion.waitRotationDeg(90);
    LbMission.end();
}

void TurnLeft() {                         // R? trái
    LbMotion.runLR(-vL, vR);
    LbMotion.waitRotationDeg(90);
    LbMission.end();
}

void Forward() {                         // ?i th?ng
    LbMotion.runLR(vL, vR);
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


void loop() {
    LbMission.begin(TB1A + TB1B); // B?t ??u nhi?m v? khi c? TB1A và TB1B ???c ch?m
    distance_4 = Leanbot.pingMm(); // ?o kho?ng cách b?ng c?m bi?n siêu âm
    obstacle = LbIRArray.read(ir5R); // ??c giá tr? c?m bi?n

    Serial.println(obstacle);

    FollowLine(); // G?i hàm dò line
    Forward(); // G?i hàm ?i th?ng

    if (distance_4 < 30 && obstacle > 0) {
        TurnRight(); // R? trái n?u có v?t c?n
        delay(100); // D?ng l?i trong 100ms
        FollowLine();
        Forward(); // Ti?p t?c ?i th?ng
    }

    if (distance_4 < 30) {
        LbMotion.runLR(400, 400); // Di chuy?n v? phía tr??c
        while (LbIRLine.isBlackDetected()); // Ti?p t?c di chuy?n khi còn th?y ???ng ?en
        LbMotion.stopAndWait(); // D?ng l?i
        LbGripper.moveToLR(90, 90, 10000); // Di chuy?n gripper

    }

    LbMission.end(); // K?t thúc nhi?m v?
}