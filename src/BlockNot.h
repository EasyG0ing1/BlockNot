#ifndef BlockNot_h
#define BlockNot_h
#pragma once

#include <Arduino.h>

#define WITH_RESET      true
#define NO_RESET        false
#define SECONDS         true
#define MILLISECONDS    false

//Macros - their usage and significance is described in README.md

#define TIME_PASSED             timeSinceLastReset()
#define TIME_SINCE_RESET        timeSinceLastReset()
#define ELAPSED                 timeSinceLastReset()
#define TIME_REMAINING          getTimeUntilTrigger()
#define REMAINING               getTimeUntilTrigger()
#define DURATION                getDuration()
#define DONE                    triggered()
#define GET_UNITS               getUnits()
#define TRIGGERED               triggered()
#define TRIGGERED_ON_DURATION   triggeredOnDuration()
#define DONE_NO_RESET           triggered(NO_RESET)
#define HAS_TRIGGERED           triggered(NO_RESET)
#define NOT_DONE                notTriggered()
#define NOT_TRIGGERED           notTriggered()
#define FIRST_TRIGGER           firstTrigger()
#define RESET                   reset()
#define RESET_TIMERS            resetAllTimers()
#define ENABLE                  enable()
#define STOP                    stop()
#define START                   start()
#define DISABLE                 disable()
#define ENABLED                 isEnabled()
#define SWAP                    negateState()
#define FLIP                    negateState()
#define SWAP_STATE              negateState()
#define FLIP_STATE              negateState()

class BlockNot {
public:
/*
    This library is very simple as libraries go. Each method in the library is described in README.md
    see: https://github.com/EasyG0ing1/BlockNot for complete documentation.
*/

    BlockNot() { addToTimerList(); }

    BlockNot(unsigned long time, bool units = MILLISECONDS) {
        baseUnits = units;
        duration = convertUnitsSet(time);
        reset();
        addToTimerList();
    }

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, bool units = MILLISECONDS) {
        baseUnits = units;
        timerStoppedReturnValue = stoppedReturnValue;
        duration = convertUnitsSet( time );
        reset();
        addToTimerList();
    }
    
    void setDuration(const unsigned long time, bool resetOption = WITH_RESET) {
        duration = convertUnitsSet(time);
        if (resetOption) reset();
    };
    
    void addTime(const unsigned long time, bool resetOption = NO_RESET) {
        const unsigned long newDuration = duration + convertUnitsSet(time);
        duration = newDuration;
        if (resetOption) reset();
    }
    
    void takeTime(const unsigned long time, bool resetOption = NO_RESET) {
        long newDuration = duration - convertUnitsSet(time);
        duration = newDuration > 0 ? newDuration : 0;
        if (resetOption) reset();
    }

    bool triggered(bool resetOption = true) {
        bool triggered = hasTriggered();
        if (resetOption && triggered) {
            reset();
        }
        return timerEnabled ? triggered : false;
    }
    
    bool triggeredOnDuration() {
        /**
         * Sets the next startTime to the current millis less the amount of time that has passed since the Triggered event was supposed to happen.
         * This keeps your timer triggering exactly every duration on the duration. The method getNewStartTime runs the calculation accounting
         * for possible rollover in the millis() value.
         */
        bool triggered = hasTriggered();
        if (triggered) {
            long elapsedTime = (long) timeSinceLastReset();
            reset(getNewStartTime(duration - elapsedTime));
        }
        return triggered;
    }
    
    bool notTriggered() { return timerEnabled ? hasNotTriggered() : false; }
    
    bool firstTrigger() {
        if (hasTriggered() && !onceTriggered) {
            onceTriggered = true;
            return timerEnabled ? true : false;
        }
        return false;
    }
    
    bool isEnabled() { return timerEnabled; }
    
    unsigned long getTimeUntilTrigger() { return timerEnabled ? convertUnitsGet(remaining()) : timerStoppedReturnValue; }

    unsigned long getStartTime() { return startTime; }

    unsigned long getDuration() { return timerEnabled ? convertUnitsGet( duration ) : timerStoppedReturnValue; }
    
    String getUnits() { return baseUnits == SECONDS ? "Seconds" : "Milliseconds"; }
    
    unsigned long timeSinceLastReset() { return timerEnabled ? convertUnitsGet(timeSinceReset()) : timerStoppedReturnValue; }
    
    void setDisableReturnValue(unsigned long stoppedReturnValue) { timerStoppedReturnValue = stoppedReturnValue; }

    void start() {enable();}

    void stop() {disable();}

    void enable() { timerEnabled = true; }
    
    void disable() { timerEnabled = false; }
    
    void negateState() { timerEnabled = !timerEnabled; }
    
    void reset( const unsigned long newStartTime = millis() ) { resetTimer( newStartTime ); }

    static BlockNot* getFirstTimer() { return firstTimer; }

    BlockNot* getNextTimer()         { return nextTimer; }

private:
    /*
     * Private methods and variables used by the library, All calculations happen here.
     */

    unsigned long maxMillis = 0xFFFFFFFF;

    unsigned long duration = 0;

    unsigned long startTime = 0;

    unsigned long timerStoppedReturnValue = 0;

    bool timerEnabled = true;

    bool onceTriggered = false;

    bool baseUnits = MILLISECONDS;

    static BlockNot* firstTimer;
    static BlockNot* currentTimer;
    BlockNot* nextTimer;

    void resetTimer( const unsigned long newStartTime ) {
        startTime = newStartTime;
        onceTriggered = false;
    }
    
    unsigned long remaining() {
        long timePassed = timeSinceReset();
        return (timePassed < duration) ? duration - timePassed : 0;
    }

    unsigned long getNewStartTime(long offset, unsigned long now = millis()) {
        long newStartTime = now + offset;
        if (newStartTime < 0) {
            long delta = abs(newStartTime);
            return (maxMillis - delta);
        }
        else {
            return newStartTime;
        }
    }

    long timeSinceReset() {
        long mils = millis();
        if (startTime > mils) return mils + (maxMillis - startTime);
        return mils - startTime;
    }

    bool hasTriggered() { return timeSinceReset() >= duration; }

    bool hasNotTriggered() { return timeSinceReset() < duration; }
    
    unsigned long convertUnitsSet( const unsigned long timeValue ) { return (baseUnits == SECONDS) ? (timeValue * 1000) : timeValue; }
    
    unsigned long convertUnitsGet( const unsigned long timeValue ) { return (baseUnits == SECONDS) ? (timeValue / 1000) : timeValue; }

    void addToTimerList() {
        if ( firstTimer == nullptr ) {
            firstTimer = currentTimer = this;
        } else {
            currentTimer->nextTimer = this;
            currentTimer = this;
        }
        this->nextTimer = nullptr;
    }

};

void resetAllTimers( const unsigned long newStartTime = millis() ) {
    BlockNot* timer = BlockNot::getFirstTimer();
    while( timer != nullptr ) {
        timer->BlockNot::reset( newStartTime );
        timer = timer->BlockNot::getNextTimer();
    }
}

void resetAllTimers( BlockNot* timer ) { resetAllTimers( timer->BlockNot::getStartTime() ); }

BlockNot* BlockNot::firstTimer   = nullptr;
BlockNot* BlockNot::currentTimer = nullptr;

#endif
