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
#define GET_UNITS        getUnits()
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
#define SECONDS          true
#define MILLISECONDS     false

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

    BlockNot(unsigned long time, bool units = MILLISECONDS) {
        baseUnits = units;
        duration = (baseUnits == SECONDS) ? (time * 1000) : time;
        reset();
        addToTimerList();
    }

    BlockNot(unsigned long milliseconds, unsigned long disableReturnValue) {
        duration = milliseconds;
        disableReturn = disableReturnValue;
        reset();
        addToTimerList();
    }
    
    BlockNot(unsigned long time, bool units, unsigned long disableReturnValue) {
        baseUnits = units;
        disableReturn = disableReturnValue;
        duration = (baseUnits == SECONDS) ? (time * 1000) : time;
        reset();
        addToTimerList();
    }
    
    
    void setDuration(const unsigned long time, bool resetOption = WITH_RESET) {
        if (enabled) {
            duration = (baseUnits == SECONDS) ? (time * 1000) : time;
            if (resetOption) reset();
        }
    };
    
    void addTime(const unsigned long time, bool resetOption = NO_RESET) {
        if (enabled) {
            const unsigned long newDuration = duration + ((baseUnits == SECONDS) ? (time * 1000) : time);
            duration = newDuration;
            if (resetOption) reset();
        }
    }
    
    void takeTime(const unsigned long time, bool resetOption = NO_RESET) {
        if (enabled) {
            long newDuration = duration - ((baseUnits == SECONDS) ? (time * 1000) : time);
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
    
    unsigned long getDuration() {
        unsigned long thisDuration = (baseUnits == SECONDS) ? (duration / 1000) : duration;
        return enabled ? thisDuration : disableReturn;
    }
    
    String getUnits() {
        return baseUnits == SECONDS ? "Seconds" : "Milliseconds";
    }
    
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
    unsigned long duration = 0;

    unsigned long startTime = 0;

    unsigned long disableReturn = 0;

    bool enabled = true;

    bool onceTriggered = false;

    bool baseUnits = MILLISECONDS;

    BlockNot* nextTimer;

    void resetTimer() {
        startTime = enabled ? millis() : startTime;
        if (enabled) onceTriggered = false;
    }

    unsigned long remaining() {
        unsigned long result =  enabled ? (startTime + duration) - millis() : disableReturn;
        settleUnits(result);
    }

    unsigned long timeSinceReset() {
        unsigned long result = enabled ? (millis() - startTime) : disableReturn;
        settleUnits(result);
    }

    bool hasTriggered() {
        if (enabled) return millis() - startTime >= duration;
        else return false;
    }

    bool hasNotTriggered() {
        if (enabled) return millis() - startTime < duration;
        else return false;
    }

    void addToTimerList() {
        if ( firstTimer == nullptr ) {
            firstTimer = currentTimer = this;
        } else {
            currentTimer->setNextTimer( this );
            currentTimer = this;
        }
        this->setNextTimer( nullptr );
    }
    
    unsigned long settleUnits(unsigned long input) {
        return ((baseUnits == SECONDS) ? input / 1000 : input);
    }

};

void resetAllTimers() {
    BlockNot* timer = BlockNot::firstTimer;
    while( timer != nullptr ) {
        timer->BlockNot::reset();
        timer = timer->BlockNot::getNextTimer();
    }
}

BlockNot* BlockNot::firstTimer   = nullptr;
BlockNot* BlockNot::currentTimer = nullptr;

#endif
