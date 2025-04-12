#MeowTime
For a long time I have really really wanted a routine app on my pebble
smartwatch! I have a routine app on my phone but find that my mornings are
either ruined because I either:

- I follow the routine app on my phone, and then get distracted by it.
- I don't follow the routine app on my phone, and forget to do my routine, or forget to do everything in my routine.

Modern smartwatches are bulky, slow, and always on the charger. The pebble is
the best smart watch for me - it manages time without being distracting. And so
I thought of building MeowTime! When do we start doing our routines? MEOW!

## Pebble Hackthon #002

This is a routine on the pebble made for The Pebble Hackathon #002 which began
shortly after it was announced that the pebble was coming back!

Even though I bought a wearos watch, I kept finding myself going back to the
pebble because it has a long battery life (even after all these years, it still
lasts 3-4 days). Other smartwaches run out of battery and aren't able to do the
one main thing they're supposed to do - tell you the time. Dead battery tells no
time. Additionally:

- Since it didn't run out of battery quickly, I was less likely to forget it on the charger.
- The form factor made it easy to sleep with and use it as an alarm that doesn't disturb others.
- It had buttons so I could use it outside with gloves.
- It replaces a distracting phone and takes care of - telling time, setting alarms, doing pomodoro, and seeing notifications at a glance.

## To build and run

(If on mac)

Install rebble-tools

```sh
#To build and load on the emulator
rebble build && rebble install --emulator aplite

#to see the logs
rebble logs
```

## To fotmat code

Install clang-format then in project root folder run:

```
clang-format -i src/c/*
```
