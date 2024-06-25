#include <Arduino.h>
#include <BlockNot.h>

/*
 * This sketch was created for an LED layout that was installed onto the back of a 5th wheel.
 * It was written for a Raspberry Pi Pico
 *
 * It looks for four different states that a driver could be doing with the automobile blinkers.
 * It looks for:
 *  1) Left flasher flashing
 *  2) Right flasher flashing
 *  3) Both flashers flashing at the same time (hazard lights)
 *  4) Brakes are being applied
 *
 * When the flashers are engaged, it lights up LEDs that are arranged in a pattern. See the GIF images to see
 * how it lights up the various LEDs on the back of the 5th wheel.
 *
 * BlockNot has been used as the only means of getting the timing right for the various LED animations.
 *
 * This code shows you how to write a sketch where the execution of the code never stops, yet it provides
 * distinct functionality depending on the statae of the flashers.
 *
 * The input voltage from the blinkers will flash at some unknown frequency. We simply set a boolean high
 * when the pin reads at a positive voltage. When it sees that positive voltage, it resets the timeDelay
 * timer so that if there are no high values read on those pins for the duration of that timer, it
 * then sets the state that the turn signals are not being used, and we use a firstTrigger() for that
 * because we only need it to hit one time after a reset.
 *
 */

enum State {
    LEFT_BLINK,
    RIGHT_BLINK,
    ALL_OFF,
    HAZARD
};

State state = ALL_OFF;

u_int leftBlinkIn = 2;
u_int rightBlinkIn = 3;
u_int brakeIn = 4;

u_int right1 = 9;
u_int right2 = 10;
u_int right3 = 11;
u_int right4 = 12;
u_int right5 = 13;
u_int cLight = 14;

u_int left1 = 17;
u_int left2 = 18;
u_int left3 = 19;
u_int left4 = 20;
u_int left5 = 21;

u_int brakeHazard = 16;

bool brakeOn = false;

u_int rightArrow[7] = {0, right1, right2, right3, right4, right5, cLight};
u_int leftArrow[7] = {0, left1, left2, left3, left4, left5, cLight};


void getState() {
    static bool rightOn = false;
    static bool leftOn = false;
    static BlockNot timeDelay(3, SECONDS);

    if (gpio_get(rightBlinkIn) == HIGH) {
        sleep_ms(100);
        rightOn = gpio_get(rightBlinkIn) == HIGH;
        timeDelay.RESET;
    }
    if (gpio_get(leftBlinkIn) == HIGH) {
        sleep_ms(100);
        leftOn = gpio_get(leftBlinkIn) == HIGH;
        timeDelay.RESET;
    }
    if (timeDelay.firstTrigger()) {
        rightOn = false;
        leftOn = false;
    }
    if (rightOn && leftOn) {
        state = HAZARD;
    }
    else if (rightOn && !leftOn) {
        state = RIGHT_BLINK;
    }
    else if (leftOn && !rightOn) {
        state = LEFT_BLINK;
    }
    else {
        state = ALL_OFF;
    }
}


void arrowsOff() {
    for (int x = 1; x <= 6; x++) {
        gpio_put(rightArrow[x], LOW);
        gpio_put(leftArrow[x], LOW);
    }
    if (!brakeOn) {
        gpio_put(brakeHazard, LOW);
    }
}

void arrowBlink() {
    static BlockNot transition(40);
    static BlockNot start(650);
    static int index = 0;

    if (state == HAZARD) {
        index = 0;
        return;
    }
    if ((state != LEFT_BLINK && state != RIGHT_BLINK) || state == ALL_OFF) {
        arrowsOff();
        index = 0;
        return;
    }
    if (index == 0) {
        if (start.TRIGGERED) {
            index = 1;
            transition.RESET;
        }
    }
    if (index > 0) {
        if (transition.TRIGGERED) {
            if (index > 6) {
                arrowsOff();
                start.RESET;
                index = 0;
                return;
            }

            if (state == RIGHT_BLINK)
                gpio_put(rightArrow[index], HIGH);

            if (state == LEFT_BLINK)
                gpio_put(leftArrow[index], HIGH);

            index++;
        }
    }
}

void hazard() {
    static BlockNot transition(40);
    static BlockNot start(650);
    static int index = 0;
    if (state != HAZARD) {
        return;
    }

    if (index == 0) {
        if(start.TRIGGERED) {
            index = 1;
        }
    }

    if(index == 1) {
        gpio_put(brakeHazard, HIGH);
        transition.RESET;
    }

    if(index > 0) {
        if(transition.TRIGGERED) {
            if(index > 6) {
                arrowsOff();
                start.RESET;
                index = 0;
                return;
            }
            gpio_put(rightArrow[index], HIGH);
            gpio_put(leftArrow[index], HIGH);
            index++;
        }
    }
}

void brake() {
    gpio_put(brakeHazard, gpio_get(brakeIn) == LOW ? HIGH : LOW);
    brakeOn = gpio_get(brakeIn) == LOW;
}

void setup() {
    for (int x = 9 ; x <= 14 ; x++) {
        pinMode(x, OUTPUT);
    }
    for (int x = 16 ; x <= 21 ; x++) {
        pinMode(x, OUTPUT);
    }
    pinMode(brakeHazard, OUTPUT);
    pinMode(rightBlinkIn, INPUT);
    pinMode(leftBlinkIn, INPUT);
    pinMode(brakeIn, INPUT_PULLUP);
}

void loop() {
    getState();
    brake();
    switch (state) {
        case HAZARD: hazard();
        case ALL_OFF: arrowsOff();
        default: arrowBlink();
    }
    sleep_ms(1); // For good measure
}
