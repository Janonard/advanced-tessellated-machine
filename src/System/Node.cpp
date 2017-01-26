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
#include "System/Node.h"
#include <iostream>

Node::Node() :
	_surroundingNodes(),
	_surroundingValues(),
	_areDirty(),
	_isRunning(true),
	_hasPreticked(false),
	_context(nullptr)
{
	_surroundingNodes.fill(nullptr);
	_surroundingValues.fill(0);
	_areDirty.fill(false);
}

Node::~Node()
{
}

bool Node::configure(const Config& configuration, GLFWwindow* context)
{
	this->_context = context;
	return true;
}

bool Node::deconfigure()
{
	return true;
}

Node* Node::getSurroundingNode(const Direction& direction) const
{
	return this->_surroundingNodes[(int) direction];
}

void Node::setSurroundingNode(const Direction& direction, Node* newNode)
{
	this->_surroundingNodes[(int) direction] = newNode;
}

bool Node::hasPreticked() const
{
	return this->_hasPreticked;
}

bool Node::push(const Direction& direction, NODE_INT_TYPE value)
{
	Node* targetNode = this->getSurroundingNode(direction);
	Direction oppositeDirection = (Direction) (((int) direction + 4) % 8);
	if (targetNode != nullptr)
	{
		return targetNode->receivePush(oppositeDirection, value);
	}
	else
	{
		return false;
	}
}

bool Node::receivePush(const Direction& direction, NODE_INT_TYPE value)
{
	if (this->isDirty(direction))
	{
		return false;
	}
	else
	{
		this->_surroundingValues[(int) direction] = value;
		this->_areDirty[(int) direction] = true;
		return true;
	}
}

bool Node::pull(const Direction& direction, NODE_INT_TYPE* value)
{
	if (this->isDirty(direction))
	{
		*value = this->_surroundingValues[(int) direction];
		this->_areDirty[(int) direction] = false;
		return true;
	}
	else
	{
		return false;
	}
}


bool Node::isDirty(const Direction& direction) const
{
	return this->_areDirty[(int) direction];
}

bool Node::pretick()
{
	this->_hasPreticked = false;
	return true;
}

bool Node::tick()
{
	return true;
}

bool Node::isRunning() const
{
	return this->_isRunning;
}

std::string Node::getDebugString() const
{
	return "I'm a node!";
}

GLFWwindow* Node::getContext() const
{
	return this->_context;
}
