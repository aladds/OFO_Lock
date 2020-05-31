# OFO_Lock
Reprogram an OFO bike lock to unlock with a numeric code

Since the OFO company are gone in the english-speaking world (and many others) since 2018, and there are a few bikes still around, it seems a shame that they were going unused.

Disclaimer: The bicycle I have access to was legally acquired, yet I still attempted to contact the company after doing so. They did not respond.

Sadly many of them have been trashed or disposed of, but occasionally you come across an example which is usable (via. purchase or another means). Thankfully there are some resources out there helping keep them going: https://bikesharemuseum.com/fix-an-ofo-bicycle/

I took particular interest in the lock, since it's got to have at least Bluetooth, GPS and 2G for connectivity.

Turns out it does, although it's actually got 4G, and it also has an accelerometer.

Based on the above repair guide, the locks are easy to remove using a fro-wing screwdriver. Once open, the lock barrel springs back and the bike can be recovered. 

Open the lock with 5 Philips head screws on the rear. 

At this point the lock battery is probably rather flat, so remove the battery, keep the wire (clip it off near the battery) and recycle the old battery.

Now you need to disconnect the leads connecting the top and bottom halves of the unit - most can be disconnected straight away (the speaker and 4G antenna) but the Bluetooth antenna is on the rear. For that you need to remove 2 Philips screws which go through the PCB. Do this, remove the PCB and disconnect the antenna. Then remove the rest of the connectors from the PCB to free it from the rest of the lock. Set everything else aside now. 

On the PCB are connectors for the St-LinkV2 debugger - SWCLK and SWDIO - you just need to connect these to your debugger, along with GND and 3.3v power - remember the battery lead? Connect that there. 

Then load up openocd on your PC. At this point, if you’re keen, you can do something like https://www.optiv.com/blog/automated-unlocking-nrf51-series-socs-nrfsec to download the old firmware to your machine for analysis. I’ve done that (manually) and it works - so I could restore this lock to factory settings if I wanted to.

But I just did this in openocd:

nrf51 bulk_erase

Which blanks the chip. Now you’re ready to program it!

I got an Arduino framework (and recommended BLE stack) from here: 

https://github.com/sandeepmistry/arduino-nRF5

And I used this library to send it to sleep:

https://github.com/mristau/Arduino_nRF5x_lowPower

With these together I wrote the enclosed code which seeps, wakes up on pressing the “back” button and hangs around for 10 seconds for you to punch in the code and it to unlock. Then it goes back to sleep. In sleep the whole board draws 0.3mA, which should give me about 6 months from the 1200mAh battery I installed (it would be longer but when it’s powered up it takes 6mA, and the lock unlocking peaks at ~50mA for a couple of seconds). You’ll want something bigger if you plan on using wireless. 

I’ve not yet played with the Bluetooth (Beyond simple debugging) the 4G, GPS or accelerometer. But the external peripherals are all disabled until you power them up by setting their control pins low. 

I have an incomplete map of how things are connected too.

See the code in action:

https://youtu.be/dOF5OYbdcFc
