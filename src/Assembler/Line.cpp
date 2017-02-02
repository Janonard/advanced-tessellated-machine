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

using namespace std;

Assembler::Line::Line() :
	_rawLine(),
	_splitLine(),
	_lineNumber(0),
	_filePath(),
	_symbols(),
	_additionalLines()
{
	
}

bool Assembler::Line::assembleLine()
{
	if (!this->splitLineInWords())
		return false;
	
	for (string word : this->_splitLine)
	{
		cout << word << "|";
	}
	cout << endl;
	
	return true;
}

bool Assembler::Line::linkLine(Memory* outMemory)
{
	return true;
}

void Assembler::Line::printErrorHeader()
{
	cerr << "ERROR while assembling file '" << this->getFilePath() << "'!" << endl;
	cerr << "In line " << this->getLineNumber() << ":" << endl;
	cerr << this->getRawLine() << endl;
}

bool Assembler::Line::splitLineInWords()
{
	this->_splitLine = vector<string>();
	this->_splitLine.push_back("");
	bool bracketing = false;
	
	for (char c : this->_rawLine)
	{
		if (c == ';')
		{
			break;
		}
		
		if ((not bracketing) and (c == '<'))
		{
			bracketing = true;
		}
		else if (bracketing and c == '>')
		{
			bracketing = false;
		}
		
		if ((not bracketing) and (c == ' ' or c == '\t'))
		{
			this->_splitLine.push_back(string());
		}
		else
		{
			this->_splitLine.back().push_back(c);
		}
	}
	
	if (bracketing)
	{
		this->printErrorHeader();
		cerr << "The last bracket was not closed!" << endl;
		return false;
	}
	
	return true;
}

const std::string & Assembler::Line::getRawLine() const
{
	return this->_rawLine;
}

void Assembler::Line::setRawLine(const std::string& rawLine)
{
	this->_rawLine = rawLine;
}

uint Assembler::Line::getLineNumber() const
{
	return this->_lineNumber;
}

void Assembler::Line::setLineNumber(uint lineNumber)
{
	this->_lineNumber = lineNumber;
}

const std::string & Assembler::Line::getFilePath() const
{
	return this->_filePath;
}

void Assembler::Line::setFilePath(const std::string& filePath)
{
	this->_filePath = filePath;
}

std::shared_ptr<Assembler::SymbolMap> Assembler::Line::getSymbols() const
{
	return this->_symbols;
}

void Assembler::Line::setSymbols(std::shared_ptr<SymbolMap> symbols)
{
	this->_symbols = symbols;
}

vector<shared_ptr<Assembler::Line>> Assembler::Line::getAdditionaLines() const
{
	return this->_additionalLines;
}

void Assembler::Line::clearAdditionalLines()
{
	this->_additionalLines = vector<shared_ptr<Assembler::Line>>();
}
