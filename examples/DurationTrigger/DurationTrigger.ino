/*
 * The README file has a thorough explanation of what is going on in this sketch.
 *
 * Read the section about Triggered On Duration then come back here and play
 * with this sketch. It should drive the concept home since you will be able to
 * see in real time what this trigger method does.
 *
 * In Serial Monitor, type p then hit enter to pause the loop, then wait for a
 * few durations to pass then enter p again to enable the loop and you will
 * see how BlockNot can stack up your trigger events if you need that feature.
 */

#include <Arduino.h>
#include <BlockNot.h>

BlockNot mainTimer(10000);
BlockNot checkTimer(1,SECONDS);

bool run = true;

void processSerial() {
    String input = Serial.readString();
    if (input.startsWith("p")) {
        run = !run;
        Serial.println((run ? "Loop RUNNING" : "Loop PAUSED"));
    }
    while(Serial.available()) Serial.read();
}

void setup() {
    Serial.begin(115200);
    Serial.println(F("\nREADY!\n"));
}

void loop() {
    if (Serial.available()) processSerial();
    if (run) {
        if(checkTimer.TRIGGERED) {
            if (mainTimer.TRIGGERED_ON_DURATION_ALL) {
                Serial.println("Timer TRIGGERED");
            }
            else {
                String startTime = String(mainTimer.getStartTime());
                String now = String(millis());
                String duration = String(mainTimer.getDuration());
                String nextTrigger = String(mainTimer.getNextTriggerTime());

                Serial.print("Timer DID NOT TRIGGER.");
                Serial.print(" Start time is: " + startTime);
                Serial.print(" Now: "  + now);
                Serial.print(" Duration: " + duration);
                Serial.println(" Next Trigger: " + nextTrigger);
            }
        }
    }
}
