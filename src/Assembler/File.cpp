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
#include "Assembler/File.h"
#include <iostream>

using namespace std;

Assembler::File::File() :
	_text(),
	_lines(),
	_elements(),
	_symbols(),
	_baseFilePath(),
	_memorySize(0)
{
	try
	{
		this->_symbols = make_shared<SymbolMap>();
	}
	catch (bad_alloc e)
	{
		this->printErrorHeader();
		throw(e);
	}
}

Assembler::File::~File()
{
	this->_symbols.reset();
}

bool Assembler::File::assembleText()
{
	string currentLine = string();
	uint currentLineNumber = 1;
	
	for (char c : this->_text)
	{
		if (c == '\n')
		{
			if (! this->assembleLine(currentLine, currentLineNumber))
				return false;
			currentLine = string();
			currentLineNumber++;
		}
		else
		{
			currentLine.push_back(c);
		}
	}
	
	if (currentLine.size() > 0)
	{
		return this->assembleLine(currentLine, currentLineNumber);
	}
	else
	{
		return true;
	}
}

bool Assembler::File::assembleLine(string line, uint lineNumber)
{
	
	shared_ptr<Assembler::Line> newLine(nullptr);
	try
	{
		newLine = make_shared<Assembler::Line>();
	}
	catch (std::bad_alloc e)
	{
		this->printErrorHeader();
		cerr << "Could not allocate new memory!" << endl;
		throw(e);
	}
	
	newLine->setRawLine(line);
	newLine->setLineNumber(lineNumber);
	newLine->setFilePath(this->getBaseFilePath());
	newLine->setSymbols(this->_symbols);
	newLine->setMemoryLocation(this->_memorySize);
	
	this->_lines.push_back(newLine);
	
	if (! newLine->assembleLine())
		return false;
	
	for (shared_ptr<Assembler::Line> additionalLine : newLine->getAdditionaLines())
	{
		this->_lines.push_back(additionalLine);
	}
	
	NODE_INT_TYPE newSize = this->_memorySize + newLine->getMemorySize();
	if (newSize < this->_memorySize) // We hit the 64k limit
	{
		this->printErrorHeader();
		cerr << "The binary is bigger than 64kB!" << endl;
		return false;
	}
	this->_memorySize = newSize;
	
	return true;
}

void Assembler::File::printErrorHeader()
{
	if (this->_lines.size() > 0 && this->_lines.back().get() != nullptr)
	{
		this->_lines.back()->printErrorHeader();
	}
	else
	{
		cerr << "ERROR while assembling!" << endl;
	}
}

const std::string & Assembler::File::getText() const
{
	return this->_text;
}

void Assembler::File::setText(const std::string& text)
{
	this->_text = text;
}

vector<shared_ptr<ExecutableElement>> Assembler::File::getElements() const
{
	return this->_elements;
}

std::shared_ptr<Assembler::SymbolMap> Assembler::File::getSymbols() const
{
	return this->_symbols;
}

void Assembler::File::resetElements()
{
	this->_elements = vector<shared_ptr<ExecutableElement>>();
}

const std::string & Assembler::File::getBaseFilePath() const
{
	return this->_baseFilePath;
}

void Assembler::File::setBaseFilePath(std::string baseFilePath)
{
	this->_baseFilePath = baseFilePath;
}
