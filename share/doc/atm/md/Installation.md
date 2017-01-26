# Installation

## Requirements

ATM currently only supports Linux, what means that when you have Windows, you have to wait a while
until it has been ported. There are builds provided for 32 and 64-bit PCs and a graphics card
supporting OpenGL version 3.2 is required too. 

## Download

You can download ATM from it's [github release page](https://github.com/Janonard/advanced-tessellated-machine/releases)
and you can choose between an RPM package and a TAR.GZ archive. When you have Fedora or SUSE, you
should choose the RPM package, otherwise, the TAR.GZ is your choice. If you don't know whether your
PC is a 32 or a 64-bit machine, you should choose the 32-bit version since it works on 64-bit PCs
too.


## Installation

### RPM

After you have downloaded ATM, you only have to run this command on the package:

	sudo dnf install ./your-package.rpm

When you don't have rpm, you can do it with yum too:

	sudo yum install ./your-package.rpm
	
Naturally, you have to replace `your-package` with the proper name of your package.

### .tar.gz

As mentioned before, this thing is a bit more complicated. First, you have to install the required software,
which is: `glfw` version 3.2, `soil` version 1.07, `glew` version 1.13 and `boost_filesystem` version 1.60. Newer version
will do it too, older maybe, but try to get these versions, since those are the versions I use. Installation may vary
from system to system, you propably know yours better than I do.

Next, you should unpack the archive. You can use the "unpack here" variant of your favorite package manager, since
it contains a internal folder. After doing this, you open a terminal in this folder and enter this:

	sudo cp -r bin /usr/local/bin/
	sudo cp -r share /usr/local/share/

These commands copy the binary and shared files into the standart directories.
