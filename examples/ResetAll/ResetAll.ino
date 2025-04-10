/**
 * This library shows how to use the resetAllTimers() method
 */

#include <Arduino.h>
#include <BlockNot.h>

BlockNot timer1(32, SECONDS, GLOBAL_RESET);
BlockNot timer2(52, SECONDS);
BlockNot timer3(19000);
BlockNot printRemainingTimer(2, SECONDS);
BlockNot resetTimer(8, SECONDS);

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

void printRemaining() {
    String t1 = "Timer One Remaining:   " + String(timer1.REMAINING) + " " + timer1.GET_UNITS;
    String t2 = "Timer Two Remaining:   " + String(timer2.REMAINING) + " " + timer2.GET_UNITS;
    String t3 = "Timer Three Remaining: " + String(timer3.REMAINING) + " " + timer3.GET_UNITS;
    Serial.println(t1);
    Serial.println(t2);
    Serial.println(t3);
    Serial.print("\n");
}


void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println(F("\nREADY!\n"));
    delay(1000);
    printTimers();
    int count = 0;
    while(true) {
        if(printRemainingTimer.TRIGGERED) {
            printRemaining();
            count++;
        }
        if (resetTimer.TRIGGERED) {
            Serial.println("\nRESET\n");
            RESET_TIMERS;
        }
        if (count >= 12)
            break;
    }
    Serial.println("\nDONE!\n");
}

void loop() {
}
