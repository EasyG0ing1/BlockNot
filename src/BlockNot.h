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
    mic_cTime, mil_cTime, sec_cTime, min_cTime
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
#define MICROSECONDS            BlockNotUnit::mic_cTime
#define MILLISECONDS            BlockNotUnit::mil_cTime
#define SECONDS                 BlockNotUnit::sec_cTime
#define MINUTES                 BlockNotUnit::min_cTime
#define NO_GLOBAL_RESET         BlockNotGlobal::no
#define GLOBAL_RESET            BlockNotGlobal::yes
#define RUNNING                 BlockNotState::running
#define STOPPED                 BlockNotState::stopped

#define ELAPSED                     getTimeSinceLastReset()
#define REMAINING                   getTimeUntilTrigger()
#define DURATION                    getDuration()
#define GET_UNITS                   getUnits()
#define GET_START_TIME              getStartTime()

#ifdef __GNUC__ // GCC or Clang
    #define DONE triggered()
    #warning "DONE is deprecated and will be removed in a future release. Use TRIGGERED instead."
    #define TIME_PASSED getTimeSinceLastReset()
    #warning "WARNING: TIME_PASSED is deprecated and will be removed in a future release. Use ELAPSED instead."
    #define TIME_SINCE_RESET getTimeSinceLastReset()
    #warning "WARNING: TIME_SINCE_RESET is deprecated and will be removed in a future release. Use ELAPSED instead."
    #define TIME_TILL_TRIGGER getTimeUntilTrigger()
    #warning "WARNING: TIME_TILL_TRIGGER is deprecated and will be removed in a future release. Use REMAINING instead."
    #define TRIGGERED_ON_MARK triggeredOnDuration()
    #warning "WARNING: TRIGGERED_ON_MARK is deprecated and will be removed in a future release. Use TRIGGERED_ON_DURATION instead."
    #define NOT_DONE notTriggered()
    #warning "WARNING: NOT_DONE is deprecated and will be removed in a future release. Use NOT_TRIGGERED instead."
    #define ISSTARTED isRunning()
    #warning "WARNING: ISSTARTED is deprecated and will be removed in a future release. Use ISRUNNING instead."
    #define TRIGGERED_ON_DURATION_ALL triggeredOnDuration(ALL)
    #warning "WARNING: TRIGGERED_ON_DURATION_ALL is deprecated and will be removed in a future release. Use TRIGGERED_ON_DURATION(ALL) instead."
    #define TRIGGERED_ALL triggeredOnDuration(ALL)
    #warning "WARNING: TRIGGERED_ALL is deprecated and will be removed in a future release. Use TRIGGERED_ON_DURATION(ALL) instead."
    #define START_RESET start(WITH_RESET)
    #warning "WARNING: START_RESET is deprecated and will be removed in a future release. Use START(WITH_RESET) instead."
#elif defined(_MSC_VER) // Microsoft Visual Studio
    #define DONE (__pragma(message("WARNING: DONE is deprecated and will be removed in a future release. Use TRIGGERED instead.")), triggered())
    #define TIME_PASSED (__pragma(message("WARNING: TIME_PASSED is deprecated and will be removed in a future release. Use ELAPSED instead.")), getTimeSinceLastReset())
    #define TIME_SINCE_RESET (__pragma(message("WARNING: TIME_SINCE_RESET is deprecated and will be removed in a future release. Use ELAPSED instead.")), getTimeSinceLastReset())
    #define TIME_TILL_TRIGGER (__pragma(message("WARNING: TIME_TILL_TRIGGER is deprecated and will be removed in a future release. Use REMAINING instead.")), getTimeUntilTrigger())
    #define TRIGGERED_ON_MARK (__pragma(message("WARNING: TRIGGERED_ON_MARK is deprecated and will be removed in a future release. Use TRIGGERED_ON_DURATION instead.")), triggeredOnDuration())
    #define TRIGGERED_ON_DURATION_ALL (__pragma(message("WARNING: TRIGGERED_ON_DURATION_ALL is deprecated and will be removed in a future release. Use TRIGGERED_ON_DURATION(ALL) instead.")), triggeredOnDuration(ALL))
    #define TRIGGERED_ALL (__pragma(message("WARNING: TRIGGERED_ALL is deprecated and will be removed in a future release. Use TRIGGERED_ON_DURATION(ALL) instead.")), triggeredOnDuration(ALL))
    #define NOT_DONE (__pragma(message("WARNING: NOT_DONE is deprecated and will be removed in a future release. Use NOT_TRIGGERED instead.")), notTriggered())
    #define START_RESET (__pragma(message("WARNING: START_RESET is deprecated and will be removed in a future release. Use START(WITH_RESET) instead.")), start(WITH_RESET))
    #define ISSTARTED (__pragma(message("WARNING: ISSTARTED is deprecated and will be removed in a future release. Use ISRUNNING instead.")), isRunning())
#else
    #define DONE triggered()
    #define TIME_PASSED getTimeSinceLastReset()
    #define TIME_SINCE_RESET getTimeSinceLastReset()
    #define TIME_TILL_TRIGGER getTimeUntilTrigger()
    #define TRIGGERED_ON_MARK triggeredOnDuration()
    #define NOT_DONE notTriggered()
    #define ISSTARTED isRunning()
    #define TRIGGERED_ON_DURATION_ALL triggeredOnDuration(ALL)
    #define TRIGGERED_ALL triggeredOnDuration(ALL)
    #define START_RESET start(WITH_RESET)
#endif

#define TRIGGERED                   triggered()
#define LAST_TRIGGER_DURATION       lastTriggerDuration()
#define HAS_TRIGGERED               triggered(NO_RESET)
#define TRIGGER_NEXT                triggerNext()
#define TRIGGERED_ON_DURATION(...)  triggeredOnDuration(__VA_ARGS__)
#define NOT_TRIGGERED               notTriggered()
#define FIRST_TRIGGER               firstTrigger()
#define RESET                       reset()
#define RESET_TIMERS                resetAllTimers()
#define START(...)                  start(__VA_ARGS__)
#define STOP                        stop()
#define ISRUNNING                   isRunning()
#define ISSTOPPED                   isStopped()
#define TOGGLE                      toggle()

class BlockNot {
#define TIME_PASSED getTimeSinceLastReset()

public:
    /**
     * Constructors
     */
    BlockNot();

    explicit BlockNot(unsigned long milliseconds);

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

    BlockNot(unsigned long milliseconds, unsigned long stoppedReturnValue, BlockNotGlobal globalReset,
             BlockNotState state);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, BlockNotUnit units, BlockNotGlobal globalReset);

    BlockNot(unsigned long time, unsigned long stoppedReturnValue, BlockNotUnit units, BlockNotGlobal globalReset,
             BlockNotState state);

    /**
     * Public Methods
     */

    void setDuration(unsigned long time, bool resetOption = WITH_RESET);

    void setDuration(unsigned long time, BlockNotUnit units, bool resetOption = WITH_RESET);

    void addTime(unsigned long time, bool resetOption = NO_RESET);

    void takeTime(unsigned long time, bool resetOption = NO_RESET);

    bool triggered(bool resetOption = true);

    bool triggeredOnDuration(bool allMissed = false);

    bool notTriggered();

    bool firstTrigger();

    void triggerNext();

    void setFirstTriggerResponse(bool response);

    unsigned long getNextTriggerTime() const;

    unsigned long getTimeUntilTrigger() const;

    unsigned long getStartTime() const;

    unsigned long getStartTime(BlockNotUnit units) const;

    unsigned long getDuration() const;

    unsigned long lastTriggerDuration() const;

    String getUnits() const;

    unsigned long getTimeSinceLastReset() const;

    void setStoppedReturnValue(unsigned long stoppedReturnValue);

    void start(bool resetOption = NO_RESET);

    void stop();

    bool isRunning() const;

    bool isStopped() const;

    void toggle();

    unsigned long convert(unsigned long value, BlockNotUnit units) const;

    void switchTo(BlockNotUnit units);

    void reset(unsigned long newStartTime = 0);

    void setMillisOffset(unsigned long offset = 0);

    void setMicrosOffset(unsigned long offset = 0);

    void speedComp(unsigned long time);

    void disableSpeedComp();

    unsigned long getMillis() const;

    BlockNotUnit getBaseUnits() const;

    static void getHelp(Print &output, bool haltCode = false);

    static void getHelp(bool haltCode = false);

    class cTime {
    public:
        double seconds = 0.0; // Central storage for time in seconds

        // Class for milliseconds
        class milli_t {
            double &seconds;

        public:
            milli_t(double &s) : seconds(s) {
            }

            milli_t &operator=(double ms) {
                seconds = ms * 0.001; // Convert milliseconds to seconds
                return *this;
            }

            operator double() const {
                return seconds * 1000.0; // Convert seconds to milliseconds
            }
        };

        // Class for microseconds
        class micro_t {
            double &seconds;

        public:
            micro_t(double &s) : seconds(s) {
            }

            micro_t &operator=(double us) {
                seconds = us * 0.000001; // Convert microseconds to seconds
                return *this;
            }

            operator double() const {
                return seconds * 1000000.0; // Convert seconds to microseconds
            }
        };

        // Class for minutes
        class minutes_t {
            double &seconds;

        public:
            minutes_t(double &s) : seconds(s) {
            }

            minutes_t &operator=(double mins) {
                seconds = mins * 60.0; // Convert minutes to seconds
                return *this;
            }

            operator double() const {
                return seconds / 60.0; // Convert seconds to minutes
            }
        };

        // Accessors for helper classes

        milli_t millis;
        micro_t micros;
        minutes_t minutes;

        // Constructor
        cTime() : millis(seconds), micros(seconds), minutes(seconds) {
        }

        // Getter for seconds
        double getSeconds() const { return seconds; }

        // Setter for seconds
        void setSeconds(double s) { seconds = s; }
    };

    static BlockNot *firstTimer;
    static BlockNot *currentTimer;
    BlockNot *nextTimer;

private:
    /**
     * Private Variables and Methods
     */
    unsigned long startTime;
    unsigned long millisOffset;
    unsigned long microsOffset;
    unsigned long timerStoppedReturnValue;
    unsigned long lastDuration;
    int totalMissedDurations;
    bool onceTriggered;
    bool triggerOnNext;
    bool firstTriggerResponse;
    bool speedCompensation;
    unsigned long compTime;
    unsigned long newStartTimeMillis;
    unsigned long newStartTimeMicros;

    static BlockNotGlobal global;
    BlockNotUnit baseUnits;
    cTime duration;
    cTime stopTime;
    BlockNotState timerState;

    void resetTimer(unsigned long newStartTime);

    void initDuration(unsigned long time);

    void initDuration(unsigned long time, BlockNotUnit desiredUnits);

    unsigned long timeSinceReset() const;

    bool hasTriggered();

    bool hasNotTriggered() const;

    void addToTimerList();

    unsigned long timeTillTrigger() const;

    unsigned long remaining() const;

    unsigned long getDurationTriggerStartTime() const;

    unsigned long convertUnits(const cTime &timeValue) const;
};

/**
 * Global methods affecting all instances of the BlockNot class.
 */
void resetAllTimers(unsigned long newStartTime = 0);

#endif
