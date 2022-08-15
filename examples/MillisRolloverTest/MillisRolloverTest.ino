#include <Arduino.h>
#include <BlockNot.h>

/*
 * This sketch is intended to offer you the ability to see that when millis() rolls over,
 * A BlockNot timer continues to calculate time duration accurately.
 *
 * mainTimer is the timer we are clocking in this test, and we record the value of the
 * Arduino's actual millis() each time that timer resets, then when it triggers, we
 * get millis() again and we take the difference so that we can see whether or not
 * the time between triggers is actually equal to the duration set in the timer.
 *
 * BlockNot has the ability to include an offset for millis() that I put into the library
 * specifically to perform this kind of a test. you can assign the offset using a timers
 * setMillisOffset(unsigned long) method.
 *
 * In this test, I set the millis offset to be a value that is 20 seconds before the
 * rollover will happen. Then the main timer is set for 15 seconds, that way the rollover
 * will happen 5 seconds into the second trigger event.
 *
 * You can also use a stop watch, and start it when the mainTimer triggers the first time,
 * then stop it when it triggers the second time (after the rollover) to verify that it
 * did trigger in 15 seconds despite the rollover occurring 5 seconds into the duration
 * of the timer.
 *
 * Once the rollover happens in this test, and you see that the timer triggers properly,
 * you will need to reset the Arduino to see the test again, or you'll have to wait
 * about two months ;-)
 */

BlockNot mainTimer(15, SECONDS);
BlockNot checkTimer(550);

unsigned long startTime;

void setup() {
    Serial.begin(115200);
    mainTimer.setMillisOffset(4294947296); //20 seconds before rollover
    mainTimer.RESET;
    startTime = millis();
    Serial.println(F("Test Started"));
}

void loop() {
    static unsigned long startMillis = mainTimer.getMillis();

    if(checkTimer.TRIGGERED) {
        long currentTimerMillis = mainTimer.getMillis();
        if (currentTimerMillis < startMillis) {
            Serial.println(F("\nMillis Rolled"));
            Serial.println("Start value: " + String(startMillis));
            Serial.println("Current value: " + String(currentTimerMillis) + "\n");
            startMillis = currentTimerMillis;
        }
    }
    if(mainTimer.TRIGGERED) {
        unsigned long endTime = millis();
        long delta = endTime - startTime;
        Serial.println("Timer Triggered after " + String(delta/1000) + " seconds elapsed");
        startTime = endTime;
    }
}
