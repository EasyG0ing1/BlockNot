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

#include <BlockNot.h>

/**
 * Global Variables
 */

BlockNot *BlockNot::firstTimer = nullptr;
BlockNot *BlockNot::currentTimer = nullptr;
BlockNotGlobal BlockNot::global = GLOBAL_RESET;

/**
 * Constructors
 */

BlockNot::BlockNot() {
    baseUnits = MILLISECONDS;
    timerState = RUNNING;
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long milliseconds) {
    baseUnits = MILLISECONDS;
    timerState = RUNNING;
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    initDuration(milliseconds);
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long milliseconds, const BlockNotState state) {
    baseUnits = MILLISECONDS;
    timerState = state;
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    if(timerState == STOPPED)
        stop();
    initDuration(milliseconds);
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long time, const BlockNotUnit units = MILLISECONDS) {
    baseUnits = units;
    timerState = RUNNING;
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    initDuration(time);
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long time, const BlockNotUnit units, const BlockNotState state) {
    baseUnits = units;
    timerState = state;
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    if(timerState == STOPPED)
        stop();
    initDuration(time);
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long milliseconds, const BlockNotGlobal globalReset) {
    baseUnits = MILLISECONDS;
    timerState = RUNNING;
    initDuration(milliseconds);
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long milliseconds, const BlockNotState state, const BlockNotGlobal globalReset) {
    baseUnits = MILLISECONDS;
    timerState = state;
    if(timerState == STOPPED)
        stop();
    initDuration(milliseconds);
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long time, const BlockNotUnit units, const BlockNotGlobal globalReset) {
    baseUnits = units;
    timerState = RUNNING;
    initDuration(time);
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long time, const BlockNotUnit units, const BlockNotState state, const BlockNotGlobal globalReset) {
    baseUnits = units;
    timerState = state;
    if(timerState == STOPPED)
        stop();
    initDuration(time);
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long milliseconds, const unsigned long stoppedReturnValue) {
    baseUnits = MILLISECONDS;
    timerState = RUNNING;
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    initDuration(milliseconds);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long milliseconds, const unsigned long stoppedReturnValue, const BlockNotState state) {
    baseUnits = MILLISECONDS;
    timerState = state;
    if(timerState == STOPPED)
        stop();
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    initDuration(milliseconds);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();

}

BlockNot::BlockNot(const unsigned long time, const unsigned long stoppedReturnValue, const BlockNotUnit units) {
    baseUnits = units;
    timerState = RUNNING;
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long time, const unsigned long stoppedReturnValue, const BlockNotUnit units, const BlockNotState state) {
    baseUnits = units;
    timerState = state;
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    if(timerState == STOPPED)
        stop();
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long milliseconds, const unsigned long stoppedReturnValue, const BlockNotGlobal globalReset) {
    baseUnits = MILLISECONDS;
    timerState = RUNNING;
    initDuration(milliseconds);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long milliseconds, const unsigned long stoppedReturnValue, const BlockNotGlobal globalReset, const BlockNotState state) {
    baseUnits = MILLISECONDS;
    timerState = state;
    if(timerState == STOPPED)
        stop();
    initDuration(milliseconds);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long time, const unsigned long stoppedReturnValue, const BlockNotUnit units, const BlockNotGlobal globalReset) {
    baseUnits = units;
    timerState = RUNNING;
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(const unsigned long time, const unsigned long stoppedReturnValue, const BlockNotUnit units, const BlockNotGlobal globalReset, const BlockNotState state) {
    baseUnits = units;
    timerState = state;
    if(timerState == STOPPED)
        stop();
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

/**
 * Public Methods
 */

void BlockNot::setDuration(const unsigned long time, const bool resetOption) {
    initDuration(time);
    if (resetOption) reset();
}

void BlockNot::setDuration(const unsigned long time, BlockNotUnit inUnits, const bool resetOption) {
    initDuration(time, inUnits);
    if (resetOption) reset();
}

void BlockNot::addTime(const unsigned long time, const bool resetOption) {
    unsigned long newDuration;
    switch(baseUnits) {
        case MICROSECONDS: {
            newDuration = duration.micros + time;
            if (newDuration < duration.micros) newDuration = 0xFFFFFFFFL;
            duration.micros = newDuration;
            break;
        }
        default: {
            newDuration = duration.millis + time;
            if (newDuration < duration.millis) newDuration = 0xFFFFFFFFL;
            duration.millis = newDuration;
            break;
        }
    }
    if (resetOption) reset();
}

void BlockNot::takeTime(const unsigned long time, const bool resetOption) {
    long newDuration;
    switch(baseUnits) {
        case MICROSECONDS: {
            newDuration = duration.micros - time;
            if (newDuration > duration.micros) newDuration = 0L;
            duration.micros = newDuration;
            break;
        }
        default: {
            newDuration = duration.millis - time;
            if (newDuration > duration.millis) newDuration = 0L;
            duration.millis = newDuration;
            break;
        }
    }
    if (resetOption) reset();
}

bool BlockNot::triggered(const bool resetOption) {
    const bool triggered = hasTriggered();
    if (resetOption && triggered) {
        reset();
    }
    return timerState == RUNNING && triggered;
}

bool BlockNot::triggeredOnDuration(const bool allMissed) {
    const bool triggered = hasTriggered();
    if (triggered) {
        unsigned long missedDurations;
        switch(baseUnits) {
            case MICROSECONDS: {
                missedDurations = timeSinceReset() / static_cast<unsigned long>(duration.micros);
                break;
            }
            default: {
                missedDurations = timeSinceReset() / static_cast<unsigned long>(duration.millis);
                break;
            }
        }
        totalMissedDurations += (allMissed ? missedDurations : 0);
        const unsigned long newStartTime = getDurationTriggerStartTime();
        reset(newStartTime);
    }
    if (totalMissedDurations > 0 && allMissed) {
        totalMissedDurations--;
        return true;
    }
    return triggered;
}

bool BlockNot::notTriggered() {
    return timerState == RUNNING && !hasTriggered();
}

bool BlockNot::firstTrigger() {
    if(onceTriggered) {
        return firstTriggerResponse;
    }
    if (hasTriggered()) {
        onceTriggered = true;
        return timerState == RUNNING;
    }
    return false;
}

void BlockNot::triggerNext() {
    triggerOnNext = true;
}

void BlockNot::setFirstTriggerResponse(const bool response) {
    firstTriggerResponse = response;
}

unsigned long BlockNot::getNextTriggerTime() const {
    cTime nextTrigger;
    if (triggerOnNext) {
        nextTrigger.micros = micros();
        nextTrigger.millis = millis();
    }
    else {
        switch(baseUnits) {
            case MICROSECONDS: {
                nextTrigger.micros = startTime + duration.micros;
                break;
            }
            default: {
                nextTrigger.millis = startTime + duration.millis;
                break;
            }
        }
    }
    return convertUnits(nextTrigger);
}

unsigned long BlockNot::getTimeUntilTrigger() const {
    return timeTillTrigger();
}

unsigned long BlockNot::getStartTime() const {
    cTime sTime;
    switch(baseUnits) {
        case MICROSECONDS: {
            sTime.micros = startTime;
            break;
        }
        default: {
            sTime.millis = startTime;
            break;
        }
    }
    return convertUnits(sTime);
}

unsigned long BlockNot::getStartTime(const BlockNotUnit units) const {
    cTime timeValue;
    timeValue.micros = startTime;
    switch(units) {
        case MINUTES:
            return timeValue.minutes;
        case SECONDS:
            return timeValue.seconds;
        case MILLISECONDS:
            return timeValue.millis;
        case MICROSECONDS:
            return timeValue.micros;
    }
    return 0L;
}

unsigned long BlockNot::getDuration() const {
    return timerState == RUNNING ? convertUnits(duration) : timerStoppedReturnValue;
}

unsigned long BlockNot::lastTriggerDuration() const {
    return lastDuration;
}

String BlockNot::getUnits() const {
    return (baseUnits == SECONDS) ? "Seconds" : (baseUnits == MILLISECONDS) ?  "Milliseconds" : "Microseconds";
}

unsigned long BlockNot::getTimeSinceLastReset() const {
    cTime timeLapsed;
    switch(baseUnits) {
        case MICROSECONDS: {
            timeLapsed.micros = timeSinceReset();
            break;
        }
        default: {
            timeLapsed.millis = timeSinceReset();
            break;
        }
    }
    return (timerState == RUNNING) ? convertUnits(timeLapsed) : timerStoppedReturnValue;
}

void BlockNot::setStoppedReturnValue(const unsigned long stoppedReturnValue) {
    timerStoppedReturnValue = stoppedReturnValue;
}

void BlockNot::start(const bool resetOption) {
    if(resetOption)
        reset();
    else {
        switch(baseUnits) {
            case MICROSECONDS: {
                startTime = micros() - stopTime.micros;
                break;
            }
            default: {
                startTime = millis() - stopTime.millis;
                break;
            }
        }
    }
    timerState = RUNNING;
}

void BlockNot::stop() {
    timerState = STOPPED;
    switch(baseUnits) {
        case MICROSECONDS: {
            stopTime.micros = micros();
            break;
        }
        default: {
            stopTime.millis = millis();
            break;
        }
    }
}

bool BlockNot::isRunning() const {return timerState == RUNNING;}

bool BlockNot::isStopped() const {return timerState == STOPPED;}

void BlockNot::toggle() {
    if(timerState == RUNNING)
        timerState = STOPPED;
    else
        timerState = RUNNING;
}

unsigned long BlockNot::convert(const unsigned long value, const BlockNotUnit units) const {
    cTime timeValue;
    unsigned long result = 0L;
    switch(baseUnits) {
        case MINUTES: {
            timeValue.minutes = value;
            break;
        }
        case SECONDS: {
            timeValue.seconds = value;
            break;
        }
        case MILLISECONDS: {
            timeValue.millis = value;
            break;
        }
        case MICROSECONDS: {
            timeValue.micros = value;
            break;
        }
    }
    switch(units) {
        case MINUTES: {
            result= timeValue.minutes;
            break;
        }
        case SECONDS: {
            result = timeValue.seconds;
            break;
        }
        case MILLISECONDS: {
            result = timeValue.millis;
            break;
        }
        case MICROSECONDS: {
            result = timeValue.micros;
            break;
        }
    }
    return result;
}

void BlockNot::switchTo(const BlockNotUnit units) { baseUnits = units; }

void BlockNot::reset(const unsigned long newStartTime) {
    unsigned long finalStartTime = newStartTime;
    if(finalStartTime == 0) {
        switch(baseUnits) {
            case MICROSECONDS: {
                finalStartTime = micros() + microsOffset;
                break;
            }
            default: {
                finalStartTime = millis() + millisOffset;
                if (speedCompensation)
                    delay(compTime);
                break;
            }
        }
    }
    resetTimer(finalStartTime);
}

void BlockNot::setMillisOffset(const unsigned long offset) {
    long delta = offset - millisOffset;
    startTime = startTime + delta;
    millisOffset = offset;
}

void BlockNot::setMicrosOffset(const unsigned long offset) {
    microsOffset = offset;
}

void BlockNot::speedComp(const unsigned long time) {
    speedCompensation = true;
    compTime = time;
}

void BlockNot::disableSpeedComp() {
    speedCompensation = false;
}

unsigned long BlockNot::getMillis() const {
    return millis() + millisOffset;
}

BlockNotUnit BlockNot::getBaseUnits() const {
    return baseUnits;
}

void BlockNot::getHelp(Print &output, const bool haltCode) {
    output.println("\n\nThe following macros can be used for coding simplicity and to produce more readable code:\n");
    output.println("Macro\t\t\t\tMethod Called");
    output.println("-------------------------------------------------------");
    output.println("ELAPSED\t\t\t\tgetTimeSinceLastReset()");
    output.println("REMAINING\t\t\tgetTimeUntilTrigger()");
    output.println("DURATION\t\t\tgetDuration()");
    output.println("GET_UNITS\t\t\tgetUnits()");
    output.println("GET_START_TIME\t\t\tgetStartTime()");
    output.println("TRIGGERED\t\t\ttriggered()");
    output.println("LAST_TRIGGER_DURATION\t\tlastTriggerDuration()");
    output.println("HAS_TRIGGERED\t\t\ttriggered(NO_RESET)");
    output.println("TRIGGER_NEXT\t\t\ttriggerNext()");
    output.println("TRIGGERED_ON_DURATION\t\ttriggeredOnDuration");
    output.println("TRIGGERED_ON_DURATION(ALL)\ttriggeredOnDuration(ALL)");
    output.println("NOT_TRIGGERED\t\t\tnotTriggered()");
    output.println("FIRST_TRIGGER\t\t\tfirstTrigger()");
    output.println("RESET\t\t\t\treset()");
    output.println("RESET_TIMERS\t\t\tresetAllTimers()");
    output.println("START\t\t\t\tstart()");
    output.println("START(WITH_RESET)\t\tstart(WITH_RESET)");
    output.println("STOP\t\t\t\tstop()");
    output.println("ISRUNNING\t\t\tisRunning()");
    output.println("ISSTOPPED\t\t\tisStopped()");
    output.println("TOGGLE\t\t\t\ttoggle()");
    output.println("\nYou use macros like you would a method call only no neeed for passing arguments unless the macro");
    output.println("explicitely supports it:\n");
    output.println("if (myTimer.TRIGGERED) {");
    output.println("\t//My Code");
    output.println("}");
    output.println(" ");
    output.println(" ");
    if (haltCode) {
        while(true){}
    }
}

void BlockNot::getHelp(const bool haltCode) {
    getHelp(Serial, haltCode);
}

/**
 * Private Methods
 */

void BlockNot::initDuration(const unsigned long time) {
    switch(baseUnits) {
        case MINUTES: {
            duration.minutes = time;
            break;
        }
        case SECONDS: {
            duration.seconds = time;
            break;
        }
        case MILLISECONDS: {
            duration.millis = time;
            break;
        }
        case MICROSECONDS: {
            duration.micros = time;
            break;
        }
    }
}

void BlockNot::initDuration(const unsigned long time, const BlockNotUnit inUnits) {
    switch(inUnits) {
        case MICROSECONDS:
            duration.micros = time;
            break;
        case MILLISECONDS:
            duration.millis = time;
            break;
        case SECONDS:
            duration.seconds = time;
            break;
        case MINUTES: {
            duration.minutes = time;
            break;
        }
    }
}

void BlockNot::resetTimer(const unsigned long newStartTime) {
    startTime = newStartTime;
    triggerOnNext = false;
    onceTriggered = false;
}

unsigned long BlockNot::timeSinceReset() const {
    unsigned long result;
    unsigned long millisBase = millisOffset + millis();
    switch(baseUnits) {
        case MICROSECONDS: {
            result = microsOffset + micros() - startTime;
            break;
        }
        default: {
            result = millisBase - startTime;
            break;
        }
    }
    return result;
}

bool BlockNot::hasTriggered() {
    if (triggerOnNext) {
        triggerOnNext = false;
        return true;
    }
    bool triggered;
    const unsigned long sinceReset = timeSinceReset();
    switch(baseUnits) {
        case MICROSECONDS: {
            triggered = sinceReset >= static_cast<unsigned long>(duration.micros);
            break;
        }
        default: {
            triggered = sinceReset >= static_cast<unsigned long>(duration.millis);
            break;
        }
    }
    if(triggered)
        lastDuration = sinceReset;
    return triggered;
}

bool BlockNot::hasNotTriggered() const {
    bool notTriggered;
    switch(baseUnits) {
        case MICROSECONDS:
            notTriggered = timeSinceReset() < static_cast<unsigned long>(duration.micros);
            break;
        default:
            notTriggered = timeSinceReset() < static_cast<unsigned long>(duration.millis);
            break;
    }
    return notTriggered;
}

unsigned long BlockNot::timeTillTrigger() const {
    const unsigned long sinceReset = timeSinceReset();
    unsigned long tillTrigger = 0L;
    if (!triggerOnNext) {
        cTime triggerTime;
        switch(baseUnits) {
            case MICROSECONDS: {
                triggerTime.micros = (sinceReset < duration.micros) ? static_cast<unsigned long>(duration.micros - sinceReset) : 0L;
                tillTrigger = (timerState == RUNNING) ? convertUnits(triggerTime) : timerStoppedReturnValue;
                break;
            }
            default: {
                triggerTime.millis = (sinceReset < duration.millis) ? static_cast<unsigned long>(duration.millis - sinceReset) : 0L;
                tillTrigger = (timerState == RUNNING) ? convertUnits(triggerTime) : timerStoppedReturnValue;
                break;
            }
        }
    }
    return tillTrigger;
}

unsigned long BlockNot::remaining() const {
    const unsigned long timePassed = timeSinceReset();
    unsigned long remain = 0L;
    if (!triggerOnNext) {
        switch(baseUnits) {
            case MICROSECONDS: {
                remain = (timePassed < duration.micros) ? (static_cast<unsigned long>(duration.micros) - timePassed) : 0;
                break;
            }
            default: {
                remain = (timePassed < duration.millis) ? (static_cast<unsigned long>(duration.millis) - timePassed) : 0;
                break;
            }
        }
    }
    return remain;
}

unsigned long BlockNot::getDurationTriggerStartTime() const {
    unsigned long durationStartTime;
    switch(baseUnits) {
        case MICROSECONDS: {
            durationStartTime = startTime + ((timeSinceReset() / static_cast<unsigned long>(duration.micros)) * static_cast<unsigned long>(duration.micros));
            break;
        }
        default: {
            durationStartTime = startTime + ((timeSinceReset() / static_cast<unsigned long>(duration.millis)) * static_cast<unsigned long>(duration.millis));
            break;
        }
    }
    return durationStartTime;
}

unsigned long BlockNot::convertUnits(const cTime &timeValue) const {
    return baseUnits == MINUTES ? timeValue.minutes :
           baseUnits == SECONDS ? timeValue.seconds :
           baseUnits == MILLISECONDS ? timeValue.millis :
           timeValue.micros;
}

void BlockNot::addToTimerList() {
    if (firstTimer == nullptr) {
        firstTimer = currentTimer = this;
    } else {
        currentTimer->nextTimer = this;
        currentTimer = this;
    }
    this->nextTimer = nullptr;
}

/**
 * Global Methods affecting all instantiations of the BlockNot class
 */

void resetAllTimers(const unsigned long newStartTime) {
    BlockNot *current = BlockNot::firstTimer;
    while (current != nullptr) {
        current->reset(newStartTime);
        current = current->nextTimer;
    }
}