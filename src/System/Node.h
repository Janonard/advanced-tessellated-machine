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

#include <string>
#include <array>
#include <GLFW/glfw3.h>
#include "System/Config.h"
#include "System/Direction.h"

#define NODE_INT_TYPE uint16_t

/*
* This is the base class for all ATM nodes. It has the ability to push and pull data to and from
* all nodes which surrounds it, which are eight in count.
*/
class Node
{
public: // methods
	
	Node();
	virtual ~Node();
	
	/*
	 * This is something like the second construction stage.
	 * After construction, we get plugged into the motherboard
	 * and when all nodes are plugged in, the motherboard calls this
	 * method. Child classes can override this to completly setup
	 * themselves.
	 * 'configuration' contains all config entries in the system's config file.
	 * When configuration fails, this method will return false.
	 */
	virtual bool configure(const Config& configuration, GLFWwindow* context);
	
	/*
	 * When the main loop has ended, the motherboard calls this method.
	 * Child classes can override this to deconfigure themselves.
	 * When deconfiguration fails, this method will return false.
	 */
	virtual bool deconfigure();
	
	/*
	* Returns the a pointer to the node at the given position relative
	* to us.
	*/
	Node* getSurroundingNode(const Direction& direction) const;
	
	/*
	 * Sets the pointer to the node at the given position relative to us.
	 */
	void setSurroundingNode(const Direction& direction, Node* newNode);
	
	/*
	* Pushes a value to a given side.
	* Returns true when the action was successfull, false when not.
	* This happens if the node at the end of the connection
	* hasn't pulled the last push. In this case, no action will be done.
	*/
	bool push(const Direction& direction, NODE_INT_TYPE value);
	
	/*
	 * Tries to set the value of '_surroundingValues' for the
	 * given direction.
	 * If the last value is not pulled yet, this method returns false and no action is done.
	 */
	bool receivePush(const Direction& direction, NODE_INT_TYPE value);
	
	/*
	* Pulls a value from a given side.
	* Returns true when the action was successfull, false when not.
	* This happens when a value was already pulled. In this case,
	* the function won't override the integer.
	*/
	bool pull(const Direction& direction, NODE_INT_TYPE* value);
	
	/*
	 * Returns whether a connection of us is dirty.
	 * This means that we have not pulled from this direction.
	 */
	bool isDirty(const Direction& direction) const;
	
	/*
	* Before the motherboard calls 'tick()', it calls this method.
	* All actions that push values should be done here, since it
	* makes it possible to push and pull a value in one tick cycle.
	*/
	virtual bool pretick();
	
	/*
	 * All runtime actions are done within this method,
	 * which get called by the motherboard every tick cycle.
	 */
	virtual bool tick();
	
	/*
	* Returns a string that holds all relevant information about the node.
	*/
	virtual std::string getDebugString() const;
	
	/*
	 * Returns whether we have preticked or not.
	 */
	bool hasPreticked() const;
	
	/*
	 * Returns whether we are currently running or not.
	 * When all nodes on the motherboard stopped running,
	 * it ends running too.
	 */
	bool isRunning() const;
	
	/*
	 * Returns a reference to the currently open context.
	 */
	GLFWwindow* getContext() const;
	
private: // , non-accessible methods
	
	Node(const Node& node);
	Node& operator=(const Node& node);
	
protected:// properties
	
	/*
	 * An array with pointers to the nodes that are around us.
	 */
	std::array<Node*,8> _surroundingNodes;
	
	/*
	 * When a node pushes something to us, it is saved here.
	 */
	std::array<NODE_INT_TYPE,8> _surroundingValues;
	
	/*
	 * When a node pushes something to us, it sets the dirty flag
	 * to true. When we pull this value, the flag is set to false again.
	 */
	std::array<bool,8> _areDirty;
	
	/*
	 * While we are running and everything is fine, this flag is positive.
	 * When we stopped running, it is false.
	 */
	bool _isRunning;
	
	/*
	 * When we did something in our pretick, this is positive.
	 * If not, not.
	 */
	bool _hasPreticked;
	
	/*
	 * A reference to the currently open glfw context.
	 */
	GLFWwindow* _context;
	
};
