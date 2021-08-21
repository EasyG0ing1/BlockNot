/*
 * BlockNot is a simple and easy to use Arduino class for the implementation
 * of non-blocking timers, as it is far better to use non-blocking timers
 * with a micro-controller since they allow you to trigger code at defined
 * durations of time, without stopping the execution of your main loop.
 *
 * Written by - Michael Sims
 * Full documentation can be found at: https://github.com/EasyG0ing1/BlockNot
 *
 * See LICENSE file for acceptable use conditions - this is open source
 * and there are no restrictions on its usage, I simply ask for some acknowledgment
 * if it is used in your project.
 */


#ifndef BlockNot_h
#define BlockNot_h
#include <Arduino.h>

#pragma once

#define WITH_RESET      true
#define NO_RESET        false
#define GLOBAL_RESET    true
#define SECONDS         2.2f
#define MILLISECONDS    2.1f
#define ALL             true
#define HOURS

/*
 * Macros - their usage and significance is described in README.md
 */

#define TIME_PASSED             timeSinceLastReset()
#define TIME_SINCE_RESET        timeSinceLastReset()
#define TIME_TILL_TRIGGER       timeTillTrigger()
#define ELAPSED                 timeSinceLastReset()
#define TIME_REMAINING          getTimeUntilTrigger()
#define REMAINING               getTimeUntilTrigger()
#define DURATION                getDuration()
#define DONE                    triggered()
#define GET_UNITS               getUnits()
#define TRIGGERED               triggered()
#define TRIGGERED_ON_DURATION   triggeredOnDuration()
#define TRIGGERED_ON_DURATION_ALL   triggeredOnDuration(ALL)
#define HAS_TRIGGERED           triggered(NO_RESET)
#define NOT_DONE                notTriggered()
#define NOT_TRIGGERED           notTriggered()
#define FIRST_TRIGGER           firstTrigger()
#define RESET                   reset()
#define RESET_TIMERS            resetAllTimers()
#define START                   start()
#define STOP                    stop()
#define STARTED                 running()
#define RUNNING                 running()
#define STOPPED                 stopped()
#define TOGGLE                  toggle()

class BlockNot {

public:
    /*
     * Constructors
     */
    BlockNot();
    BlockNot(unsigned long time, bool setGlobal);
    BlockNot(unsigned long time, float units = MILLISECONDS);
    BlockNot(unsigned long time, float units, bool setGlobal);

    /*
     * Public Methods
     */
    void            setDuration(const unsigned long time, bool resetOption = WITH_RESET);
    void            addTime(const unsigned long time, bool resetOption = NO_RESET);
    void            takeTime(const unsigned long time, bool resetOption = NO_RESET);
    bool            triggered(bool resetOption = true);
    bool            triggeredOnDuration(bool allMissed = false);
    unsigned long   getNextTriggerTime();
    bool            notTriggered();
    bool            firstTrigger();
    unsigned long   timeTillTrigger();
    unsigned long   getTimeUntilTrigger();
    unsigned long   getStartTime();
    unsigned long   getDuration();
    String          getUnits();
    unsigned long   timeSinceLastReset();
    void            setStoppedReturnValue(unsigned long stoppedReturnValue);
    void            start();
    void            stop();
    bool            running() {return timerRunning;};
    bool            stopped() {return !timerRunning;};
    void            toggle();
    void            switchTo(float units);
    void            reset(const unsigned long newStartTime = millis());

    static BlockNot* getFirstTimer() { return firstTimer; }
    BlockNot*       getNextTimer() { return nextTimer; }

private:
    /*
     * Private Variables
     */
    unsigned long maxMillis = 0xFFFFFFFF;
    unsigned long duration = 0;
    unsigned long startTime = 0;
    unsigned long timerStoppedReturnValue = 0;
    unsigned long lastTrueTrigger = 0;
    unsigned long millisOffset = 0;
    int totalMissedDurations = 0;
    bool timerRunning = true;
    bool onceTriggered = false;
    float baseUnits = MILLISECONDS;
    static BlockNot* firstTimer;
    static BlockNot* currentTimer;
    BlockNot* nextTimer;
    static bool global;
    bool timeWarp;

    /*
     * Private Methods
     */
    void resetTimer(const unsigned long newStartTime);
    unsigned long timeSinceReset();
    bool hasTriggered();
    bool hasNotTriggered();
    void addToTimerList();
    unsigned long remaining();
    unsigned long getDurationTriggerStartTime();
    unsigned long convertUnitsSet(const unsigned long timeValue);
    unsigned long convertUnitsGet(const unsigned long timeValue);
};

/*
 * Global methods affecting all instances of the BlockNot class.
 */
void resetAllTimers( const unsigned long newStartTime = millis() );
void resetAllTimers( BlockNot* timer );

#endif
