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
#include <boost/filesystem.hpp>

using namespace std;
using namespace Assembler;

Assembler::Line::Line() :
	_rawLine(),
	_splitLine(),
	_lineNumber(0),
	_filePath(),
	_symbols(),
	_additionalLines(),
	_memoryLocation(0),
	_memorySize(0),
	_wordOffset(0),
	_argument0(),
	_argument1(),
	_command()
{
	
}

bool Assembler::Line::assembleLine()
{
	if (!this->splitLineInWords())
		return false;
	
	if (! this->identifySymbol())
		return false;
	
	if (! this->identifyArgument(0))
		return false;
	
	if (! this->identifyArgument(1))
		return false;
	
	if (! this->identifyCommand())
		return false;
	
	if (! this->identifyCombination())
		return false;
	
	return true;
}

bool Assembler::Line::linkLine(Memory& outMemory)
{
	try
	{
		this->_argument0.solveSymbols(this->_symbols);
		this->_argument1.solveSymbols(this->_symbols);
	}
	catch (std::out_of_range e)
	{
		this->printErrorHeader();
		cerr << "Referenced symbol could not be found!" << endl;
		return false;
	}
	
	for (uint8_t character : this->_command.getCode())
	{
		outMemory.push_back(character);
	}
	
	for (uint8_t character : this->_argument0.getCode())
	{
		outMemory.push_back(character);
	}
	
	for (uint8_t character : this->_argument1.getCode())
	{
		outMemory.push_back(character);
	}
	
	return true;
}

void Assembler::Line::printErrorHeader()
{
	cerr << endl;
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
		if (c == cComment)
		{
			break;
		}
		
		if ((not bracketing) and (c == this->cOpenBracket))
		{
			bracketing = true;
		}
		if (bracketing and c == this->cClosedBracket)
		{
			bracketing = false;
		}
		
		if ((not bracketing) and (c == cSpace or c == cTab))
		{
			if (this->_splitLine.back().size() > 0)
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
	
	if (this->_splitLine.back() == string())
	{
		this->_splitLine.pop_back();
	}
	
	return true;
}

bool Assembler::Line::identifySymbol()
{
	if (this->_splitLine.size() > 0 && this->_splitLine[0].back() == this->cSymbolIdentifier)
	{
		string symbolName = this->_splitLine[0];
		symbolName.pop_back();
		
		if (! this->addSymbol(symbolName, this->_memoryLocation))
			return false;
		
		this->_wordOffset = 1;
	}
	return true;
}

bool Assembler::Line::identifyCommand()
{
	try
	{
		this->_command = Command();
	}
	catch (exception e)
	{
		this->printErrorHeader();
		cerr << "Could not allocate memory!" << endl;
		return false;
	}
	
	if (this->_splitLine.size() > this->_wordOffset)
	{
		auto iter = find(this->vCommands.begin(), this->vCommands.end(), this->_splitLine[this->_wordOffset]);
		if (iter != this->vCommands.end())
		{
			this->_command.setType(CommandType(iter-this->vCommands.begin()));
			return true;
		}
		else
		{
			this->printErrorHeader();
			cerr << "Unknown command!" << endl;
			cerr << "'" << this->_splitLine[this->_wordOffset] << "'" << endl;
			return false;
		}
	}
	else
	{
		this->_command.setType(CommandType::None);
		return true;
	}
}

bool Assembler::Line::addSymbol(std::string name, NODE_INT_TYPE location)
{
	if (this->_symbols.get() != nullptr)
	{
		if (name[0] == this->cNumberIdentifier)
		{
			this->printErrorHeader();
			cerr << "A symbol may not start with '" << this->cNumberIdentifier << "'!" << endl;
			return false;
		}
		
		if (name[0] == this->cAddressIdentifier)
		{
			this->printErrorHeader();
			cerr << "A symbol may not start with '" << this->cAddressIdentifier << "'!" << endl;
			return false;
		}
		
		if (name[0] == this->cOpenBracket)
		{
			this->printErrorHeader();
			cerr << "A symbol may not start with '" << this->cOpenBracket << "'!" << endl;
			return false;
		}
		
		if (name[0] == this->cClosedBracket)
		{
			this->printErrorHeader();
			cerr << "A symbol may not start with '" << this->cClosedBracket << "'!" << endl;
			return false;
		}
		
		if (find(this->vRegisters.begin(), this->vRegisters.end(), name) != this->vRegisters.end())
		{
			this->printErrorHeader();
			cerr << "A symbol may not have the same name as a register!" << endl;
			return false;
		}
		
		if (find(this->vChannels.begin(), this->vChannels.end(), name) != this->vChannels.end())
		{
			this->printErrorHeader();
			cerr << "A symbol may not have the same name as a channel!" << endl;
			return false;
		}
		
		this->_symbols->emplace(name, location);
		return true;
	}
	else
	{
		this->printErrorHeader();
		cerr << "Internal error!" << endl;
		return false;
	}
	return true;
}

bool Assembler::Line::execDefine()
{
	if (this->_argument1.getCode().size() < 2)
	{
		this->printErrorHeader();
		cerr << "Internal Error!" << endl;
		return false;
	}
	NODE_INT_TYPE location = 0;
	location = this->_argument1.getCode()[0] * 0x100 + this->_argument1.getCode()[1];
	this->addSymbol(this->_argument0.getSymbolName(), location);
	this->_argument0 = Argument();
	this->_argument1 = Argument();
	this->_memorySize = 0;
	return true;
}

bool Assembler::Line::execPosition()
{
	if (this->_argument0.getCode().size() < 2)
	{
		this->printErrorHeader();
		cerr << "Internal Error!" << endl;
		return false;
	}
	
	NODE_INT_TYPE finalLocation = this->_argument0.getCode()[0] * 0x100 + this->_argument0.getCode()[1];
	NODE_INT_TYPE currentLocation = this->_memoryLocation;
	Memory* memory = this->_command.getCodePointer();
	while (currentLocation < finalLocation)
	{
		memory->push_back(0);
		currentLocation++;
	}
	this->_argument0 = Argument();
	this->_argument1 = Argument();
	this->_memorySize = memory->size();
	return true;
}

bool Assembler::Line::execSpace()
{
	if (this->_argument0.getCode().size() < 2)
	{
		this->printErrorHeader();
		cerr << "Internal Error!" << endl;
		return false;
	}
	
	NODE_INT_TYPE finalLocation = this->_memoryLocation + this->_argument0.getCode()[0] * 0x100 + this->_argument0.getCode()[1];
	NODE_INT_TYPE currentLocation = this->_memoryLocation;
	Memory* memory = this->_command.getCodePointer();
	while (currentLocation < finalLocation)
	{
		memory->push_back(0);
		currentLocation++;
	}
	this->_argument0 = Argument();
	this->_argument1 = Argument();
	this->_memorySize = memory->size();
	return true;
}

bool Assembler::Line::execBinary()
{
	string pathArg = this->_argument0.getSymbolName();
	
	boost::filesystem::path pathToFile(this->getFilePath());
	pathToFile.remove_filename();
	pathToFile.append(pathArg);
	
	fstream file(pathToFile.c_str(), ios_base::in | ios_base::binary);
	if (! file)
	{
		this->printErrorHeader();
		cerr << "Could not open " << pathToFile << "!" << endl;
		return false;
	}
	
	char nextChar;
	while (file.get(nextChar))
	{
		this->_command.getCodePointer()->push_back(nextChar);
	}
	
	file.close();
	
	this->_argument0 = Argument();
	this->_argument1 = Argument();
	this->_memorySize = this->_command.getCodePointer()->size();
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

uint16_t Assembler::Line::getMemoryLocation() const
{
	return this->_memoryLocation;
}

void Assembler::Line::setMemoryLocation(uint16_t memoryLocation)
{
	this->_memoryLocation = memoryLocation;
}

uint16_t Assembler::Line::getMemorySize() const
{
	return this->_memorySize;
}

const Assembler::Argument & Assembler::Line::getArgument0() const
{
	return this->_argument0;
}

const Assembler::Argument & Assembler::Line::getArgument1() const
{
	return this->_argument1;
}

const Assembler::Command & Assembler::Line::getCommand() const
{
	return this->_command;
}

