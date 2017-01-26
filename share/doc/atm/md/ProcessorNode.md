# Processor Node

The processors are the core of the ATM system, as they are programmable and controll all other
nodes.

## Registers

An ATM processor has four main and four additional registers, which are all 16 bits in size but
differ in usage and accessibility. These main registers are:

* `null`
* `acc`
* `bcc`
* `ccc`

and the additionals are:

* `acc swap`
* `bcc swap`
* `ccc swap`
* `program pointer`, short `pp`

The main registers are designed for reading, writing and adding values to them, but `null` is quite
special: When you read from it, it will always be zero, and when you write something to it, it will
vanish in the void. When talking about registers in general, these main ones are meant.

Those registers, except the `null` register may also be used as pointers like you know them from C
or C++: They hold an address that points to a part of the memory. When you want to use the values
of the pointed memory instead of the values contained in the registers, you write:

* `$acc`
* `$bcc`
* `$ccc`

Every of those has a swap register. The only purpose of those registers is to exchange their content
with the main one.

The `program pointer` controls the execution flow. When a tick begins, the processor will load the
byte the `pp` points to and execute it as a command. Also, it will load arguments after the command
byte if needed. After this is done the processor will increase the `pp` by the number of loaded
bytes.

## Directions

Since every node has connections, the processor has some too! They are accessible via the keywords:

	code	name
	
	0x00	up
	0x01	up_left
	0x02	left
	0x03	down_left
	0x04	down
	0x05	down_right
	0x06	right
	0x07	up_right

## Flags

The processor knows three flags:

* `operation successfull`, short `ops`
* `greater`
* `lower`

You can think of these flags as boolean values which are used by several commands. Their use is
explained in the command definitions.

## Stacks

The `branch stack` is used by `branch` and `return` commands and contains all jump points.

## Memory

The memory is the processor's main data storage that contains both programs and variables.
It is accessible via the commands `move8` and `move16`, which read or write an 8-bit or 16-bit value
from or to the memory. More details are available in the description of those commands.

The size of the memory is limited to 64k since addressing is done with 16-bit integers. Bank
switching is currently not available.

## Commands

This is a list of all commands a processor understands, which does not include special assembly
mechanics. If you want to know something about those, you should read the assembly language
documentation.

All subtopics contain a listing with every related command, containing it's code and it's size with
parameters.

### halt and splash

	code	size	name
	
	0x00	1		halt
	0x01	1		splash
	
`halt` and `splash` are some very basic control commands: When the processor uses `splash`, nothing
happens, but `halt` will stop the processor from running and when all processors halted, ATM itself
will halt too and the emulation will be closed.

### move

The `move` commands are the most important and complicated ones in the ATM instruction set, but they
have two basic rules: First, there must always be at least one register involved, and second, the
original value is never changed. With this in mind, you will understand the following listings way
better.

#### direct memory access

	code	size	name	param0			param1
	
	0x02	1+2+1	move8	<address>		<register>
	0x03	1+2+1	move16	<address>		<register>
	0x0A	1+2+1	move8	<register>		<address>
	0x0B	1+2+1	move16	<register>		<address>
	
	
	Examples:
	
	move8 $0000 acc
	move16 acc $4242

Direct memory access is done with an address and a register: When reading, the processor will read
the value at the given location and load it into the given register, when writing, it will do it the
opposite way.

This access comes in two different types: 8 and 16 bit. The processor always uses little endian
encoding, this means that when you want to store the 16-bit integer `0x0102` in `0xFF00`, the
processor will store `0x01` in `0xFF00` and `0x02` in `0xFF01`. The same goes the other way: When
you have `0x01` in `0xFF00` and `0x02` in `0xFF01`, the the register's content will be `0x0102`.

This is the way you write addresses: you take a dollar sign and four hexadecimal digits. Note that
you always have to write all digits, otherwise the assembler will get angry!

The 8-bit versions cut everything they do not need: When you have `0x0102` in your register and
you want to store an 8-bit value, it will be `0x02` in the memory. Just remember that.

#### indirect memory access

	code	size	name	param0			param1
	
	0x04	1+1+1	move8	<regAddress>	<register>
	0x05	1+1+1	move16	<regAddress>	<register>
	0x0C	1+1+1	move8	<register>		<regAddress>
	0x0D	1+1+1	move16	<register>		<regAddress>
	
	
	Examples:
	
	move16 $acc bcc
	move8 ccc $bcc
	
Indirect memory access follows the same rules as the direct memory access, with the exception that
the used address is read from a register instead of beeing hardcoded in the memory.

#### loading hardcoded numbers

	code	size	name	param0			param1
	
	0x07	1+1+1	move	<8bitInt>		<register>
	0x08	1+2+1	move	<16bitInt>		<register>
	
	
	Examples:
	
	move #42 acc
	move #FFFF bcc
	
The processor also has a way to load hardcoded 8- and 16-bit integers. As you might have guessed,
there is opposite direction to move a value from a register to a number. :)

You write numbers almost like addresses but with two differences: First, you put a number sign, some
also call it hashtag, infront of it and you can write them with two or four digits. Using two digits
results in using the 8-bit version, using four results in using the 16-bit version.

#### channel access

	code	size	name	param0			param1
	
	0x06	1+1+1	move	<channel>		<register>
	0x0E	1+1+1	move	<register>		<channel>
	
	
	Examples:
	
	move up acc
	move acc down
	
There are also `move` commands for channel access. They will try to read/write a value and set the
`ops`-flag accordingly, so you can handle push/pull-errors.

#### register interchange
	
	code	size	name	param0			param1
	
	0x09	1+1+1	move	<register>		<register>
	
	
	Example:
	
	move acc bcc

The value of a register can also be moved to another register which is done by this command.

### add

The `add` is a special `move` command, which adds the content of `param0` to the content of `param1`
and saves it into `param1`. Also, `param1` must be a register, what shrinks the number of possible
`add`s alot. When you are not sure how exactly these commands work, you can look it up in the reading
versions of `move` since both act the same.

#### direct memory access

	code	size	name	param0			param1

	0x0F	1+2+1	add8	<address>		<register>
	0x10	1+2+1	add16	<address>		<register>
	
	
	Example:
	
	add8 $4242 acc

#### indirect memory access

	code	size	name	param0			param1

	0x11	1+1+1	add8	<regAddress>	<register>
	0x12	1+1+1	add16	<regAddress>	<register>
	
	
	Example:
	
	add16 $acc bcc

#### loading hardcoded numbers

	code	size	name	param0			param1

	0x14	1+1+1	add		<8bitInt>		<register>
	0x15	1+2+1	add		<16bitInt>		<register>
	
	
	Example:
	
	add #01 acc

#### channel access

	code	size	name	param0			param1

	0x13	1+1+1	add		<channel>		<register>
	
	
	Example:
	
	add up acc

#### register interchange

	code	size	name	param0			param1

	0x16	1+1+1	add		<register>		<register>
	
	
	Example:
	
	add acc bcc

### negating registers

	code	size	name	param0
	
	0x17	1+1		negate	<register>
	
	
	Example:
	
	negate acc
	
You can negate registers by using this command. This command is especially usefull when you want to do
a subtraction: Just load the value into a register, negate it and add it to the other register.

### shifting

	code	size	name			param0
	
	0x18	1+1		bit_shift_l		<register>
	0x19	1+1		byte_shift_l	<register>
	0x1A	1+1		bit_shift_r		<register>
	0x1B	1+1		byte_shift_r	<register>
	
	
	Example:
	
	byte_shift_r acc

Shifting a value means that all digits are moved to the left or to the right by once. There are two
different types of shifting: Byte shifting and Bit shifting. When you have `0x00F0` in your register
and shift it bytewise to the right, it becomes `0x000F`. Bit shifting does the same, but only by one bit
instead of a whole byte. This way, your register would get the value `0x0078`.

### swaping

	code	size	name	param0
	
	0x1C	1+1		swap	<register>
	
	
	Example:
	
	swap ccc
	
This command exchanges the content of the register with the content of it's swap register. This means that
when you have `0x1111` in `acc` and `0x2222` in `accSwap` and you swap them, you will get `0x2222` in `acc`
and `0x1111` in `bcc`.

### comparing and jumping

	code	size	name			param0		param1
	
	0x1D	1+1+1	compare			<register>	<register>
	0x1E	1+2		jump			<address>
	0x1F	1+2		jump_ops		<address>
	0x20	1+2		jump_opf		<address>
	0x21	1+2		jump_greater	<address>
	0x22	1+2		jump_lower		<address>
	0x23	1+2		jump_equal		<address>
	0x24	1+2		jump_nequal		<address>
	
	
	Example:
	
	compare acc null
	jump_equal $FFFF

Processors can change their program flow by jump according to the state of the flags. Or to say it in another
way: The `pp` can be set to a value, which lead to the processor executing different parts of the program.

`compare` is able to compare the content of two registers and to set the flags `greater` and `lower` accordingly.
If the content of `param0` is greater than the content of `param1`, the `greater` flag will be set to `1` and
the `lower` flag to `0`; the same goes the other way when the content of `param0` is lower than the content of
`param1`. When the contents are equal, both flags will be set to `0`.

The first jump does not care about any flags, it simply executes the jump. If the last action, usually a channel
access, was successfull, `jump_ops` will execute a jump, if not, `jump_opf` will. The other jumps are self-explaining.

If no jump was executed, the program will continue as if nothing happened.

### Branch and Return

	code	size	name	param0
	
	0x25	1+2		branch	<address>
	0x26	1		return
	
	
	Example:
	
	branch $4242
	return
	
Branching is a bit like jumping but with two differences: First, it does not react on flags, which means every branch
will be executed, and second, that the old position will be saved. This way the processor can return to the last call
when it is done with a part of the program.

These two commands are very usefull when you write subroutines. For example: You have written a small piece of code
that multiplies two values. Now you can branch to it from everywhere and your program will proceed when the multiplication
is done.

### Debugging commands

	code	size	name		param0
	
	0x27	1+1		dVerbose	<8bitInt>
	0x28	1+1		dSlow		<8bitInt>
	0x29	1		dCrash
	
	Example:
	
	dVerbose #01
	dSlow #00
	dCrash

The debugging are a very unrealistic part of ATM: The make no sense at all from a technical point of view, but are very
usefull when debugging a program.

`dVerbose` turns the verbose mode on or off. When in verbose mode, the emulator will
print a list containing all information about all nodes that are currently running every system tick. This way you
can print the state of one processor or a step in program execution by turning verbosity on and off shortly or print
a whole routine.

`dSlow` is especially usefull for the second condition. It will set the ticking speed of the system to a
[configurable](Configuration.md) debugging tick speed.

The parameter is always meant as boolean value: `0` means `false` and `1` or greater means `true`.

`dCrash` was added to be able to write tests: Programs that do something and end properly when everything is okay
and fail when this isn't the case. The processor executes this command, the complete emulation will crash. Use at your
own risc!
