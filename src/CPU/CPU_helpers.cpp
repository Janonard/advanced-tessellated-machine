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
#include <sstream>

std::string CPU::connectStringWithInt(std::string param0, int16_t param1) const
{
	std::ostringstream strout;
	strout << param1;
	param0 += strout.str();
	return param0;
}

std::string CPU::connectStringWithUInt(std::string param0, uint16_t param1) const
{
	std::ostringstream strout;
	strout << param1;
	param0 += strout.str();
	return param0;
}

bool CPU::setRegister(const NODE_INT_TYPE registerNumber, const NODE_INT_TYPE value)
{
	switch (registerNumber)
	{
		case 0:
			return true;
		case 1:
			this->_acc = value;
			return true;
		case 2:
			this->_bcc = value;
			return true;
		case 3:
			this->_ccc = value;
			return true;
		default:
			std::cerr << "ERROR: Illegal channel number!" << std::endl;
			return false;
	}
}

bool CPU::setVarToRegister(NODE_INT_TYPE* var, const NODE_INT_TYPE registerNumber)
{
	switch (registerNumber)
	{
		case 0:
			*var = 0;
			return true;
		case 1:
			*var = this->_acc;
			return true;
		case 2:
			*var = this->_bcc;
			return true;
		case 3:
			*var = this->_ccc;
			return true;
		default:
			std::cerr << "ERROR: Illegal channel number!" << std::endl;
			return false;
	}
}

bool CPU::addToRegister(const NODE_INT_TYPE registerNumber, const NODE_INT_TYPE value)
{
	switch (registerNumber)
	{
		case 0:
			return true;
		case 1:
			this->_acc += value;
			return true;
		case 2:
			this->_bcc += value;
			return true;
		case 3:
			this->_ccc += value;
			return true;
		default:
			std::cerr << "ERROR: Illegal register number!" << std::endl;
			return false;
	}
}

bool CPU::loadRegister(NODE_INT_TYPE* reg)
{
	*reg = _memory->get8BitValue(this->_programPointer);
	this->_programPointer++;
	if (*reg > 3)
	{
		std::cerr << "ERROR: Illegal register number!" << std::endl;
		return false;
	}
	else
		return true;
}

bool CPU::loadChannel(NODE_INT_TYPE* channel)
{
	*channel = _memory->get8BitValue(this->_programPointer);
	this->_programPointer++;
	if (*channel > 7)
	{
		std::cerr << "ERROR: Illegal channel number!" << std::endl;
		return false;
	}
	else
		return true;
}

bool CPU::setTargetRegister(NODE_INT_TYPE value)
{
	NODE_INT_TYPE reg = 0;
	if (! this->loadRegister(&reg))
		return false;
	
	return this->setRegister(reg, value);
}

bool CPU::addTargetRegister(NODE_INT_TYPE value)
{
	NODE_INT_TYPE reg = 0;
	if (! this->loadRegister(&reg))
		return false;
	
	return this->addToRegister(reg, value);
}

bool CPU::getBaseRegisterValue(NODE_INT_TYPE* value)
{
	NODE_INT_TYPE baseReg = 0;
	if (! this->loadRegister(&baseReg))
		return false;
	
	return this->setVarToRegister(value, baseReg);
}
