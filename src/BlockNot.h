#ifndef BlockNot_h
#define BlockNot_h
#pragma once

#include <Arduino.h>

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
    static BlockNot* firstTimer;
    static BlockNot* currentTimer;

    BlockNot() { 
        addToTimerList();
    }

    BlockNot(unsigned long milliseconds) {
        duration = milliseconds;
        reset();
        addToTimerList();
    }

    BlockNot(unsigned long milliseconds, unsigned long disableReturnValue) {
        duration = milliseconds;
        disableReturn = disableReturnValue;
        reset();
        addToTimerList();
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

    bool triggered(bool resetOption = true) {
        bool triggered = hasTriggered();
        if (resetOption && triggered) {
            reset();
        }
        return triggered;
    }
    
    bool notTriggered() { return hasNotTriggered(); }
    
    bool firstTrigger() {
        if (hasTriggered() && !onceTriggered) {
            onceTriggered = true;
            return true;
        }
        return false;
    }
    
    bool isEnabled() {
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

    void setNextTimer( BlockNot* timer ) { nextTimer = timer; }

    BlockNot* getNextTimer() { return nextTimer; }

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

    BlockNot* nextTimer;
    
    void resetTimer() {
        startTime = enabled ? millis() : startTime;
        onceTriggered = enabled ? false : onceTriggered;
    }
    
    unsigned long remaining() { return enabled ? (startTime + duration) - millis() : disableReturn; }

    unsigned long timeSinceReset() { return enabled ? (millis() - startTime) : disableReturn; }

    bool hasTriggered() { return enabled ? ((millis() - startTime) >= duration) : false; }

    bool hasNotTriggered() { return enabled ? ((millis() - startTime) < duration) : false; }

    void addToTimerList() {
        if ( firstTimer == nullptr ) {
            firstTimer = currentTimer = this;
        } else {
            currentTimer->setNextTimer( this );
            currentTimer = this;
        }
        this->setNextTimer( nullptr );
    }
};

void resetAllTimers() {
    BlockNot* timer = BlockNot::firstTimer;
    while( timer != nullptr ) {
        timer->reset();
        timer = timer->getNextTimer();
    }

}

BlockNot* BlockNot::firstTimer   = nullptr;
BlockNot* BlockNot::currentTimer = nullptr;

#endif
