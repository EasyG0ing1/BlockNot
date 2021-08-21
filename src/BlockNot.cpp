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

#include <BlockNot.h>

/*
 * Global Variables
 */

BlockNot *BlockNot::firstTimer = nullptr;
BlockNot *BlockNot::currentTimer = nullptr;
Global BlockNot::global = GLOBAL_RESET;
/*
 * Constructors
 */

BlockNot::BlockNot() {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    baseUnits = MILLISECONDS;
    duration.millis = time;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, Unit units) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    baseUnits = units;
    if(baseUnits == SECONDS) duration.seconds = time; else duration.millis = time;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, Global globalReset) {
    baseUnits = MILLISECONDS;
    duration.millis = time;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, Unit units, Global globalReset) {
    baseUnits = units;
    if(baseUnits == SECONDS) duration.seconds = time; else duration.millis = time;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    baseUnits = MILLISECONDS;
    timerStoppedReturnValue = stoppedReturnValue;
    if(baseUnits == SECONDS) duration.seconds = time; else duration.millis = time;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units) {
    global = (global == NO_GLOBAL_RESET) ? NO_GLOBAL_RESET : GLOBAL_RESET;
    baseUnits = units;
    timerStoppedReturnValue = stoppedReturnValue;
    if(baseUnits == SECONDS) duration.seconds = time; else duration.millis = time;
    reset();
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, Global globalReset) {
    baseUnits = MILLISECONDS;
    timerStoppedReturnValue = stoppedReturnValue;
    if(baseUnits == SECONDS) duration.seconds = time; else duration.millis = time;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, Unit units, Global globalReset) {
    baseUnits = units;
    timerStoppedReturnValue = stoppedReturnValue;
    if(baseUnits == SECONDS) duration.seconds = time; else duration.millis = time;
    reset();
    global = globalReset;
    if (global == GLOBAL_RESET) addToTimerList();
}

/*
 * Public Methods
 */

void BlockNot::setDuration(const unsigned long time, bool resetOption) {
    if(baseUnits == SECONDS) duration.seconds = time; else duration.millis = time;
    if (resetOption) reset();
}

void BlockNot::addTime(const unsigned long time, bool resetOption) {
    cTime newTime;
    if (baseUnits == MILLISECONDS) newTime.millis = time; else newTime.seconds = time;
    unsigned long newDuration = duration.millis + newTime.millis;
    if (newDuration < duration.millis) newDuration = 0xFFFFFFFFL;
    duration.millis = newDuration;
    if (resetOption) reset();
}

void BlockNot::takeTime(const unsigned long time, bool resetOption) {
    cTime ct;
    if (baseUnits == MILLISECONDS) ct.millis = time; else ct.seconds = time;
    long newDuration = duration.millis - ct.millis;
    if (newDuration > duration.millis) newDuration = 0L;
    duration.millis = newDuration;
    if (resetOption) reset();
}

bool BlockNot::triggered(bool resetOption) {
    bool triggered = hasTriggered();
    if (resetOption && triggered) {
        reset();
    }
    if (timerRunning) return triggered;
    else return false;
}

bool BlockNot::triggeredOnDuration(bool allMissed) {
    bool triggered = hasTriggered();
    if (triggered) {
        int missedDurations = timeSinceReset() / (unsigned long) duration.millis;
        totalMissedDurations += (allMissed ? missedDurations : 0);
        unsigned long newStartTime = getDurationTriggerStartTime();
        reset(newStartTime);
    }
    if (totalMissedDurations > 0 && allMissed) {
        totalMissedDurations--;
        return true;
    }
    return triggered;
}

bool BlockNot::notTriggered() {
    if (timerRunning) return !hasTriggered(); else return false;
}

bool BlockNot::firstTrigger() {
    if (hasTriggered() && !onceTriggered) {
        onceTriggered = true;
        if (timerRunning) return true;
    }
    return false;
}

unsigned long BlockNot::getNextTriggerTime() {
    return  startTime + (unsigned long) duration.millis;
}

unsigned long BlockNot::getTimeUntilTrigger() {
    return timeTillTrigger();
}

unsigned long BlockNot::getStartTime() { return startTime; }

unsigned long BlockNot::getDuration() {
    return timerRunning ? convertUnits(duration) : timerStoppedReturnValue;
}

String BlockNot::getUnits() {
    return (baseUnits == SECONDS) ? "Seconds" : "Milliseconds";
}

unsigned long BlockNot::getTimeSinceLastReset() {
    cTime timeLapsed;
    timeLapsed.millis = timeSinceReset();
    return timerRunning ? convertUnits(timeLapsed) : timerStoppedReturnValue;
}

void BlockNot::setStoppedReturnValue(unsigned long stoppedReturnValue) {
    timerStoppedReturnValue = stoppedReturnValue;
}

void BlockNot::start() {
    timerRunning = true;
}

void BlockNot::stop() {
    timerRunning = false;
}

bool BlockNot::isRunning() {return timerRunning;}

bool BlockNot::isStopped() {return !timerRunning;}

void BlockNot::toggle() {
    timerRunning = !timerRunning;
}

void BlockNot::switchTo(Unit units) { baseUnits = units; }

void BlockNot::reset(const unsigned long newStartTime) {
    resetTimer(newStartTime);
}

/*
 * Private Methods
 */

void BlockNot::resetTimer(const unsigned long newStartTime) {
    startTime = newStartTime;
    onceTriggered = false;
}

unsigned long BlockNot::timeSinceReset() {
    return millis() - startTime;
}

bool BlockNot::hasTriggered() {
    return timeSinceReset() >= (unsigned long) duration.millis;
}

bool BlockNot::hasNotTriggered() {
    return timeSinceReset() < (unsigned long) duration.millis;
}

unsigned long BlockNot::timeTillTrigger() {
    cTime triggerTime;
    unsigned long sinceReset = timeSinceReset();
    triggerTime.millis = (sinceReset < duration.millis) ? (unsigned long) (duration.millis - sinceReset) : 0L;
    return timerRunning ? convertUnits(triggerTime) : timerStoppedReturnValue;
}

unsigned long BlockNot::remaining() {
    unsigned long timePassed = timeSinceReset();
    return (timePassed < duration.millis) ? ((unsigned long) duration.millis - timePassed) : 0;
}

unsigned long BlockNot::getDurationTriggerStartTime() {
    return startTime + ((timeSinceReset() / (unsigned long) duration.millis) * (unsigned long) duration.millis);
}

unsigned long BlockNot::convertUnits(cTime timeValue) {
    return (baseUnits == SECONDS) ? timeValue.seconds : timeValue.millis;
}

/*
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
