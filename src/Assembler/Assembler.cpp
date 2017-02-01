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
#include "Assembler/Assembler.h"
#include <iostream>

using namespace std;

Assembler::Assembler() :
	_text(),
	_lines(),
	_elements(),
	_symbols(),
	_baseFilePath()
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

Assembler::~Assembler()
{
	this->_symbols.reset();
}

bool Assembler::assembleText()
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

bool Assembler::assembleLine(string line, uint lineNumber)
{
	shared_ptr<AssemblerLine> newLine(nullptr);
	
	try
	{
		newLine = make_shared<AssemblerLine>();
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
	
	this->_lines.push_back(newLine);
	
	if (! newLine->assembleLine())
		return false;
	
	for (shared_ptr<AssemblerLine> additionalLine : newLine->getAdditionaLines())
	{
		this->_lines.push_back(additionalLine);
	}
	
	return true;
}

void Assembler::printErrorHeader()
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

const std::string & Assembler::getText() const
{
	return this->_text;
}

void Assembler::setText(const std::string& text)
{
	this->_text = text;
}

vector<shared_ptr<ExecutableElement>> Assembler::getElements() const
{
	return this->_elements;
}

std::shared_ptr<SymbolMap> Assembler::getSymbols() const
{
	return this->_symbols;
}

void Assembler::resetElements()
{
	this->_elements = vector<shared_ptr<ExecutableElement>>();
}

const std::string & Assembler::getBaseFilePath() const
{
	return this->_baseFilePath;
}

void Assembler::setBaseFilePath(std::string baseFilePath)
{
	this->_baseFilePath = baseFilePath;
}
