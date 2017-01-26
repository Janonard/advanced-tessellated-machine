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
#include "System/ExtensionNode.h"

PushQueueElement::PushQueueElement() :
	pushDirection(Direction::UP),
	value(0)
{
}

PushQueueElement::PushQueueElement(Direction newPushDirection, NODE_INT_TYPE newValue) :
	pushDirection(newPushDirection),
	value(newValue)
{
}

ExtensionNode::ExtensionNode() :
	_watchDirections(std::array<Direction, 2>()),
	_pushQueue(std::queue<PushQueueElement>())
{
}

ExtensionNode::~ExtensionNode()
{
	Node::~Node();
}

bool ExtensionNode::pretick()
{
	if (this->_pushQueue.size() > 0)
	{
		PushQueueElement element = this->_pushQueue.front();
		if (this->push(element.pushDirection, element.value))
		{
			this->_pushQueue.pop();
		}
	}
	return true;
}

void ExtensionNode::addPushQueueElement(const Direction& pushDirection, uint16_t value)
{
	this->_pushQueue.push(PushQueueElement(pushDirection,value));
}

std::array< Direction, 2 > ExtensionNode::getWatchDirections() const
{
	return this->_watchDirections;
}

void ExtensionNode::setWatchDirections(const std::array< Direction, int(2) >& newDirections)
{
	this->_watchDirections = newDirections;
}
