# Keyboard Adapter

The keyboard adapter is an extension node, but it acts more activly. It has no commands,
but pushes every key or char action to the left, which is the processor (2|2). The return
values should be splitted into two parts, since the first half of the first byte contains
the event type and the rest contains the event code, usually the key or character.

## `0x0` Key pressed and `0x1` Key released

When a key is pressed, the keyboard adapter will fire an event like this: `0x0041`, which
means that the key 'A' has been pressed. When the user releases the key, the adapter will
fire: `0x1041`, which means that 'A' has been released. It's quite simple, but has some
restrictions:

The range of "pressable" keys is quite limited, which is an emulation problem: Every
real keyboard may have different keys and when you are waiting for a key to be pressed
that does not exist on the real keyboard, you can wait for a long time. Therefore, the
range of registered characters is limited to those that assumly exist on every keyboard.

These keys are:

* all alphabetical keys (A-Z), coded in ASCII code
* all numerical keys (0-9), coded in ASCII code
* function keys from F1 to F12, coded as `F1=0x122` to `F12=0x12D`
* arrow keys, coded as `right=0x106`, `left=0x107`, `down=0x108`, `up=0x109`
* `escape=0x100`, `enter=0x101`, `backspace=0x103`

Yes, you read right: There are no modifiers like shift or control, and no special characters
at all. You might say: "I want to enter texts and I need these characters!" and you're right:
For texts, these characters are needed, but these events are meant for games or other purposes
that need the keyboard like a controller. When you want to enter text, you should use:

## `0x2` Character entered

This event is fired when a character was entered. This means, that all characters, that fit
into the transaction scheme and therefore are lower or equal `0xFFF`, can be used. It even applies
the shift key!
