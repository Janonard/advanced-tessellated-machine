# Installation

ATM currently supports Fedora and Ubuntu, if you have another Distro or Windows, you have to wait
or port it on your own. Also, you need a graphics card supporting OpenGL 3.2.

Since there are no packages of ATM yet, you need to download and build ATM on your own. Don't worry,
this sounds harder than it is. All you need is moving around in bash and entering some commands.

The first step is to open a terminal and install the dependecies. You can do this with these commands:

	# Fedora
	sudo dnf install git cmake clang glfw-devel SOIL-devel libGLEW-devel boost-devel
	# Ubuntu
	sudo apt-get update && sudo apt-get install git cmake clang libglfw3-dev libsoil1-dev libglew-dev libboost-devel

When these commands have finished, you shall download ATM:

	git clone https://github.com/Janonard/advanced-tessellated-machine.git

Next you enter the source code, create a build directory and build it.

	cd advanced-tessellated-machine
	mkdir build
	cd build
	cmake ..
	make

This should take a while. After that you should test your build by entering

	make test

All tests should pass. In the end, you install everything with this command:

	sudo make install
	
You're done! Was it really so hard?
