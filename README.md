# Elite Dangerous Keypad

I started this project as a way to add more buttons to a HOTAS controller I use to fly my space ships in the game [Elite: Dangerous](https://www.elitedangerous.com/).

The original extension for the controller was just an el-cheapo numeric keypad on top of a small support mede of Lego. Then I bought an Arduino and made [this button pad](http://imgur.com/a/oEdEI).

To transform the Arduino into a USB keyboard, I started with the instructions from [Mitch Tech](http://imgur.com/a/oEdEI). Visit his site for more instruction on how to upload the firmwares and use them.

The code I wrote allows for 6-key roll over (it can send up to six simultaneous key presses) and alternate modes, selected by the flip switches. Which means each key can send 5 diferent scan codes, based on the position of the switches.

The scan codes for each key can be found on Section 10 of the official [USB Usage Tables](http://www.usb.org/developers/hidpage/Hut1_12v2.pdf)(.pdf).

You can adapt this for many other things. In the future, I may add Macros, to send combinations of keys with each keypress.
