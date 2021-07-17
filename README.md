# BlockNot Arduino Library
**This library enables you to create non-blocking timers using simple, common sense terms which simplifies the reading and writing of your code. It offers, among several things, convenient timer functionality, but most of all ... it gets you away from blocking methods - like delay() - as a means of managing events in your code. Non-Blocking is the proper way to implement timing events in Arduino code and BlockNot makes it easy!**

### Version update notes are at the end of this document

## Getting started immediately
Here is an example of BlockNot's easiest and most common usage:

First, you crate the timer:
```C++ 
#include <BlockNot.h>   
BlockNot helloTimer(1300); //In Milliseconds    
```
**OR** 

```C++ 
#include <BlockNot.h>   
BlockNot helloTimer(15, SECONDS); //Whole Seconds    
```
Then, you just test it to see if it triggered.
```C++
   if (helloTimer.TRIGGERED) {  
      Serial.println("Hello World!"); 
   } 
 ``` 
 That is all you need to start using BlockNot. Keep reading to learn about other features of the library.  
  
    
## Theory behind BlockNot    
 This is a traditional non-blockling timer:  
  
```C++  
long someDuration = 1300;  
long startTime = millis();  
if (millis() - startTime >= someDuration) {  
        //Code to run after someDuration has passed.
 }  
```  

This does the same thing, only with much simpler code!
```C++  
if (myTimer.TRIGGERED) {  
        //Code to run after timer has triggered.
 }  
```  
The idea behind BlockNot is very simple. You create the timer, set its duration, then check on the timer in your looping code to see if it TRIGGERED, or you can check for other information as you might need.

For example, if you wanted to see if the timers duration has come to pass, but you don't want to reset the timer, you can use this method:
```C++  
if (myTimer.triggered(NO_RESET)) {}  
```  
OR, you can do it like this:
```C++  
if (myTimer.HAS_TRIGGERED) {}  
```  
They both do the same thing, but in terms of readability, the second example is the obvious choice. BlockNot has several easy to understand commands that make it very 'user-friendly' and make your code much more readable.

Here is a simple graph showing you how BlockNot timers work. What's important here is to realize that your code never stops executing while the timer is passing time.

![Graphical illustration of timer in action](https://i.imgur.com/5HkKgTA.png)

## The Trigger

BlockNot is all about the trigger event. When runners get ready to start a race, it is common practice for someone to stand next to the line and hold a gun in the air and pull the trigger when the race starts. That is the idea behind the TRIGGERED event in BlockNot. If your timer is set, for example, to 1300 miliseconds, it will return true when you call the TRIGGERED event every 1300 milliseconds ... there are exceptions as you will see.  

```C++  
if (voltageReadTimer.TRIGGERED) {  
    readVoltage();
}  
```   

As of version 1.7.0, we now have a new trigger method called <b>triggeredOnDuration()</b> ... 

So, let's say that you have a 2300ms timer, and you call for the TRIGGERED event at 3000ms after the timers start time. Well, when you call TRIGGERED, the timers new startTime is whatever the current millis() value is, so by calling the timer long after its trigger event past, the time in between the event and your calling of the event is ignored. 

## The Reset

Resetting a timer is critical to performing repeated events at the right intervals. However, there may be times when you don't want this behavior. 

Resetting of a timer once it has triggered is the default behavior of BlockNot.
```C++  
if (myTimer.TRIGGERED) { my code }  
```  
Using TRIGGERED, the timer automatically resets, whereas if you do this:
```C++  
if (myTimer.HAS_TRIGGERED) { my code }   
```  
The startTime **does not reset** and that test will always come back true every time it is executed in your code, as long as the timers duration has passed.

There is another method which I have found quite handy in specific situations. And that is the FIRST_TRIGGER method. You use it like this:
```C++  
if (myTimer.FIRST_TRIGGER) { my code }  
```  
That method will return true ONE TIME after the timers duration has passed, but subsequent calls to that method will return false until you manually reset the timer like this:
```C++  
myTimer.RESET;  
```  
Why would you need to do that? There are countless scenarios where that would be immediately useful. I have used it with stepper motor projects where I want an idle stepper motor to be completely cut off from any voltage when it's been idle for a defined length of time ... lets say 25 seconds.

So first, we define the timer
```C++  
BlockNot stepperSleepTimer (25, SECONDS);  
```  
Then in the code that we use to step the motor, we reset the timer:
```C++  
stepperSleepTimer.RESET;  
```  
Then, in your loop, you would put something like this:
```C++  
if (stepperSleepTimer.ONE_TRIGGER) {  
     sleepStepper();
 }  
```  
So that if the stepper hasn't moved in the last 25 seconds, it will be put to sleep and that sleep routine won't be constantly run over and over again. Yet when the stepper is engaged again, then that sleep timer gets reset.

## Reset All Timers

Recently added to BlockNot is the ability to reset every timer that you have created all at once in a single line of code.

You can do this by calling either the method or the macro:
```C++
resetAllTimers();
RESET_TIMERS;
```

When you call this method, it first captures the value of millis() then it assigns that value to the startTime of all of your timers so that they all reset at precisely the exact same time.

Read the description of the ResetAll example below for more info.

## Seconds / Milliseconds

Sometimes you only need to have timers that deal in full seconds, and it can be cumbersome to have to use numbers like 45000 for 45 seconds! As of version 1.6.5, you can now instantiate a timer with an optional argument to define it as a SECONDS timer like this:
```C++
BlockNot myTimer(5, SECONDS);
```
When you define your timer as a SECONDS timer, any values you read from the timer will be in seconds even though under the hood, time is calculated in milliseconds. The response values will be rounded however the compiler handles that. Also, any changes you make to the timer's duration must be done in seconds (More discussion about this at the bottom of this document in the <b>Version Update Notes</b> section).

## Start / Stop

You can stop a timer, then start it again as needed.

When a timer is stopped, any call to the timer that would return a boolean value will ALWAYS 
return false. And when you query the timer where a numeric value is supposed to be returned, 
it will return a ZERO by default, although you can change what number it returns for those 
methods, as long as the number you set is a positive number (BlockNot does not ever deal with 
negative numbers, since time cannot go backwards),

These methods will ALWAYS return false when a timer is stopped (for macro calls see the Macro 
section of this document):

*    **triggered()**
*    **notTriggered()**
*    **firstTrigger()**

These methods will return a ZERO by default when a timer is stopped, or whichever value you chose to get back (explained below).
*    **getDuration()**
*    **getTimeUntilTrigger()**
*    **timeSinceLastReset()**

These methods will do NOTHING if the timer is stopped. Meaning that the changes you wish to make to the timer simply cannot be done if the timer is stopped - I am considering changing that behavior - if you think it needs to be changed, send me an email or open an incident.
*    **setDuration()**
*    **addTime()**
*    **takeTime()**
*    **reset()**

### How do I choose what number is returned on a stopped timer?

All you do is add your desired number when you create your timer like this
```C++  
BlockNot myTimer(2000, 8675309);  
```  

The first number is the timers duration and the second number is the return value for the methods just mentioned above that return numeric values when the timer is stopped - 0 by default (Jenny's phone number in the example ;-). That second number CANNOT be a negative number and the compiler will throw an error if you pass it a negative number.

OR, you can set it after the timer has been created
```C++
setDisableReturnValue(8675309);
```

You can disable / enable a timer using these methods / macros.
```C++  
myTimer.enable();  
myTimer.ENABLE;  
myTimer.disable();  
myTimer.DISABLE;  
```  

And you can find out if the timer is started or stopped using either of these calls:
```C++  
myTimer.isEnabled();  
myTimer.ENABLED;  
```  

You can also flip the state of the timer (if stopped, it will enable it, or if it's timerEnabled, it will disable it):
```C++  
myTimer.negateState();  
```  
But the macro's are far easier and more intuitive:
```C++  
myTimer.FLIP;  
myTimer.SWAP;  
myTimer.FLIP_STATE;  
myTimer.SWAP_STATE;  
```  
Why would you want to just change the state with one line of code? Perhaps you have a toggle button that will toggle a timer to be started or stopped ... you can assign the one command to the button and everything is handled.

```C++  
if (BUTTON_PRESSED) {  
   myTimer.FLIP_STATE;
 }  
```  

## Summary

Well, those are the fundamentals of BlockNot.

Simple, right?

BlockNot is a library intended to make the employment of non-blocking timers easy, intuitive, natural and obvious. It can be engaged with simple single word method calls or by calling the core methods directly.

There are more methods that allow you to affect change on your timers after instantiation and also methods to get info about your timers. You can change the duration of an existing timer in three different ways, you can reset the timer, or you can even find out how much time is left before the trigger event occurs, or find out how much time has passed since the timer last triggered.

## Examples

There are currently four examples in the library.

#### BlockNotBlink

This sketch does the same thing as the famous blink sketch, only it does it with BlockNot elegance and style.


#### BlockNotBlinkParty

If you have a nano or an uno or equivalent laying around and four LEDs and some resistors, connect them to pins 9 - 12 and run this sketch. You will immediately see the benefit of non-blocking timers. You could never write a sketch that could do the same thing using the delay() command.  It would be impossible.

<B>Non-Blocking MATTERS!</B>

#### TimersRules

This sketch has SIX timers created and running at the same time. There are various things happening at the trigger event of each timer. The expected behavior is explained in the out Strings to Serial. Read them, then let it run for a minute or so then stop your Serial monitor and look at the output. You should be able to look at the number of milliseconds that is given in each output, and compare the differences with the expected behavior and see that everything runs as it is expected to run.<BR><BR>For example, when LiteTimer triggers, you should soon after that see the output from stopAfterThreeTimer.  When you look at the number of milliseconds in each of their outputs, you can see that indeed it does trigger three seconds after being reset, but then it does not re-trigger until after it is reset again.
 
#### ResetAll

This sketch shows how all BlockNot timers defined in your sketch can be reset with a single line of code, rather than having to call reset() for each and every one separately. This comes in handy when all timers need to be reset at once, e.g. after the system clock has been adjusted from an external source (NTP or RTC, for example).

<BR>These examples barely scratch the surface of what you can accomplish with BlockNot.

#### OnWithOffTimers

This example shows you how to use on and off timers to control anything that you need to have on for a certain length of time and also off for a certain length of time.

The example specifically blinks two LEDs such that they will always be in sync every 6 seconds ... by this pattern:

![](https://i.imgur.com/NU6PAQW.png)

# Methods

Below you will find the name of each method in the library and any arguments that it accepts. Below that list, you will find the names of the macros that are connected to each method along with the arguments that a given macro may or may not pass to the method. The macros are key to making your code simple.

** For any method call that resets a timer by default, the resetting behavior can be overridden by passing **NO_RESET** into the methods argument, the exception to this is the triggeredOnDuration() method, which exists because of the way it resets your timer, so overriding reset would make the method useless.


*    **triggered()** - Returns true if the duration time has passed. Also resets the timer to the current millis() (override by passing NO_RESET as an argument).
*    **triggeredOnDuration()** - Returns true if the duration time has passed. Also resets the timer to the current startTime + duration. There is no override for the resetting of the timer. See discussion below under the version 1.7.0 update notes.
*    **notTriggered()** - Returns true if the trigger event has not happened yet.
*    **getDuration()** - Returns an unsigned long, the duration that is currently set in the timer.
*    **setDuration()** - Override the current timer duration and set it to a new value. This also resets the timer. If you need the timer to NOT reset, then pass arguments like this (newDuration, NO_RESET);
*    **addTime()** - Adds the time you pass into the argument to the current duration value. This does NOT reset the timer. To also reset the timer, call the method like this **addTime(newTime, WITH_RESET);**
*    **takeTime()** - The opposite effect of addTime(), same deal if you want to also reset the timer.
*    **getTimeUntilTrigger()** - Returns an unsigned long with the number of milliseconds remaining until the trigger event happens.
*    **timeSinceLastReset()** - Returns an unsigned long with the number of milliseconds that have passed since the timer was last reset or instantiated.
*    **reset()** - Sets the start time of the timer to the current millis().
*    **firstTrigger()** - Returns true only once and only after the timer has triggered.
*    **enable()** - enables the timer. Timers are timerEnabled by default.
*    **disable()** - disables the timer.
*    **start()** - starts the timer (A new timer is created in the running state).
*    **stop()** - stops the timer.
*    **isEnabled()** - returns true or false depending on the current state of the timer.
*    **resetAllTimers()** - loops through all timers that you created and resets startTime to millis(), which is recorded once and applied to all timers so they will all have the exact same startTime. 

# Macros

Here are the macro terms and the methods that they call along with any arguments they pass into the method:

*    **TIME_PASSED** - timeSinceLastReset()
*    **TIME_SINCE_RESET** - timeSinceLastReset()
*    **ELAPSED** - timeSinceLastReset()
*    **TIME_REMAINING** - getTimeUntilTrigger()
*    **REMAINING** - getTimeUntilTrigger()
*    **DURATION** - getDuration()
*    **DONE** - triggered()
*    **TRIGGERED** - triggered()
*    **TRIGGERED_ON_DURATION** - triggeredOnDuration()
*    **DONE_NO_RESET** - triggered(NO_RESET)
*    **HAS_TRIGGERED** - triggered(NO_RESET)
*    **NOT_DONE** - notTriggered()
*    **NOT_TRIGGERED** - notTriggered()
*    **FIRST_TRIGGER** - firstTrigger()
*    **RESET** - reset()
*    **RESET_TIMERS** - resetAllTimers()
*    **ENABLE** - enable()
*    **DISABLE** - disable()
*    **START** - start()
*    **STOP** - stop()
*    **SWAP** - negateState()
*    **FLIP** - negateState()
*    **SWAP_STATE** - negateState()
*    **FLIP_STATE** - negateState()
*    **ENABLED** - isEnabled()

You can, of course create your own macros within your code. So, for example, let's say that you wanted a macro that overrides the default reset behavior in the setDuration() method, which by default, will change the duration of the timer to your new value and will also reset the timer. But lets say you want to change the duration WITHOUT resting the timer and you wanted that to be done with a word that makes more sense to you.

```C++  
#define QUICK_CHANGE(value) myTimer.setDuration(value, false)  
  
QUICK_CHANGE(3200);  
```  

The only difference here, is that you cannot make a macro that applies universally to all of your timers. You would need to make one for each timer you have created. If you have ideas for macros that you think should be included in BlockNot, let me know and I'll see about adding it.

Thank you for your interest in BlockNot. I hope you find it as invaluable in your projects as I have in mine.    
<BR>Mike Sims  
sims.mike@gmail.com

## Version Update Notes
### 1.7.2

- Minor update
  - Added start() method.
  - Added stop() method. 
  - Added **START** and **STOP** Macros. 
  - Removed restrictions preventing changes to a timer when it is stopped (disabled). Changes made to a stopped timer will now work.


### 1.7.1

- Minor code enhancements that improves efficiency thanks to [@bizprof](https://github.com/bizprof)

### 1.7.0

- Updated version to 1.7 - It just made sense to do a full step since the latest re-write, which includes cosmetic code changes as well as normalizing repetitive code, has been fairly substantial and it includes the new resetAllTimers() (RESET_TIMERS) method with its bug fixes.<BR>
- **Bug Fix** - Fixed bug where invoking resetAllTimers() was causing an accumulated time drift fot all timers.<BR>
- **triggeredOnDuration()** - method added so that when this method is called, the timer is reset by startTime + duration instead of using the current millis() value. This helps guarantee that the timer will only trigger exactly by the duration that you have set for the timer. That way, if you call a test for the timer being triggered and the time that you do the test is some time after the timer actually triggered, then it's new startTime will not include that extra time burned between the actual trigger time and the time when you called the test.<BR><BR> This will help you implement timers that trigger more accurately as long as your delays between the timer triggering and the time you test for the trigger are not consistently lapsed or else you will eventually run into the problem where the trigger will happen twice in a row ... run through the logic and think it through before you use this method.<BR><BR>The macro for calling this method is myTimer.TRIGGERED_ON_DURATION

### 1.6.7

- **Bug Fix** - Fixed bug where declaring the timer with the time duration alone would not compile.<BR>

### 1.6.6

- **Reset ALL timers** with a single command - resetAllTimers(); <b>OR</b> RESET_TIMERS;<BR> See the discussion above in the documentation for more details.<BR>
Thank you [@bizprof](https://github.com/bizprof) for contributing this feature to the project.

### 1.6.5

- **SECONDS Mode** - Sometimes we only need to work with secons, which makes it pain in the rear to constantly have to work with numbers in the thousands because everything is in milliseconds. SO NOW, if you need to work just with whole seconds, you can declare your timer as a SECONDS timer when you instantiate it, like this:

 ```C++
 BlockNot myTimer(5, SECONDS);
 ```
Once the timer is established as a SECONDS timer, then any changes you make to it such as adding or removing time, or even setting the duration to a new number, must be done in seconds, not milliseconds.<BR><BR>
All time calculations under the hood continue to happen in millisecond units, I just included the proper math so that those units are converted from seconds to milliseconds and back on all related methods.<BR><BR>
Therefore, any methods that you use which return a length of time such as getDuration() etc., will be first calculated using milliseconds, then divided by 1,000 and returned to you, so however C++ rounds those numbers is how you will get them back. But when you're dealing with whole second timers, then fractional second precision should be irrelevant to you. If it matters, then don't use a SECONDS timer.

## Suggestions

I welcome any and all suggestions for changes or improvements. You can either open an issue, or code the change yourself and create a pull request. This library is for all of us and making it the best it can be is important! 
<BR><BR>You can also email me:<BR>sims.mike@gmail.com
