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
#include <string>
#include <iostream>
#include <sstream>
#include "Assembler/Mnemonics.h"

using namespace std;

Assembler::Assembler() : 
	_fileName(),
	_lines(),
	_memoryOffset(0),
	_symbols()
{
}

const string& Assembler::getFileName() const
{
	return this->_fileName;
}

void Assembler::setFileName(const string fileName)
{
	this->_fileName = fileName;
}

NODE_INT_TYPE Assembler::getMemoryOffset() const
{
	return this->_memoryOffset;
}

void Assembler::setMemoryOffset(NODE_INT_TYPE newMemoryOffset)
{
	this->_memoryOffset = newMemoryOffset;
}

const vector<Symbol>& Assembler::getSymbols() const
{
	return this->_symbols;
}

void Assembler::setSymbols(const vector<Symbol>& symbols)
{
	this->_symbols = symbols;
}

void AssemblerFunc::splitLinesFilterComments(iostream& textStream, vector<vector<string>>* outFilteredSourcecode)
{
	auto filteredSourceCode = vector<vector<string>>();
	vector<string> emptyLine = vector<string>();
	emptyLine.push_back("");
	filteredSourceCode.push_back(emptyLine);
	
	bool commentFound = false;
	char nextCharacter = ' ';
	
	while (textStream.get(nextCharacter))
	{
		if (nextCharacter == (char) 10 || nextCharacter == (char) 13)
		{
			filteredSourceCode.push_back(emptyLine);
			commentFound = false;
		}
		else
		if (nextCharacter == ';')
		{
			commentFound = true;
		}
		else
		if (! commentFound)
		{
			if ((nextCharacter == ' ') | (nextCharacter == '\t'))
			{
				if (filteredSourceCode.back().back() != "")
				{
					filteredSourceCode.back().push_back("");
				}
			}
			else
			{
				filteredSourceCode.back().back().push_back(nextCharacter); // push our character at the end of the last word of the last line
			}
		}
	}
	
	*outFilteredSourcecode = filteredSourceCode;
}

shared_ptr<ExecutableElement> AssemblerFunc::assembleText(Assembler* assFile, iostream& textStream)
{
	vector<vector<string>> filteredSourceCode;
	splitLinesFilterComments(textStream, &filteredSourceCode);
	
	int lineNumber = 1;
	NODE_INT_TYPE position = assFile->getMemoryOffset();
	
	auto commands = vector<shared_ptr<CommandLine>>();
	for (vector<string> line : filteredSourceCode)
	{
		commands.push_back(make_shared<CommandLine>());
		if (! commands.back()->loadLine(line, assFile->getFileName(), lineNumber, position))
		{
			return nullptr;
		}
		
		vector<Symbol> symbols = assFile->getSymbols();
		for(Symbol newSymbol : commands.back()->getSymbols())
		{
			symbols.push_back(newSymbol);
		}
		assFile->setSymbols(symbols);
		
		uint newPosition = uint(position) + uint(commands.back()->getMachinecode().size());
		if (newPosition <= 0xFFFF)
		{
			position = NODE_INT_TYPE(newPosition);
		}
		else
		{
			cerr << "ERROR in line " << lineNumber << ": memory position out of range. The final binary may not be bigger than 0xFFFF!" << endl;
			return nullptr;
		}
		lineNumber++;
	}
	
	vector<uint8_t> newMachinecode = vector<uint8_t>();
	for (shared_ptr<CommandLine> singleCommand : commands)
	{
		if (! singleCommand->solveSymbols(assFile->getSymbols()))
		{
			return nullptr;
		}
		
		for (uint8_t nextCharacter : singleCommand->getMachinecode())
		{
			newMachinecode.push_back(nextCharacter);
		}
	}
	
	shared_ptr<ExecutableElement> executable = make_shared<ExecutableElement>();
	executable->setMemory(newMachinecode);
	return executable;
}
