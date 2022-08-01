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


/*
 * Macros - their usage and significance is described in README.md
 */

enum Unit {mil, sec};
enum Global {yes, no};

#define WITH_RESET true
#define NO_RESET   false
#define ALL        true
#define SECONDS                 Unit::sec
#define MILLISECONDS            Unit::mil
#define NO_GLOBAL_RESET         Global::no
#define GLOBAL_RESET            Global::yes

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
#define TRIGGERED_ON_DURATION       triggeredOnDuration()
#define TRIGGERED_ON_DURATION_ALL   triggeredOnDuration(ALL)
#define HAS_TRIGGERED               triggered(NO_RESET)
#define NOT_DONE                    notTriggered()
#define NOT_TRIGGERED               notTriggered()
#define FIRST_TRIGGER               firstTrigger()
#define RESET                       reset()
#define RESET_TIMERS                resetAllTimers()
#define START                       start()
#define STOP                        stop()
#define ISSTARTED                     isRunning()
#define ISRUNNING                     isRunning()
#define ISSTOPPED                     isStopped()
#define TOGGLE                      toggle()

class BlockNot {

public:

    /*
     * Constructors
     */
    BlockNot();
    BlockNot(unsigned long time);
    BlockNot(unsigned long time, Unit units);
    BlockNot(unsigned long time, Global globalReset);
    BlockNot(unsigned long time, Unit units, Global globalReset);
    BlockNot(unsigned long time, unsigned long stoppedReturnValue);
    BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units);
    BlockNot(unsigned long time, unsigned long stoppedReturnValue, Global globalReset);
    BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units, Global globalReset);

    /*
     * Public Methods
     */
    void            setDuration(const unsigned long time, bool resetOption = WITH_RESET);
    void            addTime(const unsigned long time, bool resetOption = NO_RESET);
    void            takeTime(const unsigned long time, bool resetOption = NO_RESET);
    bool            triggered(bool resetOption = true);
    bool            triggeredOnDuration(bool allMissed = false);
    bool            notTriggered();
    bool            firstTrigger();
    unsigned long   getNextTriggerTime();
    unsigned long   getTimeUntilTrigger();
    unsigned long   getStartTime();
    unsigned long   getDuration();
    String          getUnits();
    unsigned long   getTimeSinceLastReset();
    void            setStoppedReturnValue(unsigned long stoppedReturnValue);
    void            start();
    void            stop();
    bool            isRunning();
    bool            isStopped();
    void            toggle();
    void            switchTo(Unit units);
    void            reset(const unsigned long newStartTime = millis());

    static BlockNot* getFirstTimer() { return firstTimer; }
    BlockNot*       getNextTimer() { return nextTimer; }

private:
    /*
     * Private Variables and Methods
     */
    unsigned long startTime = 0;
    unsigned long timerStoppedReturnValue = 0;
    int totalMissedDurations = 0;
    bool timerRunning = true;
    bool onceTriggered = false;
    union cTime {
        double seconds;
        class milli_t {
            double seconds;
        public:
            milli_t &operator=(double ms) {
                seconds = ms/1000.0;
                return *this; }
                operator double() const { return seconds * 1000; }
        } millis;
    };
    static Global global;
    Unit baseUnits = MILLISECONDS;
    cTime duration;

    static BlockNot* currentTimer;
    static BlockNot* firstTimer;
    BlockNot* nextTimer;

    void resetTimer(const unsigned long newStartTime);
    unsigned long timeSinceReset();
    bool hasTriggered();
    bool hasNotTriggered();
    void addToTimerList();
    unsigned long timeTillTrigger();
    unsigned long remaining();
    unsigned long getDurationTriggerStartTime();
    unsigned long convertUnits(cTime timeValue);
};

/*
 * Global methods affecting all instances of the BlockNot class.
 */
void resetAllTimers( const unsigned long newStartTime = millis() );
void resetAllTimers( BlockNot* timer );

#endif
