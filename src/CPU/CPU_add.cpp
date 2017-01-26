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

bool CPU::execAddAddressRegister8bit()
{
	NODE_INT_TYPE address = _memory->get16BitValue(this->_programPointer);
	this->_programPointer += 2;
	
	uint8_t value = this->_memory->get8BitValue(address);
	
	return this->addTargetRegister(value);
}

bool CPU::execAddAddressRegister16bit()
{
	NODE_INT_TYPE address = _memory->get16BitValue(this->_programPointer);
	this->_programPointer += 2;
	
	NODE_INT_TYPE value = _memory->get16BitValue(address);
	
	return this->addTargetRegister(value);
}

bool CPU::execAddRegAddressRegister8bit()
{
	NODE_INT_TYPE address = 0;
	
	if (! this->getBaseRegisterValue(&address))
		return false;
	
	NODE_INT_TYPE value = _memory->get8BitValue(address);
	
	return this->addTargetRegister(value);
}

bool CPU::execAddRegAddressRegister16bit()
{
	NODE_INT_TYPE address = 0;
	
	if (! this->getBaseRegisterValue(&address))
		return false;
	
	NODE_INT_TYPE value = _memory->get16BitValue(address);
	
	return this->addTargetRegister(value);
}

bool CPU::execAddChannelRegister()
{
	NODE_INT_TYPE channel = 0;
	if (! this->loadChannel(&channel))
		return false;
	
	NODE_INT_TYPE targetReg = 0;
	if (! this->loadRegister(&targetReg))
		return false;
	
	NODE_INT_TYPE value = 0;
	if (this->pull((Direction) channel, &value))
	{
		this->_operationSuccessfull = true;
		return this->addToRegister(targetReg, value);
	}
	else
	{
		this->_operationSuccessfull = false;
	}
	return true;
}

bool CPU::execAddValueRegister8bit()
{
	uint8_t value = _memory->get8BitValue(this->_programPointer);
	this->_programPointer++;
	
	return this->addTargetRegister(value);
}

bool CPU::execAddValueRegister16bit()
{
	uint16_t value = _memory->get16BitValue(this->_programPointer);
	this->_programPointer += 2;
	
	return this->addTargetRegister(value);
}

bool CPU::execAddRegisterRegister()
{
	NODE_INT_TYPE value = 0;
	
	this->getBaseRegisterValue(&value);
	
	return this->addTargetRegister(value);
}
