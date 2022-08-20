#include <BlockNot.h>

 /**
  * BlockNot can be used to debounce a button, and you don't even have to be conservative on your delay time
  * because when you press a button, the odds that you'll press a button again within a single second even
  * is really low, and since timers trigger AFTER the stated duration has passed, you can have your button
  * code run only if the timer has triggered, which means that if it has triggered your code will run immediately
  * without needing to debounce it using the delay() method, and after you let go of the button, the timer will
  * be primed and ready to go very shortly there after. So then the duration of the timer can be as long as
  * half a second or even a full second or longer since the timer is primed after that duration has already
  * passed. You're basically debouncing ahead of time so that your button code can run immediately.
  *
  * Connect one pin of your button to pin 12 of your Arduino, then the other button pin goes to ground, then
  * upload this sketch and try it out.
  *
  * If you want the button to respond faster than one second, change the timers duration from 1, SECONDS to
  * something smaller like just 500 for half a second or 250 for 1/4 second.
  */

BlockNot buttonTimer(1, SECONDS);
//BlockNot buttonTimer(500); //   1/2 second
//BlockNot buttonTimer(250); //   1/4 second

#define BUTTON 12

#define BUTTON_PRESSED digitalRead(BUTTON) == LOW;


 void setup() {
     Serial.begin(115200);
     pinMode(BUTTON, INPUT_PULLUP);
 }

 loop() {
     if(BUTTON_PRESSED)
         if(buttonTimer.TRIGGERED)
             Serial.println("Button Pressed!");
 }