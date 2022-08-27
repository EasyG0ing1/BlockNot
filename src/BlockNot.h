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

enum Unit {
    mic, mil, sec
};
enum Global {
    yes, no
};
enum State {
    running, stopped
};

#define WITH_RESET true
#define NO_RESET   false
#define ALL        true
#define SECONDS                 Unit::sec
#define MILLISECONDS            Unit::mil
#define MICROSECONDS            Unit::mic
#define NO_GLOBAL_RESET         Global::no
#define GLOBAL_RESET            Global::yes
#define RUNNING                 State::running
#define STOPPED                 State::stopped

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

    BlockNot(unsigned long time);

    BlockNot(unsigned long time, State state);

    BlockNot(unsigned long time, Unit units);

    BlockNot(unsigned long time, Unit units, State state);

    BlockNot(unsigned long time, Global globalReset);

    BlockNot(unsigned long time, State state, Global globalReset);

    BlockNot(unsigned long time, Unit units, Global globalReset);

    BlockNot(unsigned long time, Unit units, State state, Global globalReset);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, State state);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units, State state);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, Global globalReset);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, Global globalReset, State state);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units, Global globalReset);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units, Global globalReset, State state);

    /**
     * Public Methods
     */

    void setDuration(const unsigned long time, bool resetOption = WITH_RESET);

    void setDuration(const unsigned long time, Unit units, bool resetOption = WITH_RESET);

    void addTime(const unsigned long time, bool resetOption = NO_RESET);

    void takeTime(const unsigned long time, bool resetOption = NO_RESET);

    bool triggered(bool resetOption = true);

    bool triggeredOnDuration(bool allMissed = false);

    bool notTriggered();

    bool firstTrigger();

    unsigned long getNextTriggerTime();

    unsigned long getTimeUntilTrigger();

    unsigned long getStartTime();

    unsigned long getStartTime(Unit units);

    unsigned long getDuration();

    String getUnits();

    unsigned long getTimeSinceLastReset();

    void setStoppedReturnValue(unsigned long stoppedReturnValue);

    void start(bool resetOption = NO_RESET);

    void stop();

    bool isRunning();

    bool isStopped();

    void toggle();

    unsigned long convert(unsigned long value, Unit units);

    void switchTo(Unit units);

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
    int totalMissedDurations = 0;
    bool onceTriggered = false;

    union cTime {
        double seconds;

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

    static Global global;
    Unit baseUnits = MILLISECONDS;
    cTime duration;
    cTime stopTime;
    State timerState = RUNNING;

    static BlockNot *currentTimer;
    static BlockNot *firstTimer;
    BlockNot *nextTimer;

    void resetTimer(const unsigned long newStartTime);

    void initDuration(unsigned long time);

    void initDuration(unsigned long time, Unit desiredUnits);

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
