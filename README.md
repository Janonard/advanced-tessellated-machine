# advanced tesselated machine

Welcome to the world of the advanced tesselated machine!

ATM is a computer architecture focused on having fun writing assembler programs.
It features four 16-bit processors with 64k of Memory each, text output and keyboard access,
a bit like the old computers did!

But while the interesting aspects of these old systems are
included, the annoying and boring aren't! After a bit of learning, almost every command is
self-explaining and you can produce clean, nice-to-read assembly code.

This doesn't say that programming for ATM is easy, of course it isn't! The cost of having
full controll over everything is that you have to do almost everything on you own: No standart
libraries, no classes, no functions, not even loops! This makes ATM easy to learn, but hard to
master, an old video game design philosophy used since the 1970s.

Still interested? Well then, just start with it by reading the [docs](share/doc/atm/md/README.md)!

Do you want to start modifying the code to your wishes? You can, since ATM is published under
the terms of the [GNU Public License](LICENSE).

## Building the code

To build the code, you need some software. Only Linux is supported, Windows not yet
and Mac propably never will.

When you are on Ubuntu/Debian/etc., install the software by entering this into a terminal:

	sudo apt-get update && sudo apt-get install git cmake clang libgflw-dev libsoil-dev libglew-dev
	
When you are on Fedora, you use this instead:

	sudo dnf install git cmake clang glfw-devel SOIL-devel libGLEW-devel

After you have done this, you should download the source code with this command:

	git clone https://github.com/Janonard/advanced-tesselated-machine.git

Then you use these commands to create a build directory and compile the code:

	cd advanced-tesselated-machine
	mkdir build
	cd build
	cmake ..
	make

When you want to install the binaries after building, you use:

	sudo make install
