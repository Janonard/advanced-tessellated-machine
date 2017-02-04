/*
 * advanced tesselated machine
 * Copyright (C) 2015-2016 Janonard
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
#include "Assembler/Line.h"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace Assembler;

bool Assembler::Line::identifyArgument(uint argumentNumber)
{
	
	Argument* argPtr;
	if (argumentNumber == 0)
	{
		this->_argument0 = Argument();
		argPtr = &(this->_argument0);
	}
	else
	{
		this->_argument1 = Argument();
		argPtr = &(this->_argument1);
	}
	
	shared_ptr<Memory> memory(nullptr);
	try
	{
		memory = make_shared<Memory>();
	}
	catch (exception e)
	{
		this->printErrorHeader();
		cerr << "Could not allocate memory!" << endl;
		return false;
	}
	argPtr->setCode(memory);
	
	if (this->_splitLine.size() <= this->_wordOffset + argumentNumber +1)
		return true;
	
	string word = this->_splitLine[this->_wordOffset + argumentNumber + 1];
	
	
	
	if (word[0] == this->cNumberIdentifier)
	{
		if (word[-1] == this->cNumberIdentifier)
		{
			argPtr->setType(ArgumentType::SymbolNumber);
			word = word.substr(1);
			word.pop_back();
			argPtr->setSymbolName(word);
			this->_memorySize += 2;
			return true;
		}
		else if (word.size() == this->cEightBitNumberLength)
		{
			argPtr->setType(ArgumentType::Number8Bit);
			if (!this->codeToInt(word.substr(1,2),memory))
				return false;
			this->_memorySize += 1;
			return true;
			
		}
		else if (word.size() == this->cSixteenBitNumberLength)
		{
			argPtr->setType(ArgumentType::Number16Bit);
			if (!this->codeToInt(word.substr(1,2),memory))
				return false;
			if (!this->codeToInt(word.substr(3,2),memory))
				return false;
			this->_memorySize += 2;
			return true;
		}
		else
		{
			this->printErrorHeader();
			cerr << "A number must have two or four digits or must be a symbol!" << endl;
			return false;
		}
	}
	else if (word[0] == this->cAddressIdentifier)
	{
		if (auto iter = std::find(this->vAddressRegisters.begin(), this->vAddressRegisters.end(), word) != this->vAddressRegisters.end())
		{
			argPtr->setType(ArgumentType::AddressRegister);
			memory->push_back(iter);
			this->_memorySize += 1;
			return true;
		}
		else if (word.size() == this->cSixteenBitNumberLength)
		{
			argPtr->setType(ArgumentType::Address);
			if (!this->codeToInt(word.substr(1,2),memory))
				return false;
			if (!this->codeToInt(word.substr(3,2),memory))
				return false;
			this->_memorySize += 2;
			return true;
		}
		else
		{
			this->printErrorHeader();
			cerr << "An address must have four digits or must be an address register!" << endl;
			return false;
		}
	}
	else if (word[0] == this->cOpenBracket and word[-1] == this->cClosedBracket)
	{
		argPtr->setType(ArgumentType::Filename);
		word.pop_back();
		word = word.substr(1);
		argPtr->setSymbolName(word);
		return true;
	}
	else if (auto iter = std::find(this->vRegisters.begin(), this->vRegisters.end(), word) != this->vRegisters.end())
	{
		argPtr->setType(ArgumentType::Register);
		memory->push_back(iter);
		this->_memorySize += 1;
		return true;
	}
	else if (auto iter = std::find(this->vChannels.begin(), this->vChannels.end(), word) != this->vChannels.end())
	{
		argPtr->setType(ArgumentType::Channel);
		memory->push_back(iter);
		this->_memorySize += 1;
		return true;
	}
	else
	{
		argPtr->setType(ArgumentType::Symbol);
		argPtr->setSymbolName(word);
		this->_memorySize += 2;
		return true;
	}
	
	return true;
}
