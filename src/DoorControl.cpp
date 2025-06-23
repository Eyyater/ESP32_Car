#include <Arduino.h> 

const uint8_t up = 25;
const uint8_t down = 27;
int door = 0;

void Door_Init() {
    pinMode(up, OUTPUT);
    pinMode(down, OUTPUT);

    ledcSetup(4, 10000, 8);
    ledcAttachPin(up, 4);
    ledcSetup(5, 10000, 8);
    ledcAttachPin(down, 5);

    ledcWrite(4, 0);
    ledcWrite(5, 0);
}

void Door_Down () {
    if (door == 0) {
        ledcWrite(4, 150);
        ledcWrite(5, 0);
        // digitalWrite(up, HIGH);
    
        delay(100);
    
        ledcWrite(4, 0);
        ledcWrite(5, 0);
        // digitalWrite(up, LOW);
        delay(200);
        door = 1;
    }
}

void Door_Open () {
    if (door == 1) {
        ledcWrite(5, 150);
        ledcWrite(4, 0);
        // digitalWrite(down, HIGH);

        delay(100);

        ledcWrite(4, 0);
        ledcWrite(5, 0);
        // digitalWrite(down, LOW);
        delay(200);
        door = 0;
    }
}

void Door_Test() {
    digitalWrite(up, HIGH);
    digitalWrite(down, LOW);

    delay(5000);

    digitalWrite(up, LOW);
    digitalWrite(down, LOW);
}