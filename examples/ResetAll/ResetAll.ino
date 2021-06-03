#include <BlockNot.h>

BlockNot myTimer03s( 3, SECONDS);
BlockNot myTimer05s( 5, SECONDS);
BlockNot myTimer07s( 7, SECONDS);
BlockNot myTimer11s(11, SECONDS);
BlockNot myTimer13s(13, SECONDS);

BlockNot myReset   (30, SECONDS);  // Timer to trigger reset of all timers every 30s

unsigned long startTime = millis();   // To count seconds independent of timers

void printTime(BlockNot* timer) {
    unsigned long seconds = (millis() - startTime)/1000;
    Serial.print("\nTimestamp is " + String(seconds) + ": " + String(timer->getDuration()) + "s timer was triggered");
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
        printTime(&myReset);
        Serial.println(F("\n\n---- Reset all timers ----")); 
        resetAllTimers();
    } 
}
