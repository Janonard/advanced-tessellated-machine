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
#pragma once

#include <vector>
#include <memory>

#include "Executable/Definitions.h"
#include "Assembler/Definitions.h"
#include "Assembler/Argument.h"
#include "Assembler/Command.h"

namespace Assembler
{
	
	class Line
	{
	public: // methods
		Line();
		
		bool assembleLine();
		
		// Appends the machine code of this line to `outMemory`.
		bool linkLine(Memory& outMemory);
		
		void printErrorHeader();
		
	private: // methods
		
		bool splitLineInWords();
		
		bool identifySymbol();
		
		bool identifyArgument(uint argumentNumber);
		
		bool identifyCommand();
		
		bool identifyCombination();
		
		bool addSymbol(std::string name, NODE_INT_TYPE location);
		
		bool execDefine();
		
		bool execPosition();
		
		bool execSpace();
		
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
		
		const Argument& getArgument0() const;
		
		const Argument& getArgument1() const;
		
		const Command& getCommand() const;
		
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
		
		Command _command;
		
		const char cNumberIdentifier = '#';
		const char cAddressIdentifier = '$';
		const char cOpenBracket = '<';
		const char cClosedBracket = '>';
		const char cSpace = ' ';
		const char cTab = '\t';
		const char cSymbolIdentifier = ':';
		const char cEightBitNumberLength = 3;
		const char cSixteenBitNumberLength = 5;const char cComment = ';';
		
		const std::array<std::string,4> vRegisters = {{"null","acc","bcc","ccc"}};
		const std::array<std::string,3> vAddressRegisters = {{"$acc","$bcc","$ccc"}};
		const std::array<std::string,8> vChannels = {{"up","upRight","right","downRight","down","downLeft","left","upLeft"}};
		const std::array<std::string,34> vCommands = {{"halt", "splash", "move", "move8", "move16", "add", "add8", "add16", "negate", "bit_shift_l", "byte_shift_l", "bit_shift_r", "byte_shift_r", "swap", "compare", "jump", "jump_ops", "jump_opf", "jump_greater", "jump_lower", "jump_equal", "jump_nequal", "branch", "return", "dVerbose", "dSlow", "dCrash", "#define", "#var", "#position", "#space", "#include", "#binary", "NODE"}};
		
	};
	
}
