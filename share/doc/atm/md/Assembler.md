# Assembler

The assembler is a program, that reads your source code, assembles it and writes
the output into a executable file.

An assembly source code is a list of commands which can be directly translated
to machine code. All [processor commands](ProcessorNode.md) are supported, but
there are also some additional features.

## The first file

The first file, the one that you are entering as a argument to atm, has one special

It let's you choose for with node you are programming for: When you write `NODE 1 1`,
all following lines will be executed by the processor at the position `(1|1)`, until
the file ends or you switch the node again. You have to write `NODE X X` excatly as shown,
since the executable loader will process it and he's not as a handy person as the assembler
is.

## Binary size limit

A binary may not be bigger than 0xFFFF = 65535 Bytes, because everything beyond would be
unreachable for the processor. The assembler will throw an error when hit this size.

## Additional features and commands

### symbols

The symbol system is very usefull for controlling the program flow and static
memory allocation. You can add a symbol wherever you want by writing something
like:
	
	symbol: jump symbol

By writing the symbol's name and a ':' infront of a command, you create a symbol
with the given name that points to the memory location of this command.

Symbols may be used as adressess, that means that you can use a symbol everwhere
the processor needs an address. Also, you can use the symbol's value as a 16-bit
number by surrounding it with number signs '#'.

You can also define a symbol staticly by using this command:

	#define <symbol> <address>

This creates a symbol with the given address. Since it is a meta-command that
starts with a number sign, it has no binary translation and will not appear in
the executable.

Symbol names may not be a keyword which may confuse the assembler like the
registers or channels and may not start with `#`, `$` or `@`. Also, two symbols
must not have the same name.

### memory manipulation

There are these meta-commands that manipulate the resulting memory directly:

	#var		<8bitInt>
	#var		<16bitInt>
	#position	<address>
	#space		<address>
	
The `#var` commands copy the given integer values into the memory, which is
espacialy usefull for hardcoding strings, values, etc.

`#position` and `#space` are quite similar but also quite different. `#position`
adds zeros until the given address in the memory is reached. The next command
will have the given address. When the command cannot reach the given address, 
since the starting address is bigger than the desired, the assembler will give
an error. `#space` adds the given amount of zeros to the memory.

### file inclusion

The assembler can include external files via these meta-commands:

	#include	<filename>
	#binary		<filename>

You clarify a filename by surrounding the path with `<` and `>`. Including a
file with `#include` leads to it's assembly and inclusion the to final binary,
while `#binary` simply copies the contents of the file into the binary.

The paths are relative to the including file, therefore when you `#include <routines/foo.atma>`
in file `/home/bar/test.atma`, the assembler will look for `/home/bar/routines/foo.atma`
and will cry if it does not find it.
