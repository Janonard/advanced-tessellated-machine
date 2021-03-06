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
#include "Assembler/Command.h"

using namespace std;
using namespace Assembler;

Assembler::Command::Command() :
	_type(CommandType::Halt),
	_code()
{
}

Assembler::CommandType Assembler::Command::getType() const
{
	return this->_type;
}

void Assembler::Command::setType(Assembler::CommandType type)
{
	this->_type = type;
}

const Memory& Assembler::Command::getCode() const
{
	return this->_code;
}

Memory * Assembler::Command::getCodePointer()
{
	return &this->_code;
}

void Assembler::Command::setCode(Memory code)
{
	this->_code = code;
}
