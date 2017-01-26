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
#include "Executable/ExecutableElement.h"
#include <iostream>

ExecutableElement::ExecutableElement() :
	_x(0),
	_y(0),
	_memory()
{
	
}

void ExecutableElement::setMemory(std::string memoryFileText)
{
	this->_memory.clear();
	for (char character : memoryFileText)
	{
		this->_memory.push_back(uint8_t(character));
	}
}

uint8_t ExecutableElement::get8BitValue(const NODE_INT_TYPE position) const
{
	if (position > this->_memory.size())
	{
		return 0;
	}
	else
	{
		return this->_memory[position];
	}
}

void ExecutableElement::set8BitValue(const NODE_INT_TYPE position, uint8_t value)
{
	if (position < this->_memory.size())
	{
		this->_memory[position] = value;
	}
}

uint16_t ExecutableElement::get16BitValue(const NODE_INT_TYPE position) const
{
	uint16_t value = 0;
	if (position < this->_memory.size())
	{
		value = ((uint16_t) this->_memory[position]) * 0x100;
	}
	if (position+1 < this->_memory.size())
	{
		value += (uint16_t) this->_memory[position+1];
	}
	return value;
}

void ExecutableElement::set16BitValue(const NODE_INT_TYPE position, uint16_t value)
{
	if (position < this->_memory.size())
	{
		this->_memory[position] = (uint8_t) ((value - (value % 0x100)) / 0x100);
		if (position+1 < this->_memory.size())
		{
			this->_memory[position+1] = (uint8_t) (value % 0x100);
		}
	}
}
