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
#include "CPU/CPU.h"
#include <iostream>

bool CPU::execAbsoluteJump()
{
	NODE_INT_TYPE address = this->_memory->get16BitValue(this->_programPointer);
	this->_programPointer = address;
	return true;
}

bool CPU::execAbsoluteJumpOperationSuccessfull()
{
	if (this->_operationSuccessfull)
	{
		NODE_INT_TYPE address = this->_memory->get16BitValue(this->_programPointer);
		this->_programPointer = address;
	}
	else
	{
		this->_programPointer += 2;
	}
	return true;
}

bool CPU::execAbsoluteJumpOperationFailed()
{
	if (! this->_operationSuccessfull)
	{
		NODE_INT_TYPE address = this->_memory->get16BitValue(this->_programPointer);
		this->_programPointer = address;
	}
	else
	{
		this->_programPointer += 2;
	}
	return true;
}

bool CPU::execAbsoluteJumpGreater()
{
	if (this->_greater)
	{
		NODE_INT_TYPE address = this->_memory->get16BitValue(this->_programPointer);
		this->_programPointer = address;
	}
	else
	{
		this->_programPointer += 2;
	}
	return true;
}

bool CPU::execAbsoluteJumpLower()
{
	if (this->_lower)
	{
		NODE_INT_TYPE address = this->_memory->get16BitValue(this->_programPointer);
		this->_programPointer = address;
	}
	else
	{
		this->_programPointer += 2;
	}
	return true;
}

bool CPU::execAbsoluteJumpEqual()
{
	if ((! this->_greater) && (! this->_lower))
	{
		this->_programPointer = this->_memory->get16BitValue(this->_programPointer);
	}
	else
	{
		this->_programPointer += 2;
	}
	return true;
}

bool CPU::execAbsoluteJumpNonEqual()
{
	if ((this->_greater) | (this->_lower))
	{
		this->_programPointer = this->_memory->get16BitValue(this->_programPointer);
	}
	else
	{
		this->_programPointer += 2;
	}
	return true;
}

bool CPU::execBranch()
{
	if (this->_branchStack.size() == this->_branchStack.max_size())
	{
		std::cerr << "ERROR: Maximal size of branch stack reached!" << std::endl;
		return false;
	}
	this->_branchStack.push_back(this->_programPointer + 2);
	this->_programPointer = this->_memory->get16BitValue(this->_programPointer);
	return true;
}

bool CPU::execReturn()
{
	if (this->_branchStack.size() == 0)
	{
		std::cerr << "ERROR: Trying to access branch stack, but it's empty!" << std::endl;
		return false;
	}
	this->_programPointer = this->_branchStack.back();
	this->_branchStack.pop_back();
	return true;
}
