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
#define GLEW_STATIC
#include <GL/glew.h>
#include "System/Motherboard.h"
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <pwd.h>

using namespace std;
using namespace std::chrono;

void Motherboard::glfwErrorCallback(int error, const char* description)
{
	cerr << "ERROR: " << description << endl;
}

Motherboard::Motherboard() :
	_verbose(false),
	_slow(false),
	_nodes(),
	_tickSpeed(1000000),
	_slowTickSpeed(1),
	_context(nullptr),
	_screenSizeMulitplier(1.0),
	_state(RUNNING)
{
	array<Node*, motherboardNodesY> emptyNodeArray;
	emptyNodeArray.fill(nullptr);
	this->_nodes.fill(emptyNodeArray);
}

Motherboard::State Motherboard::tick()
{
	if (this->_state != RUNNING)
		return this->_state;
	
	if (this->_verbose)
	{
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				if (this->_nodes[x][y] != nullptr)
				{
					string debugString = this->_nodes[x][y]->getDebugString();
					if (debugString != "")
						cout << x << "/" << y << ": " << this->_nodes[x][y]->getDebugString() << endl;
				}
			}
		}
		cout << "=====================================================" << endl;
	}
	
	State newState = HALTED;
	
	steady_clock::time_point startTime = steady_clock::now();
		
	// pretick
	for (int x = 0; x < motherboardNodesX; x++)
	{
		for (int y = 0; y < motherboardNodesY; y++)
		{
			if (this->_nodes[x][y] != nullptr)
			{
				if (this->_nodes[x][y]->pretick() == false)
				{
					newState = CRASHED;
				}
			}
		}
	}
	
	// tick
	for (int x = 0; x < motherboardNodesX; x++)
	{
		for (int y = 0; y < motherboardNodesY; y++)
		{
			if (this->_nodes[x][y] != nullptr)
			{
				if (this->_nodes[x][y]->tick() == false)
				{
					newState = CRASHED;
				}

				if ((x > 0) && (x < (motherboardNodesX-1)) && (y > 0) && (y < (motherboardNodesY-1)))
					if (this->_nodes[x][y]->isRunning() && newState != CRASHED)
						newState = RUNNING;
			}
		}
	}
	
	timespec sleepDuration = timespec();
	sleepDuration.tv_sec = 0;
	
	if (this->_slow)
		sleepDuration.tv_nsec = (long) ((1000000000 / this->_slowTickSpeed) - duration_cast<nanoseconds>(steady_clock::now() - startTime).count());
	else
		sleepDuration.tv_nsec = (long) ((1000000000 / this->_tickSpeed) - duration_cast<nanoseconds>(steady_clock::now() - startTime).count());
	
	if (sleepDuration.tv_nsec > 0)
	{
		nanosleep(&sleepDuration, NULL);
	}
	
	glfwPollEvents();
	if (glfwWindowShouldClose(this->_context) && newState != CRASHED)
		newState = HALTED;
	
	this->_state = newState;
	return newState;
}

void Motherboard::loop()
{
	while (this->tick() == RUNNING)
	{
		// do nothing!
	}
}

bool Motherboard::configure()
{	
	std::string configFilePath;
	passwd *pw = getpwuid(getuid()); // getting the home dir
	char *homedir = pw->pw_dir;
	configFilePath = std::string(homedir);
	configFilePath += "/";
	configFilePath += motherboardConfigFile;
	
	Config configuration = Config::loadFromFile(configFilePath);
	
	if (! this->configureYourself(configuration))
		return false;
	
	// gl context creation
	glfwSetErrorCallback(&Motherboard::glfwErrorCallback);
	
	if (! glfwInit())
		return false;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	if (this->_fullscreenWindow)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
		this->_context = glfwCreateWindow(vidmode->width,
											vidmode->height,
											"advanced tesselated machine",
											monitor,
											nullptr);
	}
	else
	{
		this->_context = glfwCreateWindow(motherboardScreenSizeX*this->_screenSizeMulitplier,
											motherboardScreenSizeY*this->_screenSizeMulitplier,
											"advanced tesselated machine",
											nullptr,
											nullptr);
	}
	
	if (this->_context == NULL)
		return false;
	
	glfwMakeContextCurrent(this->_context);
	
	glewExperimental = true;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		cerr << "ERROR: Loading OpenGL failed!" << endl;
		if (! glewIsSupported("GL_VERSION_3_2"))
		{
			cerr << "Your system does not support OpenGL 3.2, updating your drivers may help!" << endl;
		}
		return false;
	}
	
	for (std::array<Node*,4> nodeLine : this->_nodes)
	{
		for (Node* node : nodeLine)
		{
			if (node != nullptr)
			if (! node->configure(configuration, this->_context))
			{
				return false;
			}
		}
	}
	
	return true;
}

bool Motherboard::deconfigure()
{
	for (std::array<Node*,4> nodeArray : this->_nodes)
	{
		for (Node* node : nodeArray)
		{
			if (node != nullptr)
			{
				if (! node->deconfigure())
				{
					return false;
				}
			}
		}
	}
	
	return true;
}

bool Motherboard::configureYourself(const Config& configuration)
{
	std::string entryBuffer;
	if (configuration.getEntry("tickSpeed",&entryBuffer))
	{
		try
		{
			this->_tickSpeed = std::stof(entryBuffer);
		}
		catch (std::exception e)
		{
			cerr << "ERROR: Could not read 'tickSpeed==" << entryBuffer << "' as a float value!" << endl;
			return false;
		}
	}
	
	if (configuration.getEntry("debugTickSpeed",&entryBuffer))
	{
		try
		{
			this->_slowTickSpeed = std::stof(entryBuffer);
		}
		catch (std::exception e)
		{
			cerr << "ERROR: Could not read 'debugTickSpeed==" << entryBuffer << "' as a float value!" << endl;
			return false;
		}
	}
	
	if (configuration.getEntry("screenSizeMultiplier",&entryBuffer))
	{
		try
		{
			this->_screenSizeMulitplier = std::stof(entryBuffer);
		}
		catch (std::exception e)
		{
			cerr << "ERROR: Could not read 'windowedScreenRatio==" << entryBuffer << "' as a float value!" << endl;
			return false;
		}
	}
	
	if (configuration.getEntry("fullscreened",&entryBuffer))
	{
		if (entryBuffer == "true")
		{
			this->_fullscreenWindow = true;
		}
		else if (entryBuffer == "false")
		{
			this->_fullscreenWindow = false;
		}
		else
		{
			cerr << "ERROR: 'fullscreened=" << entryBuffer << "' is whether true nor false!" << endl;
			return false;
		}
	}
	
	return true;
}

Node * Motherboard::getNode(int x, int y) const
{
	if ((x >= 0) && (x < motherboardNodesX) && (y >= 0) && (y < motherboardNodesY))
		return this->_nodes[x][y];
	else
		return nullptr;
}

void Motherboard::setNode(int x, int y, Node* node)
{
	if ((x >= 0) && (x < motherboardNodesX) && (y >= 0) && (y < motherboardNodesY))
	{
		this->_nodes[x][y] = node;
		for (int direction = 0; direction < 8; direction++)
		{
			int directionX = x;
			int directionY = y;
			switch ((Direction) direction)
			{
				case UP:
					directionY--;
					break;
				case UP_RIGHT:
					directionX++;
					directionY--;
					break;
				case RIGHT:
					directionX++;
					break;
				case DOWN_RIGHT:
					directionX++;
					directionY++;
					break;
				case DOWN:
					directionY++;
					break;
				case DOWN_LEFT:
					directionX--;
					directionY++;
					break;
				case LEFT:
					directionX--;
					break;
				case UP_LEFT:
					directionX--;
					directionY--;
					break;
				default:
					break;
			}
			if (directionX >= 0 && directionX < 4 && directionY >= 0 && directionY < 4)
			{
				Node* surroundingNode = this->_nodes[directionX][directionY];
				Direction oppositeDirection = (Direction) ((direction + 4) % 8);
				
				if (surroundingNode != nullptr)
				{
					node->setSurroundingNode((Direction) direction,surroundingNode);
					surroundingNode->setSurroundingNode(oppositeDirection, node);
				}
			}
		}
	}
}

bool Motherboard::isVerbose() const
{
	return this->_verbose;
}

void Motherboard::setVerbose(bool verbose)
{
	this->_verbose = verbose;
}

bool Motherboard::isSlow() const
{
	return this->_slow;
}

void Motherboard::setSlow(bool slow)
{
	this->_slow = slow;
}

float Motherboard::getTickSpeed() const
{
	return this->_tickSpeed;
}

void Motherboard::setTickSpeed(float tickSpeed)
{
	this->_tickSpeed = tickSpeed;
}

float Motherboard::getSlowTickSpeed() const
{
	return this->_slowTickSpeed;
}

void Motherboard::setSlowTickSpeed(float slowTickSpeed)
{
	this->_slowTickSpeed = slowTickSpeed;
}

GLFWwindow* Motherboard::getContext() const
{
	return this->_context;
}

void Motherboard::setContext(GLFWwindow* newContext)
{
	this->_context = newContext;
}

Motherboard::State Motherboard::getState() const
{
	return this->_state;
}

