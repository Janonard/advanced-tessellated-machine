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
#include "System/ExtensionNode.h"
#include <chrono>

enum class TimeAdapterCommand
{
	UpdateTime=0,
	GetUnixTime=1,
	GetMicroTime=2
};

/*
 * This adapter manages time.
 */
class TimeAdapter : public ExtensionNode
{
public: // functions
	TimeAdapter();
	
	~TimeAdapter() override;
	
	bool tick() override;
	
	std::string getDebugString() const override;
	
private: // functions
	
	/*
	 * This takes the current time and stores it in _timepoint.
	 */
	bool execUpdateTime();
	
	/*
	 * Adds the stored _timepoint, converted to seconds, to
	 * the push queue. Since UNIX time is a 32-bit number,
	 * it is pushed in two parts you need to connect to each
	 * other.
	 */
	bool execGetUnixTime(Direction pulledDirection);
	
	/*
	 * Adds the stored _timepoint, converted to microseconds,
	 * to the push queue. Since this is a quite big number,
	 * it is pushed in two parts you need to connect to each
	 * other. The microtime is an addition of accuracy for
	 * the UNIX time, which means that it only contains
	 * the microseconds since the last whole second.
	 */
	bool execGetMicroTime(Direction pulledDirection);
	
private: // properties
	
	/*
	 * Our current timepoint.
	 */
	std::chrono::high_resolution_clock::time_point _timepoint;
};
