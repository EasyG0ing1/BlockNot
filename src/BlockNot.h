#ifndef BlockNot_h
#define BlockNot_h
#include <Arduino.h>

#pragma once

#define WITH_RESET      true
#define NO_RESET        false
#define SECONDS         true
#define MILLISECONDS    false

/*
 * Macros - their usage and significance is described in README.md
 */

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
    BlockNot();
    BlockNot(unsigned long time, bool units = MILLISECONDS);
    BlockNot(unsigned long time, unsigned long stoppedReturnValue, bool units = MILLISECONDS);
    void setDuration(const unsigned long time, bool resetOption = WITH_RESET);
    void addTime(const unsigned long time, bool resetOption = NO_RESET);
    void takeTime(const unsigned long time, bool resetOption = NO_RESET);
    bool triggered(bool resetOption = true);
    bool triggeredOnDuration();
    bool notTriggered();
    bool firstTrigger();
    bool isEnabled();
    unsigned long getTimeUntilTrigger();
    unsigned long getStartTime();
    unsigned long getDuration();
    String getUnits();
    unsigned long timeSinceLastReset();
    void setDisableReturnValue(unsigned long stoppedReturnValue);
    void start();
    void stop();
    void enable();
    void disable();
    void negateState();
    void reset(const unsigned long newStartTime = millis());
    static BlockNot* getFirstTimer() { return firstTimer; }
    BlockNot* getNextTimer() { return nextTimer; }

private:

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

    void resetTimer(const unsigned long newStartTime);
    unsigned long timeSinceReset();
    bool hasTriggered();
    bool hasNotTriggered();
    void addToTimerList();
    unsigned long remaining();
    unsigned long getNewStartTime(long offset, unsigned long now = millis());
    unsigned long convertUnitsSet(const unsigned long timeValue);
    unsigned long convertUnitsGet(const unsigned long timeValue);
};

void resetAllTimers( const unsigned long newStartTime = millis() );

void resetAllTimers( BlockNot* timer );

#endif
