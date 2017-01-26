# Usage of ATM

In short, these are the options for ATM:
You can use only one of those at a time.

	atm -r <path>
	atm -b <path>
	atm -h
	
## The Options

### -r: Run executable or assembler file

Using this option, the emulator will load the given file and run it. It does not care if it's an
executable or assembler file, you can run both instantly.

### -b: Build assembler file

This option makes ATM loading and assembling the given file. Afterwards it will save an executable
at <path>.atmx. Again, it does not care if it's an executable or assembler file.

### -h: Help

Using the help option will lead to ATM showing it's usage and terminating. All other options have
no effect when it is used.
