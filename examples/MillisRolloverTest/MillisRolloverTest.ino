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

/*
 * Once millis() has rolled over, a new rollover time will be set by a random number,
 * every seven seconds ... UNLESS ...
 */

/*
 * USER INPUT
 *
 * You can type in the word reset with a number to reset the roll to however many seconds
 * you wish. Ex:
 *
 *      reset15
 *
 * Will set the millis() for the mainTimer to roll over to 0 in 15 seconds. The mainTimer
 * will continue counting and displaying its elapsed duration about once ever second, but
 * what is important to note, is that even after the rollover, the elapsed duration for
 * mainTimer does not skip a beat.
 *
 * The reason why the elapsed time presented for mainTimer drifts as much as it does is
 * because of the blocking nature that is inherent with Serial activity and since it's
 * writing text to the Serial port every second.
 *
 * Since Serial.println is a blocking method, it will inevitably shift the point in time
 * where the variable elapsed is calculated under the if then statement for
 * reportTimer.TRIGGERED
 */

BlockNot mainTimer(15000);
BlockNot checkTimer(550);
BlockNot reportTimer(1135);
BlockNot randomTimer(7, SECONDS);

unsigned long mainTimerMillisStart;
unsigned long startTime;
bool millisReset = false;

unsigned long getRandomSeconds() {
    return random(3,25);
}

void setMillisRolloverTo(unsigned long seconds) {
    mainTimer.setMillisOffset(4294967295 - millis() - (seconds * 1000));
    mainTimerMillisStart = mainTimer.getMillis();
    millisReset = false;
    Serial.println("\n\t\tMillis reset to roll in " + String(seconds) + " seconds.\n");
}

void checkSerial() {
    String input = Serial.readString();
    if(input.startsWith("reset")) {
        unsigned long seconds = input.substring(5).toInt();
        setMillisRolloverTo(seconds);
    }
    Serial.flush();
}

void setup() {
    Serial.begin(115200);
    mainTimer.setMillisOffset(4294958296); //9 seconds before rollover
    mainTimer.RESET;
    startTime = millis();
    mainTimerMillisStart = mainTimer.getMillis();
    randomSeed(analogRead(0));
    Serial.println(F("Test Started"));
}

void loop() {
    static unsigned long currentTimerMillis;
    currentTimerMillis = mainTimer.getMillis();
    if(checkTimer.TRIGGERED) {
        currentTimerMillis = mainTimer.getMillis();
        if (currentTimerMillis < mainTimerMillisStart) {
            Serial.println(F("\nMillis Rolled"));
            Serial.println("Start value: " + String(mainTimerMillisStart));
            Serial.println("Current value: " + String(currentTimerMillis) + "\n");
            mainTimerMillisStart = currentTimerMillis;
            millisReset = true;
            randomTimer.RESET;
        }
    }
    if(reportTimer.TRIGGERED) {
        unsigned long elapsed = mainTimer.getTimeSinceLastReset();
        Serial.println("mainTimer: " + String(elapsed) + " milliseconds");
    }
    if(mainTimer.TRIGGERED) {
        unsigned long endTime = millis();
        long delta = endTime - startTime;
        auto elapsed = (double) (delta/1000);
        Serial.println("\n****************************************************\n* Main Timer Triggered after " + String(elapsed,0) + " seconds elapsed *\n****************************************************\n");
        startTime = endTime;
    }
    if (randomTimer.TRIGGERED && millisReset) {
        unsigned long seconds = getRandomSeconds();
        setMillisRolloverTo(getRandomSeconds());
    }
    if (Serial.available())
        checkSerial();
}
