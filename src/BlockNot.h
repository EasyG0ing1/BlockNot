/**
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

/**
 * Macros - their usage and significance is described in README.md
 */

enum BlockNotUnit {
    mic, mil, sec
};
enum BlockNotGlobal {
    yes, no
};
enum BlockNotState {
    running, stopped
};

#define WITH_RESET true
#define NO_RESET   false
#define ALL        true
#define SECONDS                 BlockNotUnit::sec
#define MILLISECONDS            BlockNotUnit::mil
#define MICROSECONDS            BlockNotUnit::mic
#define NO_GLOBAL_RESET         BlockNotGlobal::no
#define GLOBAL_RESET            BlockNotGlobal::yes
#define RUNNING                 BlockNotState::running
#define STOPPED                 BlockNotState::stopped

#define TIME_PASSED                 getTimeSinceLastReset()
#define TIME_SINCE_RESET            getTimeSinceLastReset()
#define ELAPSED                     getTimeSinceLastReset()
#define TIME_TILL_TRIGGER           getTimeUntilTrigger()
#define TIME_REMAINING              getTimeUntilTrigger()
#define REMAINING                   getTimeUntilTrigger()
#define DURATION                    getDuration()
#define GET_UNITS                   getUnits()
#define GET_START_TIME              getStartTime()
#define DONE                        triggered()
#define TRIGGERED                   triggered()
#define LAST_TRIGGER_DURATION       lastTriggerDuration()
#define HAS_TRIGGERED               triggered(NO_RESET)
#define TRIGGERED_ON_DURATION       triggeredOnDuration()
#define TRIGGERED_ON_MARK           triggeredOnDuration()
#define TRIGGERED_ON_DURATION_ALL   triggeredOnDuration(ALL)
#define TRIGGERED_ALL               triggeredOnDuration(ALL)
#define NOT_DONE                    notTriggered()
#define NOT_TRIGGERED               notTriggered()
#define FIRST_TRIGGER               firstTrigger()
#define RESET                       reset()
#define RESET_TIMERS                resetAllTimers()
#define START                       start()
#define START_RESET                 start(WITH_RESET)
#define STOP                        stop()
#define ISSTARTED                   isRunning()
#define ISRUNNING                   isRunning()
#define ISSTOPPED                   isStopped()
#define TOGGLE                      toggle()

class BlockNot {

public:

    /**
     * Constructors
     */
    BlockNot();

    BlockNot(unsigned long milliseconds);

    BlockNot(unsigned long milliseconds, BlockNotState state);

    BlockNot(unsigned long time, BlockNotUnit units);

    BlockNot(unsigned long time, BlockNotUnit units, BlockNotState state);

    BlockNot(unsigned long milliseconds, BlockNotGlobal globalReset);

    BlockNot(unsigned long milliseconds, BlockNotState state, BlockNotGlobal globalReset);

    BlockNot(unsigned long time, BlockNotUnit units, BlockNotGlobal globalReset);

    BlockNot(unsigned long time, BlockNotUnit units, BlockNotState state, BlockNotGlobal globalReset);

    BlockNot(unsigned long milliseconds, unsigned long stoppedReturnValue);

    BlockNot(unsigned long milliseconds, unsigned long stoppedReturnValue, BlockNotState state);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, BlockNotUnit units);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, BlockNotUnit units, BlockNotState state);

    BlockNot(unsigned long milliseconds, unsigned long stoppedReturnValue, BlockNotGlobal globalReset);

    BlockNot(unsigned long milliseconds, unsigned long stoppedReturnValue, BlockNotGlobal globalReset, BlockNotState state);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, BlockNotUnit units, BlockNotGlobal globalReset);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, BlockNotUnit units, BlockNotGlobal globalReset, BlockNotState state);

    /**
     * Public Methods
     */

    void setDuration(const unsigned long time, bool resetOption = WITH_RESET);

    void setDuration(const unsigned long time, BlockNotUnit units, bool resetOption = WITH_RESET);

    void addTime(const unsigned long time, bool resetOption = NO_RESET);

    void takeTime(const unsigned long time, bool resetOption = NO_RESET);

    bool triggered(bool resetOption = true);

    bool triggeredOnDuration(bool allMissed = false);

    bool notTriggered();

    bool firstTrigger();

    void setFirstTriggerResponse(bool response);

    unsigned long getNextTriggerTime();

    unsigned long getTimeUntilTrigger();

    unsigned long getStartTime();

    unsigned long getStartTime(BlockNotUnit units);

    unsigned long getDuration();

    unsigned long lastTriggerDuration();

    String getUnits();

    unsigned long getTimeSinceLastReset();

    void setStoppedReturnValue(unsigned long stoppedReturnValue);

    void start(bool resetOption = NO_RESET);

    void stop();

    bool isRunning();

    bool isStopped();

    void toggle();

    unsigned long convert(unsigned long value, BlockNotUnit units);

    void switchTo(BlockNotUnit units);

    void reset(const unsigned long newStartTime = 0);

    void setMillisOffset(unsigned long offset = 0);

    void setMicrosOffset(unsigned long offset = 0);

    unsigned long getMillis();

    static BlockNot *getFirstTimer() { return firstTimer; }

    BlockNot *getNextTimer() { return nextTimer; }

private:
    /**
     * Private Variables and Methods
     */
    unsigned long startTime;
    unsigned long millisOffset = 0;
    unsigned long microsOffset = 0;
    unsigned long timerStoppedReturnValue = 0;
    unsigned long lastDuration;
    int totalMissedDurations = 0;
    bool onceTriggered = false;
    bool firstTriggerResponse = false;

    union cTime {
        double seconds = 0.0;

        class milli_t {
            double seconds;
        public:
            milli_t &operator=(double ms) {
                seconds = ms * .001;
                return *this;
            }
            operator double() const {
                return seconds * 1000.0;
            }
        } millis;

        class micro_t {
            double seconds;
        public:
            micro_t &operator=(double us) {
                seconds = us * .000001;
                return *this;
            }
            operator double() const {
                return seconds * 1000000.0;
            }
        } micros;
    };

    static BlockNotGlobal global;
    BlockNotUnit baseUnits = MILLISECONDS;
    cTime duration;
    cTime stopTime;
    BlockNotState timerState = RUNNING;

    static BlockNot *currentTimer;
    static BlockNot *firstTimer;
    BlockNot *nextTimer;

    void resetTimer(const unsigned long newStartTime);

    void initDuration(unsigned long time);

    void initDuration(unsigned long time, BlockNotUnit desiredUnits);

    unsigned long timeSinceReset();

    bool hasTriggered();

    bool hasNotTriggered();

    void addToTimerList();

    unsigned long timeTillTrigger();

    unsigned long remaining();

    unsigned long getDurationTriggerStartTime();

    unsigned long convertUnits(cTime timeValue);
};

/**
 * Global methods affecting all instances of the BlockNot class.
 */
void resetAllTimers(const unsigned long newStartTime = micros());

void resetAllTimers(BlockNot *timer);

#endif
