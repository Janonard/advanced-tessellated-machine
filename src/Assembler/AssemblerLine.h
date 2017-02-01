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

typedef std::unordered_map<std::string, NODE_INT_TYPE> SymbolMap;

class AssemblerLine
{
public: // methods
	AssemblerLine();
	
	bool assembleLine();
	
	bool linkLine(Memory* outMemory);
	
	void printErrorHeader();
	
private: // methods
	
	bool splitLineInWords();
	
public: // property access
	
	const std::string& getRawLine() const;
	void setRawLine(const std::string& rawLine);
	
	uint getLineNumber() const;
	void setLineNumber(uint lineNumber);
	
	const std::string& getFilePath() const;
	void setFilePath(const std::string& filePath);
	
	std::shared_ptr<SymbolMap> getSymbols() const;
	void setSymbols(std::shared_ptr<SymbolMap> symbols);
	
	std::vector<std::shared_ptr<AssemblerLine>> getAdditionaLines() const;
	void clearAdditionalLines();
	
private: // properties
	
	std::string _rawLine;
	
	std::vector<std::string> _splitLine;
	
	uint _lineNumber;
	
	std::string _filePath;
	
	std::shared_ptr<SymbolMap> _symbols;
	
	std::vector<std::shared_ptr<AssemblerLine>> _additionalLines;
	
};
