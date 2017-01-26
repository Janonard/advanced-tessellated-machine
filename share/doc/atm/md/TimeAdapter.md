# Time Adapter

The time adapter handles time access and is therefore quite simplistic.
It supports three commands:

	code	name			returns
	0		updateTime		0
	1		getUnixTime		2
	2		getMicroTime	2

Unlike many other extension nodes, this does not take arguments, which means
that when you want to get the Unix time, you need to send `0x0001` and not
`0x0100`.

Internally, it has a time register that needs to be updated via `updateTime`
and is accessed by `getUnixTime` and `getMicroTime`.

`getUnixTime` returns the [Unix time](https://en.wikipedia.org/wiki/Unix_time)
in seconds since "the epoch". Since the Unix time is a 32-bit number, it needs
to be pushed in two values. Like in many other aspects, ATM is oriented in
a small endian way: The highest two bytes get sended first, followed by the 
lower ones.

`getMicroTime` increases the accuracy of `getUnixTime`, since it returns the
time in microseconds since the the last Unix time second was reached. You
could also say that the micro time is the Unix time in microseconds, modulo 1000000.
The micro time needs to be pushed with two values two, which follows
the same rules as `getUnixTime` does.
