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
#include "TimeAdapter/TimeAdapter.h"

using namespace std;
using namespace chrono;

TimeAdapter::TimeAdapter() :
	_timepoint(high_resolution_clock::time_point())
{
}

TimeAdapter::~TimeAdapter()
{
}

bool TimeAdapter::tick()
{
	NODE_INT_TYPE rawData;
	bool dataPulled = false;
	Direction pulledDirection = Direction::UP;
	for (Direction direction : this->_watchDirections)
	{
		if (this->pull(direction, &rawData))
		{
			dataPulled = true;
			pulledDirection = direction;
			break;
		}
	}
	
	if (dataPulled)
	{
		if (rawData == (int) TimeAdapterCommand::UpdateTime)
			return this->execUpdateTime();
		else if (rawData == (int) TimeAdapterCommand::GetUnixTime)
			return this->execGetUnixTime(pulledDirection);
		else if (rawData == (int) TimeAdapterCommand::GetMicroTime)
			return this->execGetMicroTime(pulledDirection);
	}
	return true;
}

string TimeAdapter::getDebugString() const
{
	return "time adapter";
}


bool TimeAdapter::execUpdateTime()
{
	this->_timepoint = high_resolution_clock::now();
	return true;
}

bool TimeAdapter::execGetUnixTime(Direction pulledDirection)
{
	uint32_t time = duration_cast<seconds>(this->_timepoint.time_since_epoch()).count();
	NODE_INT_TYPE timeUpperHalf = (NODE_INT_TYPE) (time / 0x10000);
	this->addPushQueueElement(pulledDirection, timeUpperHalf);
	this->addPushQueueElement(pulledDirection, (NODE_INT_TYPE) time % 0x10000);
	return true;
}

bool TimeAdapter::execGetMicroTime(Direction pulledDirection)
{
	uint32_t time = duration_cast<microseconds>(this->_timepoint.time_since_epoch()).count();
	time %= 1000000;
	NODE_INT_TYPE timeUpperHalf = (NODE_INT_TYPE) (time / 0x10000);
	this->addPushQueueElement(pulledDirection, timeUpperHalf);
	this->addPushQueueElement(pulledDirection, (NODE_INT_TYPE) time % 0x10000);
	return true;
}
