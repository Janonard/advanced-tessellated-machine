# Graphics Adapter

The graphics adapter features a possiblity to write text characters on the screen like old computers
like the [Apple 2](https://en.wikipedia.org/wiki/Apple_II) or the
[Commodore 64](https://en.wikipedia.org/wiki/Commodore_64) and is able to display any
[ASCII character](https://en.wikipedia.org/wiki/ASCII).

## The Array

The characters are ordered in an array of 30 by 24 elements. All characters are 19 pixels wide and
high, which leads to an overall resolution of 570 by 456 pixels with an aspect ratio of 5:4. This
array will be written to the screen at a framerate of 20 FPS.

Character access is done by a pointer: It points to a character and when you write or read a character
to or from th graphic adapter, the pointer character will be accessed.

The characters are counted from the left to the right and from the top to the bottom, what means
that the first char is the one in the upper left and the last is the one in the lower right corner.
The counting goes on as if there were no lines at all, which means that the first character of
the first line has the number `0`, the one of the second line has the number `30` and so on.

Usually, characters are written in strings, which is why there is the progress mode: After a
character access, the pointer will be increased by one. Translated to the screen it would mean that
pointer moves from the left to the right and jumps to the left end of the new line when it reaches
the end old one.

The processors are able to modify it by using multiple commands which are passed over the connections.
Some of these commands require an argument, and to reduce transaction time, both command and argument
are contained in one 16-bit integer. The first/upper byte contains the command and the second/lower
one the argument.

	code	name					argument	returns?
	
	0x00	getProgressMode			-			bool
	0x01	setProgressMode			bool		-
	0x02	getPosition				-			uint16
	0x03	setPositionUpperByte	uint8		-
	0x04	setPositionLowerByte	uint8		-
	0x05	getChar					-			uint8
	0x06	setChar					uint8		-
	0x07	scrollLine				-			-

The internal `bool`s of ATM are meant in such a way that when they equal zero, they are `false`, and
when they are bigger, they are `true`. Just keep that in mind.

Since the position pointer needs to be 16 bits big, it is splitted into a upper and a lower byte
that needs to be updated separatly. On the other hand there is no such regulation for the getter,
since returns may use the whole 16 bits.

The `getChar`/`setChar` need no deeper explanation: These `int`s are just normal ASCII characters.
