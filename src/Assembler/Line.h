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
#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "System/Node.h"
#include "Executable/ExecutableElement.h"
#include "Assembler/Argument.h"

namespace Assembler
{

	typedef std::unordered_map<std::string, NODE_INT_TYPE> SymbolMap;
	
	class Line
	{
	public: // methods
		Line();
		
		bool assembleLine();
		
		bool linkLine(Memory* outMemory);
		
		void printErrorHeader();
		
	private: // methods
		
		bool splitLineInWords();
		
		bool identifySymbol();
		
		bool identifyArgument(uint argumentNumber);
		
		bool identifyCommand();
		
		bool addSymbol(std::string name);
		
		bool codeToInt(const std::string& word, std::shared_ptr<Memory> memory);
		
	public: // property access
		
		const std::string& getRawLine() const;
		void setRawLine(const std::string& rawLine);
		
		uint getLineNumber() const;
		void setLineNumber(uint lineNumber);
		
		const std::string& getFilePath() const;
		void setFilePath(const std::string& filePath);
		
		std::shared_ptr<SymbolMap> getSymbols() const;
		void setSymbols(std::shared_ptr<SymbolMap> symbols);
		
		std::vector<std::shared_ptr<Line>> getAdditionaLines() const;
		void clearAdditionalLines();
		
		NODE_INT_TYPE getMemoryLocation() const;
		void setMemoryLocation(NODE_INT_TYPE memoryLocation);
		
		NODE_INT_TYPE getMemorySize() const;
		
	private: // properties
		
		std::string _rawLine;
		
		std::vector<std::string> _splitLine;
		
		uint _lineNumber;
		
		std::string _filePath;
		
		std::shared_ptr<SymbolMap> _symbols;
		
		std::vector<std::shared_ptr<Line>> _additionalLines;
		
		NODE_INT_TYPE _memoryLocation;
		
		NODE_INT_TYPE _memorySize;
		
		uint _wordOffset;
		
		Argument _argument0;
		
		Argument _argument1;
		
		uint8_t _command;
		
		const char cNumberIdentifier = '#';
		const char cAddressIdentifier = '$';
		const char cOpenBracket = '<';
		const char cClosedBracket = '>';
		const char cEightBitNumberLength = 3;
		const char cSixteenBitNumberLength = 5;
		
		const std::array<std::string,4> vRegisters = {{"acc","bcc","ccc","null"}};
		const std::array<std::string,3> vAddressRegisters = {{"$acc","$bcc","$ccc"}};
		const std::array<std::string,8> vChannels = {{"up","upRight","right","downRight","down","downLeft","left","upLeft"}};
		
	};
	
}
