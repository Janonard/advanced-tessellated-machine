/*
 * advanced tesselated machine
 * Copyright (C) 2017 Janonard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <unistd.h>
#include "ATM/ATM.h"
#include "Executable/Executable.h"

void printUsage()
{
	std::cerr << "Usage:" << std::endl;
	std::cerr << "atm -r <path>" << std::endl;
	std::cerr << "atm -b <path>" << std::endl;
	std::cerr << "atm -h" << std::endl;
	std::cerr << std::endl;
	std::cerr << "-r <path>  Run an executable or an assembler file." << std::endl;
	std::cerr << "-b <path>  Build the executable or assembler file and save the result in <path>.atmx" << std::endl;
	std::cerr << "-h         Show the usage of atm." << std::endl;
}

bool loadExecutable(std::string pathToExe, std::shared_ptr<Executable>& exec)
{
	//
	// Load executable
	try {
		exec = std::shared_ptr<Executable>(new Executable(pathToExe));
	}
	catch (std::bad_alloc e)
	{
		std::cerr << "ERROR: Could not allocate memory for the executable" << std::endl;
		return false;
	}
	catch (LoadingException e)
	{
		std::cerr << "ERROR while loading the executable: " << e.what() << std::endl;
		return false;
	}
	
	return true;
}

bool runExecutable(std::string pathToFile)
{
	if (pathToFile == "")
	{
		std::cerr << "Error! No executable given!" << std::endl;
		printUsage();
		return false;
	}
	
	if (! buildSystem())
		return false;
	
	std::shared_ptr<Executable> exec(nullptr);
	
	if (! loadExecutable(pathToFile, exec))
		return false;
	
	if (! assignElements(exec))
		return false;
	
	motherboard->loop();
	
	if (! motherboard->deconfigure())
		return false;
	
	if (motherboard->getState() == Motherboard::State::CRASHED)
		return false;
	else
		return true;
}

bool buildSystem()
{
	// creating all nodes
	try
	{
		motherboard = std::unique_ptr<Motherboard>(new Motherboard);
		cpus[0] = std::unique_ptr<CPU>(new CPU);
		cpus[1] = std::unique_ptr<CPU>(new CPU);
		cpus[2] = std::unique_ptr<CPU>(new CPU);
		cpus[3] = std::unique_ptr<CPU>(new CPU);
		graphicsAdapter = std::unique_ptr<GraphicsAdapter>(new GraphicsAdapter);
		keyboardAdapter = std::unique_ptr<KeyboardAdapter>(new KeyboardAdapter);
		timeAdapter = std::unique_ptr<TimeAdapter>(new TimeAdapter);
	}
	catch (std::exception e)
	{
		std::cerr << "ERROR: Allocating memory for the system failed!" << std::endl;
		return false;
	}
	
	// pluging all nodes onto the motherboard
	motherboard->setNode(1,1,cpus[0].get());
	cpus[0]->setVerbosePointer(&(motherboard->_verbose));
	cpus[0]->setSlowPointer(&(motherboard->_slow));
	
	motherboard->setNode(2,1,cpus[1].get());
	cpus[1]->setVerbosePointer(&(motherboard->_verbose));
	cpus[1]->setSlowPointer(&(motherboard->_slow));
	
	motherboard->setNode(1,2,cpus[2].get());
	cpus[2]->setVerbosePointer(&(motherboard->_verbose));
	cpus[2]->setSlowPointer(&(motherboard->_slow));
	
	motherboard->setNode(2,2,cpus[3].get());
	cpus[3]->setVerbosePointer(&(motherboard->_verbose));
	cpus[3]->setSlowPointer(&(motherboard->_slow));
	
	motherboard->setNode(0,1,graphicsAdapter.get());
	graphicsAdapter->setWatchDirections({{RIGHT,DOWN_RIGHT}});
	
	motherboard->setNode(3,2,keyboardAdapter.get());
	keyboardAdapter->setWatchDirections({{LEFT,UP_LEFT}});
	
	motherboard->setNode(1,3,timeAdapter.get());
	timeAdapter->setWatchDirections({{UP,UP_RIGHT}});
	
	if (! motherboard->configure())
		return false;
	else
		return true;
}

bool assignElements(const std::shared_ptr<Executable>& exec)
{
	//
	// create empty element
	std::shared_ptr<ExecutableElement> emptyElement(nullptr);
	try
	{
		emptyElement = std::make_shared<ExecutableElement>();
	}
	catch (std::bad_alloc e)
	{
		std::cerr << "ERROR: Could not allocate memory for an empty element" << std::endl;
	}
	std::vector<uint8_t> emptyMemValues = std::vector<uint8_t>();
	emptyMemValues.push_back(0);
	emptyElement->setMemory(emptyMemValues);
	
	//
	// assign exec. elements to the processors.
	bool successfull = true;
	std::shared_ptr<ExecutableElement> element = exec->getElement(1,1,&successfull);
	if (successfull)
	{
		cpus[0]->setMemory(element);
	}
	else
	{
		cpus[0]->setMemory(emptyElement);
	}
	
	element = exec->getElement(2,1,&successfull);
	if (successfull)
	{
		cpus[1]->setMemory(element);
	}
	else
	{
		cpus[1]->setMemory(emptyElement);	
	}
	
	element = exec->getElement(1,2,&successfull);
	if (successfull)
	{
		cpus[2]->setMemory(element);
	}
	else
	{
		cpus[2]->setMemory(emptyElement);
	}
	
	element = exec->getElement(2,2,&successfull);
	if (successfull)
	{
		cpus[3]->setMemory(element);
	}
	else
	{
		cpus[3]->setMemory(emptyElement);
	}
	
	return true;
}

bool buildExecutable(std::string filePath)
{
	std::shared_ptr<Executable> exec(nullptr);
	
	if (! loadExecutable(filePath, exec))
		return false;
	
	try
	{
		exec->saveExecutable();
	}
	catch (LoadingException e)
	{
		std::cerr << "ERROR while saving the executable: " << e.what() << std::endl;
		return false;
	}
	
	return true;
}

int main(int argc, char** argv)
{
	std::string pathToFile("");
	bool runFile = false;
	bool buildFile = false;
	
	if (argc <= 2)
	{
		std::cerr << "Too few arguments given!" << std::endl;
		std::cerr << std::endl;
		printUsage();
		return -1;
	}
	
	int option;
	while ((option = getopt(argc,argv,"hr:b:")) != -1)
	{
		switch (option)
		{
			case 'h':
				printUsage();
				return 1;
			case 'r':
				pathToFile = std::string(optarg);
				runFile = true;
				break;
			case 'b':
				pathToFile = std::string(optarg);
				buildFile = true;
				break;
			case '?':
				std::cerr << "Unknown option -" << (char) optopt << std::endl;
				printUsage();
				return 1;
			default:
				break;
		}
	}
	
	if (buildFile)
	{
		if (! buildExecutable(pathToFile))
			return 1;
	}
	
	if (runFile)
	{
		if (! runExecutable(pathToFile))
			return 1;
	}
	
	return 0;
}
