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

#include <array>
#include <queue>
#include "System/Direction.h"
#include "System/Node.h"

/*
 * A struct-like class to be used as an Element in the pushQueue
 * of an extension Node.
 */
class PushQueueElement
{
public:
	PushQueueElement();
	PushQueueElement(Direction newPushDirection, NODE_INT_TYPE newValue);
	
	/*
	 * The direction to push to.
	 */
	Direction pushDirection;
	/*
	 * The value to be pushed.
	 */
	NODE_INT_TYPE value;
};

/*
 * This base class for extension nodes offers two functionalities:
 * Watch directions and the push queue.
 */
class ExtensionNode : public Node
{
protected:
	
	/*
	 * The directions we should listen to.
	 */
	std::array<Direction, 2> _watchDirections;
	
	/*
	 * A queue containing all elements that need to be pushed.
	 */
	std::queue<PushQueueElement> _pushQueue;
	
public:
    ExtensionNode();
	
	virtual ~ExtensionNode() override;
	
	/*
	 * In this override of the pretick function the first element
	 * of the push queue is pushed. If successfull, this element
	 * will be removed from the push queue.
	 */
	virtual bool pretick() override;
	
	/*
	 * Creates a PushQueueElement object with the given parameters
	 * and adds it to the push queue.
	 */
	void addPushQueueElement(const Direction& pushDirection, NODE_INT_TYPE value);
	
	std::array<Direction, 2> getWatchDirections() const;
	void setWatchDirections(const std::array<Direction, 2>& newDirections);
};
