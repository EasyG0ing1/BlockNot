#include <Arduino.h>
#include <BlockNot.h>

BlockNot myTimer(8000);
BlockNot triggerTimer(10000);
BlockNot stopAfterThreeTimer(3000);
BlockNot liteTimer(15000);
BlockNot shortTimer (500);
BlockNot secondsCounter(1000);

void setup() {
    Serial.begin(57600);
    delay(1800);
    Serial.println(F("\nREADY!\n"));
}

void loop()
{

    static int x = 0;
    static unsigned long seconds = 0;

    if (secondsCounter.TRIGGERED){
        //adds one to variable seconds after the trigger event (1 second)
        seconds++;
    }
    if (myTimer.TRIGGERED) {
        Serial.println(("myTimer("+String(seconds)+"): This timer will trigger every 8 seconds!"));
        Serial.println(F("---------------------------------------------"));
    }

    if (triggerTimer.FIRST_TRIGGER) {
        Serial.println(("triggerTimer("+String(seconds)+"): This will trigger at 10 seconds, then it will not run again."));
        Serial.println(F("---------------------------------------------"));
    }

    if (stopAfterThreeTimer.FIRST_TRIGGER) {
        Serial.println(("stopAfterThreeTimer("+String(seconds)+"): This timer runs ONE TIME after 3 seconds then does nothing until it is reset from liteTimer (every 15 seconds). It will then trigger again 3 seconds after it has been reset."));
        Serial.println(F("---------------------------------------------"));
    }

    if (liteTimer.TRIGGERED) {
        Serial.println(("liteTimer("+String(seconds)+"): This is a 15 second timer. Three seconds after it triggers, stopAfterThreeTimer should trigger."));
        stopAfterThreeTimer.RESET;
        Serial.println(F("---------------------------------------------"));
    }

    if (shortTimer.TRIGGERED) {
        x++;
        if (x > 10) {
            x = 0;
            Serial.println("shortTimer("+String(seconds)+"): This timer runs code when it has triggered over 10 times. It is set for 1/2 second and should therefore run this code every 5 seconds.");
            Serial.println(F("---------------------------------------------"));
        }
    }
}
