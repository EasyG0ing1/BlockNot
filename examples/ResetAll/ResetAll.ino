#include <BlockNotMod.h>

BlockNot myTimer03s( 3000);
BlockNot myTimer05s( 5000);
BlockNot myTimer07s( 7000);
BlockNot myTimer11s(11000);
BlockNot myTimer13s(13000);

BlockNot myReset   (30000);  // Timer to trigger reset of all timers every 30s

long startTime = millis();   // To count seconds independent of timers

void printTime(BlockNot* timer) {
    long seconds = (millis() - startTime)/1000;
    Serial.print("\nTimestamp is " + String(seconds) + ": " + String(timer->getDuration() / 1000) + "s timer was triggered");
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    Serial.println(F("We're ready!"));
}

void loop()
{
    if (myTimer03s.TRIGGERED) { 
        printTime(&myTimer03s);
    }
 
    if (myTimer05s.TRIGGERED) { 
        printTime(&myTimer05s);
    }
 
    if (myTimer07s.TRIGGERED) { 
        printTime(&myTimer07s);
    }
 
    if (myTimer11s.TRIGGERED) { 
        printTime(&myTimer11s);
    }
 
    if (myTimer13s.TRIGGERED) { 
        printTime(&myTimer13s);
    }

    if (myReset.TRIGGERED) { 
        resetAllTimers();
        Serial.println(F("\n\nXXXX Reset all timers XXXX")); 
    } 
}