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

bool CPU::execMoveAddressRegister8Bit()
{
	NODE_INT_TYPE address = _memory->get16BitValue(this->_programPointer);
	this->_programPointer += 2;

	NODE_INT_TYPE value = _memory->get8BitValue(address);
	
	return this->setTargetRegister(value);
}

bool CPU::execMoveAddressRegister16Bit()
{
	NODE_INT_TYPE address = _memory->get16BitValue(this->_programPointer);
	this->_programPointer += 2;

	NODE_INT_TYPE value = _memory->get16BitValue(address);
	
	return this->setTargetRegister(value);
}

bool CPU::execMoveRegAddressRegister8Bit()
{
	NODE_INT_TYPE address = 0;
	
	if (! this->getBaseRegisterValue(&address))
		return false;
	
	NODE_INT_TYPE value = _memory->get8BitValue(address);
	
	return this->setTargetRegister(value);
}

bool CPU::execMoveRegAddressRegister16Bit()
{
	NODE_INT_TYPE address = 0;
	
	if (! this->getBaseRegisterValue(&address))
		return false;
	
	NODE_INT_TYPE value = _memory->get16BitValue(address);
	
	return this->setTargetRegister(value);
}

bool CPU::execMoveChannelRegister()
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
		return this->setRegister(targetReg, value);
	}
	else
	{
		this->_operationSuccessfull = false;
	}
	return true;
}

bool CPU::execMoveValueRegister8Bit()
{
	uint8_t value = _memory->get8BitValue(this->_programPointer);
	this->_programPointer++;

	return this->setTargetRegister(value);
}

bool CPU::execMoveValueRegister16Bit()
{
	uint16_t value = _memory->get16BitValue(this->_programPointer);
	this->_programPointer += 2;

	return this->setTargetRegister(value);
}

bool CPU::execMoveRegisterRegister()
{
	NODE_INT_TYPE value = 0;
	
	this->getBaseRegisterValue(&value);

	return this->setTargetRegister(value);
}

bool CPU::execMoveRegisterAddress8Bit()
{
	NODE_INT_TYPE value = 0;
	
	if (! this->getBaseRegisterValue(&value))
		return false;
	
	value = value % 0x100; // cut off everything that is bigger than 255.
	
	NODE_INT_TYPE address = _memory->get16BitValue(this->_programPointer);
	this->_programPointer += 2;
	
	_memory->set8BitValue(address, (uint8_t) value);
	
	return true;
}

bool CPU::execMoveRegisterAddress16Bit()
{
	NODE_INT_TYPE value = 0;
	
	if (! this->getBaseRegisterValue(&value))
		return false;
	
	NODE_INT_TYPE address = _memory->get16BitValue(this->_programPointer);
	this->_programPointer += 2;
	
	_memory->set16BitValue(address, (uint16_t) value);
	
	return true;
}

bool CPU::execMoveRegisterRegAddress8Bit()
{
	NODE_INT_TYPE value = 0;
	
	if (! this->getBaseRegisterValue(&value))
		return false;
	
	NODE_INT_TYPE address = 0;
	
	this->getBaseRegisterValue(&address);
	
	_memory->set8BitValue(address, value);
	
	return true;
}

bool CPU::execMoveRegisterRegAddress16Bit()
{
	NODE_INT_TYPE value = 0;
	
	if (! this->getBaseRegisterValue(&value))
		return false;
	
	NODE_INT_TYPE address = 0;
	
	if (! this->getBaseRegisterValue(&address))
		return false;
	
	_memory->set16BitValue(address, value);
	
	return true;
}

bool CPU::execMoveRegisterChannel()
{
	NODE_INT_TYPE value = 0;
	
	if (! this->getBaseRegisterValue(&value))
		return false;
	
	NODE_INT_TYPE channel = 0;
	if (! this->loadChannel(&channel))
		return false;
	
	this->_operationSuccessfull = this->push((Direction) channel, value);
	
	return true;
}

