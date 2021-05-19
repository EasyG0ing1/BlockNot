#ifndef BlockNot_h
#define BlockNot_h
#pragma once

#include <Arduino.h>
#include <forward_list>
using namespace std;

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
#define ENABLE           enable()
#define DISABLE          disable()
#define SWAP             negateState()
#define FLIP             negateState()
#define SWAP_STATE       negateState()
#define FLIP_STATE       negateState()
#define ENABLED          isEnabled()


class BlockNot {
public:

/*
    This library is very simple as libraries go. Each method in the library is described in README.md
    see: https://github.com/EasyG0ing1/BlockNot for complete documentation.
*/
    static forward_list<BlockNot*> timerList;

    BlockNot() { 
	    timerList.push_front(this);
    }
	
	BlockNot(unsigned long milliseconds) {
        duration = milliseconds;
        reset();
	    timerList.push_front(this);
    }
    
    BlockNot(unsigned long milliseconds, unsigned long disableReturnValue) {
        duration = milliseconds;
        disableReturn = disableReturnValue;
        reset();
	    timerList.push_front(this);
    }
    
    void setDuration(const unsigned long milliseconds, bool resetOption = WITH_RESET) {
        if (enabled) {
            duration = milliseconds;
            if (resetOption) reset();
        }
    };
    
    void addTime(const unsigned long milliseconds, bool resetOption = NO_RESET) {
        if (enabled) {
            const unsigned long newDuration = duration + milliseconds;
            duration = newDuration;
            if (resetOption) reset();
        }
    }
    
    void takeTime(const unsigned long milliseconds, bool resetOption = NO_RESET) {
        if (enabled) {
            long newDuration = duration - milliseconds;
            duration = newDuration > 0 ? newDuration : 0;
            if (resetOption) reset();
        }
    }
    
    boolean triggered(bool resetOption = true) {
        bool triggered = hasTriggered();
        if (resetOption && triggered) {
            reset();
        }
        return triggered;
    }
    
    boolean notTriggered() { return hasNotTriggered(); }
    
    boolean firstTrigger() {
        if (hasTriggered() && !onceTriggered) {
            onceTriggered = true;
            return true;
        }
        return false;
    }
    
    boolean isEnabled() {
        return enabled;
    }
    
    unsigned long getTimeUntilTrigger() { return remaining(); }
    
    unsigned long getDuration() { return enabled ? duration : disableReturn; }
    
    unsigned long timeSinceLastReset() { return timeSinceReset(); }
    
    void setDisableReturnValue(unsigned long disableReturnValue) {
        disableReturn = disableReturnValue;
    }
    
    void enable() { enabled = true; }
    
    void disable() { enabled = false; }
    
    void negateState() { enabled = !enabled; }
    
    void reset() { resetTimer(); }

private:
    
    /*
     * Private methods and variables used by the library, All calculations happen here.
     */
    unsigned long zero = 0;
    
    unsigned long duration = 0;
    
    unsigned long startTime = 0;
    
    unsigned long disableReturn = 0;
    
    bool enabled = true;
    
    bool onceTriggered = false;
    
    void resetTimer() {
        startTime = enabled ? millis() : startTime;
        onceTriggered = enabled ? false : onceTriggered;
    }
    
    unsigned long remaining() { return enabled ? (startTime + duration) - millis() : disableReturn; }
    
    unsigned long timeSinceReset() { return enabled ? (millis() - startTime) : disableReturn; }
    
    bool hasTriggered() { return enabled ? ((millis() - startTime) >= duration) : false; }
    
    boolean hasNotTriggered() { return enabled ? ((millis() - startTime) < duration) : false; }
};

void resetAllTimers() {
    for( BlockNot* timer : BlockNot::timerList ) timer->reset() ; 
}

forward_list<BlockNot*> BlockNot::timerList ;

#endif
