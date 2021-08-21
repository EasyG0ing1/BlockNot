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
bool BlockNot::global = false;
/*
 * Constructors
 */

BlockNot::BlockNot() {
    if(global) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, bool setGlobal) {
    baseUnits = MILLISECONDS;
    global = global ? global : setGlobal;
    duration = convertUnitsSet(time);
    reset();
    if (global) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, float units) {
    baseUnits = units;
    duration = convertUnitsSet(time);
    reset();
    if (global) addToTimerList();
}

BlockNot::BlockNot(unsigned long time, float units, bool setGlobal) {
    baseUnits = units;
    global = global ? global : setGlobal;
    duration = convertUnitsSet(time);
    reset();
    if (global) addToTimerList();
}

/*
 * Public Methods
 */

void BlockNot::setDuration(const unsigned long time, bool resetOption) {
    duration = convertUnitsSet(time);
    if (resetOption) reset();
}

void BlockNot::addTime(const unsigned long time, bool resetOption) {
    const unsigned long newDuration = duration + convertUnitsSet(time);
    duration = newDuration;
    if (resetOption) reset();
}

void BlockNot::takeTime(const unsigned long time, bool resetOption) {
    long newDuration = duration - convertUnitsSet(time);
    duration = newDuration > 0 ? newDuration : 0;
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
        int missedDurations = timeSinceReset() / duration;
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
    if (timerRunning) return hasTriggered(); else return false;
}

bool BlockNot::firstTrigger() {
    if (hasTriggered() && !onceTriggered) {
        onceTriggered = true;
        if (timerRunning) return true;
    }
    return false;
}

unsigned long BlockNot::timeTillTrigger() { return (hasTriggered()) ? 0 : convertUnitsGet(duration - timeSinceLastReset()); }

unsigned long BlockNot::getTimeUntilTrigger() {
    return timerRunning ? convertUnitsGet(remaining()) : timerStoppedReturnValue;
}

unsigned long BlockNot::getStartTime() { return startTime; }

unsigned long BlockNot::getDuration() {
    return timerRunning ? convertUnitsGet(duration) : timerStoppedReturnValue;
}

String BlockNot::getUnits() {
    return (baseUnits == SECONDS) ? "Seconds" : "Milliseconds";
}

unsigned long BlockNot::timeSinceLastReset() {
    return timerRunning ? convertUnitsGet(timeSinceReset()) : timerStoppedReturnValue;
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

void BlockNot::toggle() {
    timerRunning = !timerRunning;
}

void BlockNot::switchTo(float units) { baseUnits = units; }

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
    return timeSinceReset() >= duration;
}

bool BlockNot::hasNotTriggered() {
    return timeSinceReset() < duration;
}

unsigned long BlockNot::remaining() {
    unsigned long timePassed = timeSinceReset();
    return (timePassed < duration) ? (duration - timePassed) : 0;
}

unsigned long BlockNot::getDurationTriggerStartTime() {
    unsigned long timePassed = timeSinceReset();
    long numberOfDurationsPassed = timePassed / duration;
    long totalDurationTime = numberOfDurationsPassed * duration;
    return startTime + totalDurationTime;
}

unsigned long BlockNot::convertUnitsSet(const unsigned long timeValue) {
    return (baseUnits == SECONDS) ? (timeValue * 1000) : timeValue;
}

unsigned long BlockNot::convertUnitsGet(const unsigned long timeValue) {
    return (baseUnits == SECONDS) ? (timeValue / 1000) : timeValue;
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
