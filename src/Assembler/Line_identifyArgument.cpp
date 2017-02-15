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
#include "Assembler/Line.h"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace Assembler;

inline bool codeToInt(Line* line, const std::string& word, Memory& memory)
{
	try
	{
		uint8_t code = stoi(word,nullptr,16);
		memory.push_back(code);
	}
	catch (exception e)
	{
		line->printErrorHeader();
		cerr << "Could not parse value!" << endl;
		return false;
	}
	return true;
}

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
	
	Memory memory = Memory();
	
	if (this->_splitLine.size() <= this->_wordOffset + argumentNumber +1)
		return true;
	
	string word = this->_splitLine[this->_wordOffset + argumentNumber + 1];
	
	if (word[0] == this->cNumberIdentifier)
	{
		if (word.back() == this->cNumberIdentifier)
		{
			argPtr->setType(ArgumentType::SymbolNumber);
			word = word.substr(1);
			word.pop_back();
			argPtr->setSymbolName(word);
			this->_memorySize += 2;
		}
		else if (word.size() == this->cEightBitNumberLength)
		{
			argPtr->setType(ArgumentType::Number8Bit);
			if (!codeToInt(this,word.substr(1,2),memory))
				return false;
			this->_memorySize += 1;
			
		}
		else if (word.size() == this->cSixteenBitNumberLength)
		{
			argPtr->setType(ArgumentType::Number16Bit);
			if (!codeToInt(this,word.substr(1,2),memory))
				return false;
			if (!codeToInt(this,word.substr(3,2),memory))
				return false;
			this->_memorySize += 2;
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
		auto iter = std::find(this->vAddressRegisters.begin(), this->vAddressRegisters.end(), word);
		if (iter != this->vAddressRegisters.end())
		{
			argPtr->setType(ArgumentType::AddressRegister);
			// `null` isn't a valid address register, but the register numbers stay the same.
			// Therefore, we need to add 1 to the result.
			memory.push_back(iter - this->vAddressRegisters.begin() + 1);
			this->_memorySize += 1;
		}
		else if (word.size() == this->cSixteenBitNumberLength)
		{
			argPtr->setType(ArgumentType::Address);
			if (!codeToInt(this,word.substr(1,2),memory))
				return false;
			if (!codeToInt(this,word.substr(3,2),memory))
				return false;
			this->_memorySize += 2;
		}
		else
		{
			this->printErrorHeader();
			cerr << "An address must have four digits or must be an address register!" << endl;
			return false;
		}
	}
	else if (word[0] == this->cOpenBracket and word.back() == this->cClosedBracket)
	{
		argPtr->setType(ArgumentType::Filename);
		word.pop_back();
		word = word.substr(1);
		argPtr->setSymbolName(word);
	}
	else 
	{
		auto registerIter = std::find(this->vRegisters.begin(), this->vRegisters.end(), word);
		auto channelIter = std::find(this->vChannels.begin(), this->vChannels.end(), word);
		
		if (registerIter != this->vRegisters.end())
		{
			argPtr->setType(ArgumentType::Register);
			memory.push_back(registerIter - this->vRegisters.begin());
			this->_memorySize += 1;
		}
		else if (channelIter != this->vChannels.end())
		{
			argPtr->setType(ArgumentType::Channel);
			memory.push_back(channelIter - this->vChannels.begin());
			this->_memorySize += 1;
		}
		else
		{
			argPtr->setType(ArgumentType::Symbol);
			argPtr->setSymbolName(word);
			this->_memorySize += 2;
		}
	}
	
	argPtr->setCode(memory);
	return true;
}
