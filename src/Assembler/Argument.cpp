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
#include "Assembler/Argument.h"

using namespace Assembler;

Assembler::Argument::Argument() :
	_type(ArgumentType::NoArgument),
	_code(),
	_symbolName()
{
}

Assembler::ArgumentType Assembler::Argument::getType() const
{
	return this->_type;
}

void Assembler::Argument::setType(Assembler::ArgumentType type)
{
	this->_type = type;
}

std::shared_ptr<Memory> Assembler::Argument::getCode() const
{
	return this->_code;
}

void Assembler::Argument::setCode(std::shared_ptr<Memory> code)
{
	this->_code = code;
}

const std::string & Assembler::Argument::getSymbolName() const
{
	return this->_symbolName;
}

void Assembler::Argument::setSymbolName(const std::string& symbolName)
{
	this->_symbolName = symbolName;
}
