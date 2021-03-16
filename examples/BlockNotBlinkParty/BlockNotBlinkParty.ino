#include <BlockNot.h>

BlockNot blinkTimer1(150);
BlockNot blinkTimer2(275);
BlockNot blinkTimer3(400);
BlockNot blinkTimer4(525);

#define LED1 9
#define LED2 10
#define LED3 11
#define LED4 12

boolean state1 = false;
boolean state2 = false;
boolean state3 = false;
boolean state4 = false;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}

void loop() {

/*
Tru doing this with the delay() method ... not gonna happen!
*/
    
    if (blinkTimer1.TRIGGERED) {
        state1 = !state1;
        digitalWrite(LED1, (state1 ? HIGH : LOW));
    }

    if (blinkTimer2.TRIGGERED) {
        state2 = !state2;
        digitalWrite(LED2, (state2 ? HIGH : LOW));
    }

    if (blinkTimer3.TRIGGERED) {
        state3 = !state3;
        digitalWrite(LED3, (state3 ? HIGH : LOW));
    }

    if (blinkTimer4.TRIGGERED) {
        state4 = !state4;
        digitalWrite(LED4, (state4 ? HIGH : LOW));
    }

}
