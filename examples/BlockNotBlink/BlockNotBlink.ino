#include <BlockNot.h>

BlockNot blinkTimer(100);
boolean state = false;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

/*
Here, we can see how BlockNot handles the 100ms
time delay between LED state changes with ease 
and simplicity.
*/
    
    if (blinkTimer.TRIGGERED) {
        state = !state;
        digitalWrite(LED_BUILTIN, (state ? HIGH : LOW));
    }
    
}
