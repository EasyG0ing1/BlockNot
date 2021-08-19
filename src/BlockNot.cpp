#include <Arduino.h>
#include <BlockNot.h>

/*
 * Variables
 */


BlockNot *BlockNot::firstTimer = nullptr;
BlockNot *BlockNot::currentTimer = nullptr;

/*
 * Constructors
 */

BlockNot::BlockNot() { addToTimerList(); }

BlockNot::BlockNot(unsigned long time, bool units) {
    baseUnits = units;
    duration = convertUnitsSet(time);
    reset();
    addToTimerList();
}

BlockNot::BlockNot(unsigned long time, unsigned long stoppedReturnValue, bool units) {
    baseUnits = units;
    timerStoppedReturnValue = stoppedReturnValue;
    duration = convertUnitsSet(time);
    reset();
    addToTimerList();
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
    if (timerEnabled) return triggered;
    else return false;
}

bool BlockNot::triggeredOnDuration() {
    /**
     * Sets the next startTime to the current millis less the amount of time that has passed since the Triggered event was supposed to happen.
     * This keeps your timer triggering exactly every duration on the duration. The method getNewStartTime runs the calculation accounting
     * for possible rollover in the millis() value.
     */
    bool triggered = hasTriggered();
    if (triggered) {
        long elapsedTime = (long) timeSinceLastReset();
        reset(getNewStartTime(duration - elapsedTime));
    }
    return triggered;
}

bool BlockNot::notTriggered() {
    if (timerEnabled) return hasTriggered(); else return false;
}

bool BlockNot::firstTrigger() {
    if (hasTriggered() && !onceTriggered) {
        onceTriggered = true;
        if (timerEnabled) return true;
    }
    return false;
}

bool BlockNot::isEnabled() {
    return timerEnabled;
}

unsigned long BlockNot::getTimeUntilTrigger() {
    return timerEnabled ? convertUnitsGet(remaining()) : timerStoppedReturnValue;
}

unsigned long BlockNot::getStartTime() { return startTime; }

unsigned long BlockNot::getDuration() {
    return timerEnabled ? convertUnitsGet(duration) : timerStoppedReturnValue;
}

String BlockNot::getUnits() {
    return (baseUnits == SECONDS) ? "Seconds" : "Milliseconds";
}

unsigned long BlockNot::timeSinceLastReset() {
    return timerEnabled ? convertUnitsGet(timeSinceReset()) : timerStoppedReturnValue;
}

void BlockNot::setDisableReturnValue(unsigned long stoppedReturnValue) {
    timerStoppedReturnValue = stoppedReturnValue;
}

void BlockNot::start() {
    enable();
}

void BlockNot::stop() {
    disable();
}

void BlockNot::enable() {
    timerEnabled = true;
}

void BlockNot::disable() {
    timerEnabled = false;
}

void BlockNot::negateState() {
    timerEnabled = !timerEnabled;
}

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
    unsigned long mils = millis();
    if (startTime > mils) return mils + (maxMillis - startTime);
    return mils - startTime;
}

bool BlockNot::hasTriggered() {
    return timeSinceReset() >= duration;
}

bool BlockNot::hasNotTriggered() {
    return timeSinceReset() < duration;
}

unsigned long BlockNot::remaining() {
    unsigned long timePassed = timeSinceReset();
    return (timePassed < duration) ? duration - timePassed : 0;
}

unsigned long BlockNot::getNewStartTime(long offset, unsigned long now) {
    long newStartTime = now + offset;
    if (newStartTime < 0) {
        long delta = abs(newStartTime);
        return (maxMillis - delta);
    } else return newStartTime;
}

unsigned long BlockNot::convertUnitsSet(const unsigned long timeValue) {
    return (baseUnits == SECONDS) ? (timeValue * 1000) : timeValue;
}

unsigned long BlockNot::convertUnitsGet(const unsigned long timeValue) {
    return (baseUnits == SECONDS) ? (timeValue / 1000) : timeValue;
}

/*
 * Reset All Timers Methods
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
