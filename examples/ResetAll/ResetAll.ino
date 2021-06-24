/**
 * This library shows how to use the resetAllTimers() method
 */

#include <BlockNot.h>

BlockNot timer1( 1, SECONDS);
BlockNot timer2( 5, SECONDS);
BlockNot timer3( 7500 );

BlockNot resetTimer(30, SECONDS);  // Timer to trigger reset of all timers every 30s

unsigned long startTime;   // To count seconds independent of timers and use them as a simple reference.

void printTimers() {
    String heading = "Timer\tDuration\tUnits\n-----------------------------";
    String t1 = "One\t" + String(timer1.DURATION) + "\t\t" + timer1.GET_UNITS;
    String t2 = "Two\t" + String(timer2.DURATION) + "\t\t" + timer2.GET_UNITS;
    String t3 = "Three\t" + String(timer3.DURATION) + "\t\t" + timer3.GET_UNITS;
    Serial.println(heading);
    Serial.println(t1);
    Serial.println(t2);
    Serial.println(t3);
    Serial.print("\n");
}

void printTime(int timer) {
    String time = String(millis() - startTime);
    String timerString = timer == 1 ? "ONE" : (timer == 2) ? "TWO" : "THREE";
    Serial.println(time + ": " + timerString + " triggered");
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    Serial.println(F("READY!\n"));
    delay(1000);
    printTimers();
    startTime = millis();
    resetAllTimers(startTime);
}

void loop()
{
    if (timer1.TRIGGERED) {
        printTime(1);
    }
    
    if (timer2.TRIGGERED) {
        printTime(2);
    }
    
    if (timer3.TRIGGERED) {
        printTime(3);
    }
    
    if (resetTimer.TRIGGERED_ON_DURATION) {
        Serial.println(F("\n\n---- Reset all timers ----\n"));
        RESET_TIMERS;
    }
}
