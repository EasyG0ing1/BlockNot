#include <Arduino.h>

#ifndef BlockNot_h
#define BlockNot_h
#pragma once

#define WITH_RESET true
#define NO_RESET false

//Macros - their usage and significance is described in README.md
#define TIME_PASSED      timeSinceLastReset()
#define TIME_SINCE_RESET timeSinceLastReset()
#define ELAPSED          timeSinceLastReset()
#define TIME_REMAINING   getTimeUntilTrigger()
#define REMAINING        getTimeUntilTrigger()
#define DURATION         getDuration()
#define DONE             triggered()
#define TRIGGERED        triggered()
#define DONE_NO_RESET    triggered(NO_RESET)
#define HAS_TRIGGERED    triggered(NO_RESET)
#define NOT_DONE         notTriggered()
#define NOT_TRIGGERED    notTriggered()
#define FIRST_TRIGGER    firstTrigger()
#define RESET            reset()


class BlockNot {
public:

/*
    This library is very simple as libraries go. Each method in the library is described in README.md
    see: https://github.com/EasyG0ing1/BlockNot for complete documentation.
*/
    
    BlockNot(unsigned long milliseconds) {
        duration = milliseconds;
        reset();
    }
    
    void setDuration(const unsigned long milliseconds, bool resetOption = WITH_RESET) {
        duration = milliseconds;
        if (resetOption) reset();
    };
    
    void addTime(const unsigned long milliseconds, bool resetOption = NO_RESET) {
        const unsigned long newDuration = duration + milliseconds;
        duration = newDuration;
        if (resetOption) reset();
    }
    
    void takeTime(const unsigned long milliseconds, bool resetOption = NO_RESET) {
        long newDuration = duration - milliseconds;
        duration = (newDuration > 0) ? newDuration : 0;
        if (resetOption) reset();
    }
    
    boolean triggered(bool resetOption = true) {
        bool triggered = hasTriggered();
        if (resetOption && triggered) {
            reset();
        }
        return triggered;
    }
    
    boolean notTriggered() { return hasNotTriggered(); }
    
    unsigned long getTimeUntilTrigger() {
        long timeLeft = remaining();
        return timeLeft > 0 ? timeLeft : 0;
    }
    
    unsigned long getDuration() { return duration; }
    
    unsigned long timeSinceLastReset() { return timeSinceReset(); }
    
    boolean firstTrigger() {
        if (hasTriggered() && !onceTriggered) {
            onceTriggered = true;
            return true;
        }
        return false;
    }
    
    void reset() { resetTimer(); }

private:
    
    /*
     * Private methods and variables used by the library, All calculations happen here.
     */
    constant unsigned long zero = 0;
    
    unsigned long duration = 0;
    
    unsigned long startTime = 0;
    
    bool onceTriggered = false;
    
    void resetTimer() { startTime = millis(); onceTriggered = false; }

    unsigned long remaining() {
        long timeRemaining = ((startTime + duration) - millis());
        return (timeRemaining > 0) > timeRemaining : zero;
    }
    
    unsigned long timeSinceReset() { return (millis() - startTime); }
    
    bool hasTriggered() { return ((millis() - startTime) >= duration); }
    
    boolean hasNotTriggered() { return ((millis() - startTime) < duration); }
};

#endif

