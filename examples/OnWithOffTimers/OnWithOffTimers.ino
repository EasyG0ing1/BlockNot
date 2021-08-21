/*
 * This sketch shows the correct way of implementing non-blocking timers
 * when you need to have one timer control when an event stays on and
 * another timer to control how long the event stays off.
 *
 * It can be a little tricky, but this sketch has made every effort
 * at being as simple and easy to understand as possible.
 *
 * In this sketch, we use LEDs and then different timers to control
 * how long they stay on and how long they stay off. BUT, this could
 * easily be a different device such as a stepper motor, a regular motor
 * a relay ...
 *
 * This sketch will start by turning on two LEDs. One of them (blue)
 * will only stay on for 500 ms and it will repeat every 2500ms. The
 * second LED (red) will turn on and stay on for 5 seconds, then it
 * will turn off for one second. every six seconds they will turn on
 * at the same time.
 */

#include <Arduino.h>
#include <BlockNot.h>

BlockNot blueTimerON(500,GLOBAL_RESET);
BlockNot blueTimerOFF(2500);

BlockNot redTimerON(5000);
BlockNot redTimerOFF(1000);

const int BLUE_PIN = 10;
const int RED_PIN = 9;

bool redON = false;
bool blueON = false;

bool blueIsOff() { return !blueON; }

bool blueIsOn() { return blueON; }

bool redIsOff() { return !redON; }

bool redIsOn() { return redON; }

void turnRedOn() {
    digitalWrite(RED_PIN, LOW);
    redTimerON.RESET;
    redON = true;
}

void turnRedOff() {
    digitalWrite(RED_PIN, HIGH);
    redTimerOFF.RESET;
    redON = false;
}

void turnBlueOn() {
    digitalWrite(BLUE_PIN, LOW);
    blueTimerON.RESET;
    blueON = true;
}

void turnBlueOff() {
    digitalWrite(BLUE_PIN, HIGH);
    blueTimerOFF.RESET;
    blueON = false;
}

void setup() {
    Serial.begin(115200);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
    resetAllTimers();
    turnBlueOn();
    turnRedOn();
    Serial.println(F("\nREADY!\n"));
}

void loop() {

    if (blueTimerON.TRIGGERED && blueIsOn()) {
        turnBlueOff();
    }

    if (blueTimerOFF.TRIGGERED && blueIsOff()) {
        turnBlueOn();
        Serial.println("Blue On");
    }

    if (redTimerON.TRIGGERED && redIsOn()) {
        turnRedOff();
    }

    if (redTimerOFF.TRIGGERED && redIsOff()) {
        turnRedOn();
        Serial.println("Red On\n");
    }
}
