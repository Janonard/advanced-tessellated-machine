# Configuration

ATM is configurable by a config file located in your home directory: `~/.atm`

The Syntax of this file is quite easy: `name=value`. You write the name of the value, an equals sign
and the value you want to set it to. For example when you want to set the tick speed to 500 Hz, you
add the line `tickSpeed=500` to the file.

These are the configurable values with their value type and their effect:

* `tickSpeed` (float, default=`1000000.0`): Sets the tick speed limit in Hertz
* `debugTickSpeed` (float, default=`1.0`): Sets the targeted tick speed limit in Hertz for the debugging mode.
* `screenSizeMultiplier` (float, default=`1.0`): When the window is created, the base window sizes will be multiplied by it.
* `fullscreened` (bool, default=`false`): When true, the window will be created as a fullscreened one. Since the resolution of the main display will be used as the window size, `screenSizeMultiplier` will have no effect.

