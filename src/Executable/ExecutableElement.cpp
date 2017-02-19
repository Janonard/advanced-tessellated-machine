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
#include <algorithm>

using namespace std;

ExecutableElement::ExecutableElement() :
	_x(0),
	_y(0),
	_memory(),
	_lines(),
	_symbols(),
	_filePathStack(),
	_includedFiles(),
	_newLineLocation(0)
{
	try
	{
		this->_symbols = make_shared<Assembler::SymbolMap>();
	}
	catch (bad_alloc e)
	{
		cerr << "Could not allocate new memory!" << endl;
	}
}

bool ExecutableElement::linkLinesToMemory()
{
	for (shared_ptr<Assembler::Line> line : this->_lines)
	{
		if (not line->linkLine(this->_memory))
			return false;
	}
	return true;
}

const std::string & ExecutableElement::getCurrentFilePath() const
{
	return this->_filePathStack.top();
}

bool ExecutableElement::isFileIncluded(std::string filePath) const
{
	return find(this->_includedFiles.begin(), this->_includedFiles.end(), filePath) != this->_includedFiles.end();
}

void ExecutableElement::setBaseFilePath(std::string baseFilePath)
{
	this->_filePathStack = stack<string>();
	this->_filePathStack.push(baseFilePath);
	this->_includedFiles = vector<string>();
	this->_includedFiles.push_back(baseFilePath);
}

bool ExecutableElement::addIncludedFile(std::string filePath)
{
	// That means that the given file is not in _includedFiles and therefore isn't already included.
	if (find(this->_includedFiles.begin(), this->_includedFiles.end(), filePath) == this->_includedFiles.end())
	{
		this->_filePathStack.push(filePath);
		this->_includedFiles.push_back(filePath);
		return true;
	}
	else
	{
		return false;
	}
}

bool ExecutableElement::closeIncludedFile()
{
	if (this->_filePathStack.size() > 0)
	{
		this->_filePathStack.pop();
		return true;
	}
	else
	{
		return false;
	}
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
