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
#pragma once

#include "System/Node.h"
#include "System/ContextHandler.h"
#include <memory>

const int motherboardNodesX = 4;
const int motherboardNodesY = 4;
const int motherboardScreenSizeX = 570;
const int motherboardScreenSizeY = 456;
const std::string motherboardConfigFile = ".atm";

/*
 * The base class of the ATM system, which resembles the motherboard.
 * All nodes are plugged in it and it provides the connection between them.
 * Also, it configures all nodes and runs the main loop.
 */
class Motherboard
{
	/*
	 * GLFW calls this method when an Error occurs.
	 */
	static void glfwErrorCallback(int error, const char* description);
	
public: // methods
	
	enum State
	{
		RUNNING, HALTED, CRASHED
	};
	
	Motherboard();
	
	/*
	* Loads the configuration file and applies it's settings to us (by calling 'configureYourself')
	* and all of our nodes (by calling 'Node::configure').
	*/
	bool configure();
	
	/*
	 * Prepares our nodes for their destruction by calling 'Node::deconfigure'.
	 */
	bool deconfigure();
	
	/*
	* Executes a tick for each node in the system.
	* Returns the state of the system after the tick.
	*/
	State tick();
	
	/*
	 * Executes ticks until the system halts or crashes
	 */
	void loop();
	
private: // methods
	/*
	* This configuration function looks up all settings for the core system.
	*/
	bool configureYourself(const Config& configuration);
	
private: // , non-accessible methods
	
	Motherboard(const Motherboard& motherboard);
	Motherboard& operator=(const Motherboard& motherboard);
	
public: // getters and setters
	
	Node* getNode(int x, int y) const;
	void setNode(int x, int y, Node* node);
	
	bool isVerbose() const;
	void setVerbose(bool);
	
	bool isSlow() const;
	void setSlow(bool);
	
	float getTickSpeed() const;
	void setTickSpeed(float);
	
	float getSlowTickSpeed() const;
	void setSlowTickSpeed(float);
	
	GLFWwindow* getContext() const;
	
	State getState() const;
	
public: // members
	
	/*
	* If verbose is enabled, the ATM will write a log with the debug strings of all nodes.
	*/
	bool _verbose;
	/*
	 * If true, we will use the _slowTickSpeed instead of _tickSpeed
	 */
	bool _slow;
	
private: // members
	/*
	* The nodes on our board.
	*/
	std::array<std::array<Node*,motherboardNodesX>,motherboardNodesY> _nodes;
	/*
	* This contains our speed in ticks per second.
	*/
	float _tickSpeed;
	/*
	 * Contatins our speed in ticks per seconds for when we are slow.
	 */
	float _slowTickSpeed;
	/*
	 * A reference to the currently open glfw context.
	 */
	std::shared_ptr<ContextHandler> _contextHandler;
	/*
	 * The screen size mulitplier
	 */
	float _screenSizeMulitplier;
	/*
	 * If true, the screen will be created using the full screen
	 */
	bool _fullscreenWindow;
	
	/*
	 * The state of the system.
	 */
	State _state;
};
