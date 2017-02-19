# Installation

ATM currently supports Fedora and Ubuntu, if you have another Distro or Windows, you have to wait
or port it on your own. Also, you need a graphics card supporting OpenGL 3.2.

Since there are no packages of ATM yet, you need to download and build ATM on your own. Don't worry,
this sounds harder than it is. All you need is moving around in bash and entering some commands.

To build the code, you need some software. When you are on Ubuntu, install the software by entering
this into a terminal:

	sudo apt-get update && sudo apt-get install git cmake clang libgflw-dev libsoil-dev libglew-dev libboost-filesystem-dev
	
When you are on Fedora, you use this instead:

	sudo dnf install git cmake clang glfw-devel SOIL-devel libGLEW-devel boost-devel

After you have done this, you should download the source code with this command:

	git clone https://github.com/Janonard/advanced-tesselated-machine.git

Then you use these commands to create a build directory and compile the code:

	cd advanced-tesselated-machine
	sudo ./install_shared_files.sh
	mkdir build
	cd build
	cmake -D CMAKE_BUILD_TYPE=RELEASE ..
	make
	
Maybe you want to test whether you binaries are okay or not? Use this!

	sudo ../install_shared_files.sh
	make test

When you want to install the binaries after building, you use:

	sudo make install
