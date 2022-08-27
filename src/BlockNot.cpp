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
Global BlockNot::global = GLOBAL_RESET;

/**
 * Constructors
 */

BlockNot::BlockNot() {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    initDuration(time);
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, State state) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    if(state == STOPPED)
        stop();
    initDuration(time);
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, Unit units) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    baseUnits = units;
    initDuration(time);
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, Unit units, State state) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    if(state == STOPPED)
        stop();
    baseUnits = units;
    initDuration(time);
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, Global globalReset) {
    initDuration(time);
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, State state, Global globalReset) {
    if(state == STOPPED)
        stop();
    initDuration(time);
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, Unit units, Global globalReset) {
    baseUnits = units;
    initDuration(time);
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, Unit units, State state, Global globalReset) {
    if(state == STOPPED)
        stop();
    baseUnits = units;
    initDuration(time);
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, State state) {
    if(state == STOPPED)
        stop();
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();

}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    baseUnits = units;
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units, State state) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    if(state == STOPPED)
        stop();
    baseUnits = units;
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, Global globalReset) {
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, Global globalReset, State state) {
    if(state == STOPPED)
        stop();
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units, Global globalReset) {
    baseUnits = units;
    initDuration(time);
    timerStoppedReturnValue = stoppedReturnValue;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units, Global globalReset, State state) {
    if(state == STOPPED)
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

void BlockNot::setDuration(const unsigned long time, bool resetOption) {
    initDuration(time);
    if (resetOption) reset();
}

void BlockNot::setDuration(const unsigned long time, Unit inUnits, bool resetOption) {
    initDuration(time, inUnits);
    if (resetOption) reset();
}

void BlockNot::addTime(const unsigned long time, bool resetOption) {
    unsigned long newDuration;
    switch(baseUnits) {
        case MICROSECONDS:
            newDuration = duration.micros + time;
            if (newDuration < duration.micros) newDuration = 0xFFFFFFFFL;
            duration.micros = newDuration;
            break;
        case MILLISECONDS:
            newDuration = duration.millis + time;
            if (newDuration < duration.millis) newDuration = 0xFFFFFFFFL;
            duration.millis = newDuration;
            break;
        case SECONDS:
            newDuration = duration.millis + time;
            if (newDuration < duration.millis) newDuration = 0xFFFFFFFFL;
            duration.millis = newDuration;
            break;
    }
    if (resetOption) reset();
}

void BlockNot::takeTime(const unsigned long time, bool resetOption) {
    long newDuration;
    switch(baseUnits) {
        case MICROSECONDS:
            newDuration = duration.micros - time;
            if (newDuration > duration.micros) newDuration = 0L;
            duration.micros = newDuration;
            break;
        case MILLISECONDS:
            newDuration = duration.millis - time;
            if (newDuration > duration.millis) newDuration = 0L;
            duration.millis = newDuration;
            break;
        case SECONDS:
            newDuration = duration.millis - time;
            if (newDuration > duration.millis) newDuration = 0L;
            duration.millis = newDuration;
            break;
    }
    if (resetOption) reset();
}

bool BlockNot::triggered(bool resetOption) {
    bool triggered = hasTriggered();
    if (resetOption && triggered) {
        reset();
    }
    if (timerState == RUNNING) return triggered;
    else return false;
}

bool BlockNot::triggeredOnDuration(bool allMissed) {
    bool triggered = hasTriggered();
    unsigned long missedDurations;
    unsigned long newStartTime;
    if (triggered) {
        switch(baseUnits) {
            case MICROSECONDS:
                missedDurations = timeSinceReset() / (unsigned long) duration.micros;
                break;
            case MILLISECONDS:
                missedDurations = timeSinceReset() / (unsigned long) duration.millis;
                break;
            case SECONDS:
                missedDurations = timeSinceReset() / (unsigned long) duration.millis;
                break;
        }
        totalMissedDurations += (allMissed ? missedDurations : 0);
        newStartTime = getDurationTriggerStartTime();
        reset(newStartTime);
    }
    if (totalMissedDurations > 0 && allMissed) {
        totalMissedDurations--;
        return true;
    }
    return triggered;
}

bool BlockNot::notTriggered() {
    if (timerState == RUNNING) return !hasTriggered();
    else return false;
}

bool BlockNot::firstTrigger() {
    if (hasTriggered() && !onceTriggered) {
        onceTriggered = true;
        if (timerState == RUNNING)
            return true;
    }
    return false;
}

unsigned long BlockNot::getNextTriggerTime() {
    cTime nextTrigger;
    nextTrigger.micros = startTime + duration.micros;
    switch(baseUnits) {
        case MICROSECONDS:
            nextTrigger.micros = startTime + duration.micros;
            break;
        case MILLISECONDS:
            nextTrigger.millis = startTime + duration.millis;
            break;
        case SECONDS:
            nextTrigger.millis = startTime + duration.millis;
            break;
    }
    return convertUnits(nextTrigger);
}

unsigned long BlockNot::getTimeUntilTrigger() {
    return timeTillTrigger();
}

unsigned long BlockNot::getStartTime() {
    cTime sTime;
    switch(baseUnits) {
        case MICROSECONDS:
            sTime.micros = startTime;
            break;
        case MILLISECONDS:
            sTime.millis = startTime;
            break;
        case SECONDS:
            sTime.millis = startTime;
            break;
    }
    return convertUnits(sTime);
}

unsigned long BlockNot::getStartTime(Unit units) {
    cTime timeValue;
    timeValue.micros = startTime;
    switch(units) {
        case MICROSECONDS:
            return timeValue.micros;
        case MILLISECONDS:
            return timeValue.millis;
        case SECONDS:
            return timeValue.seconds;
    }
    return 0L;
}

unsigned long BlockNot::getDuration() {
    return (timerState == RUNNING) ? convertUnits(duration) : timerStoppedReturnValue;
}

String BlockNot::getUnits() {
    return (baseUnits == SECONDS) ? "Seconds" : (baseUnits == MILLISECONDS) ?  "Milliseconds" : "Microseconds";
}

unsigned long BlockNot::getTimeSinceLastReset() {
    cTime timeLapsed;
    switch(baseUnits) {
        case MICROSECONDS:
            timeLapsed.micros = timeSinceReset();
            break;
        case MILLISECONDS:
            timeLapsed.millis = timeSinceReset();
            break;
        case SECONDS:
            timeLapsed.millis = timeSinceReset();
            break;
    }
    return (timerState == RUNNING) ? convertUnits(timeLapsed) : timerStoppedReturnValue;
}

void BlockNot::setStoppedReturnValue(unsigned long stoppedReturnValue) {
    timerStoppedReturnValue = stoppedReturnValue;
}

void BlockNot::start(bool resetOption) {
    if(resetOption)
        reset();
    else {
        switch (baseUnits) {
            case SECONDS:
                startTime = millis() - stopTime.millis;
                break;

            case MILLISECONDS:
                startTime = millis() - stopTime.millis;
                break;

            case MICROSECONDS:
                startTime = micros() - stopTime.micros;
                break;
        }
    }
    timerState = RUNNING;
}

void BlockNot::stop() {
    timerState = STOPPED;
    switch(baseUnits) {
        case SECONDS:
            stopTime.millis = millis();
            break;

        case MILLISECONDS:
            stopTime.millis = millis();
            break;

        case MICROSECONDS:
            stopTime.micros = micros();
            break;
    }
}

bool BlockNot::isRunning() {return timerState == RUNNING;}

bool BlockNot::isStopped() {return timerState == STOPPED;}

void BlockNot::toggle() {
    if(timerState == RUNNING)
        timerState = STOPPED;
    else
        timerState = RUNNING;
}

unsigned long BlockNot::convert(unsigned long value, Unit units) {
    cTime timeValue;
    unsigned long result = 0L;
    switch(baseUnits) {
        case MICROSECONDS:
            timeValue.micros = value;
            break;
        case MILLISECONDS:
            timeValue.millis = value;
            break;
        case SECONDS:
            timeValue.seconds = value;
            break;
    }
    switch(units) {
        case MICROSECONDS:
            result = timeValue.micros;
            break;
        case MILLISECONDS:
            result = timeValue.millis;
            break;
        case SECONDS:
            result = timeValue.seconds;
            break;
    }
    return result;
}

void BlockNot::switchTo(Unit units) { baseUnits = units; }

void BlockNot::reset(const unsigned long newStartTime) {
   unsigned long finalStartTime = newStartTime;
    if(newStartTime == 0) {
        switch(baseUnits) {
            case MICROSECONDS:
                finalStartTime = micros() + microsOffset;
                break;
            case MILLISECONDS:
                finalStartTime = millis() + millisOffset;
                break;
            case SECONDS:
                finalStartTime = millis() + millisOffset;
                break;
        }
    }
    resetTimer(finalStartTime);
}

void BlockNot::setMillisOffset(unsigned long offset) {
    long delta = offset - millisOffset;
    startTime = startTime + delta;
    millisOffset = offset;
}

void BlockNot::setMicrosOffset(unsigned long offset) {
    microsOffset = offset;
}

unsigned long BlockNot::getMillis() {
    return millis() + millisOffset;
}

/**
 * Private Methods
 */

void BlockNot::initDuration(unsigned long time) {
    switch(baseUnits) {
        case MICROSECONDS:
            duration.micros = time;
            break;
        case MILLISECONDS:
            duration.millis = time;
            break;
        case SECONDS:
            duration.seconds = time;
            break;
    }
}

void BlockNot::initDuration(unsigned long time, Unit inUnits) {
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
    }
}

void BlockNot::resetTimer(const unsigned long newStartTime) {
    startTime = newStartTime;
    onceTriggered = false;
}

unsigned long BlockNot::timeSinceReset() {
    unsigned long result = 0L;
    unsigned long millisBase = millisOffset + millis();
    switch (baseUnits) {
        case MICROSECONDS:
            result = microsOffset + micros() - startTime;
            break;
        case MILLISECONDS:
            result = millisBase - startTime;
            break;
        case SECONDS:
            result = millisBase - startTime;
            break;
    }
    return result;
}

bool BlockNot::hasTriggered() {
    bool triggered;
    switch(baseUnits) {
        case MICROSECONDS:
            triggered = timeSinceReset() >= (unsigned long) duration.micros;
            break;
        case MILLISECONDS:
            triggered = timeSinceReset() >= (unsigned long) duration.millis;
            break;
        case SECONDS:
            triggered = timeSinceReset() >= (unsigned long) duration.millis;
            break;
    }
    return triggered;
}

bool BlockNot::hasNotTriggered() {
    bool notTriggered;
    switch(baseUnits) {
        case MICROSECONDS:
            notTriggered = timeSinceReset() < (unsigned long) duration.micros;
            break;
        case MILLISECONDS:
            notTriggered = timeSinceReset() < (unsigned long) duration.millis;
            break;
        case SECONDS:
            notTriggered = timeSinceReset() < (unsigned long) duration.millis;
            break;
    }
    return notTriggered;
}

unsigned long BlockNot::timeTillTrigger() {
    cTime triggerTime;
    unsigned long sinceReset = timeSinceReset();
    unsigned long tillTrigger = 0L;
    switch(baseUnits) {
        case MICROSECONDS: {
            triggerTime.micros = (sinceReset < duration.micros) ? (unsigned long) (duration.micros - sinceReset) : 0L;
            tillTrigger = (timerState == RUNNING) ? convertUnits(triggerTime) : timerStoppedReturnValue;
            break;
        }
        case MILLISECONDS: {
            triggerTime.millis = (sinceReset < duration.millis) ? (unsigned long) (duration.millis - sinceReset) : 0L;
            tillTrigger = (timerState == RUNNING) ? convertUnits(triggerTime) : timerStoppedReturnValue;
            break;
        }
        case SECONDS: {
            triggerTime.millis = (sinceReset < duration.millis) ? (unsigned long) (duration.millis - sinceReset) : 0L;
            tillTrigger = (timerState == RUNNING) ? convertUnits(triggerTime) : timerStoppedReturnValue;
            break;
        }
    }
    return tillTrigger;
}

unsigned long BlockNot::remaining() {
    unsigned long timePassed = timeSinceReset();
    unsigned long remain = 0L;
    switch(baseUnits) {
        case MICROSECONDS:
            remain = (timePassed < duration.micros) ? ((unsigned long) duration.micros - timePassed) : 0;
            break;
        case MILLISECONDS:
            remain = (timePassed < duration.millis) ? ((unsigned long) duration.millis - timePassed) : 0;
            break;
        case SECONDS:
            remain = (timePassed < duration.millis) ? ((unsigned long) duration.millis - timePassed) : 0;
            break;
    }
    return remain;
}

unsigned long BlockNot::getDurationTriggerStartTime() {
    unsigned long durationStartTime = 0L;
    switch(baseUnits) {
        case MICROSECONDS:
            durationStartTime = startTime + ((timeSinceReset() / (unsigned long) duration.micros) * (unsigned long) duration.micros);
            break;
        case MILLISECONDS:
            durationStartTime = startTime + ((timeSinceReset() / (unsigned long) duration.millis) * (unsigned long) duration.millis);
            break;
        case SECONDS:
            durationStartTime = startTime + ((timeSinceReset() / (unsigned long) duration.millis) * (unsigned long) duration.millis);
            break;
    }
    return durationStartTime;
}

unsigned long BlockNot::convertUnits(cTime timeValue) {
    return (baseUnits == MICROSECONDS) ? timeValue.micros : (baseUnits == MILLISECONDS) ? timeValue.millis : timeValue.seconds;
}

/**
 * Global Methods affecting all instantiations of the BlockNot class
 */

void resetAllTimers(const unsigned long newStartTime) {
    BlockNot *timer = BlockNot::getFirstTimer();
    while (timer != nullptr) {
        timer->BlockNot::reset(newStartTime);
        timer = timer->BlockNot::getNextTimer();
    }
}

void resetAllTimers(BlockNot *timer) {
        resetAllTimers(timer->BlockNot::getStartTime());
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
