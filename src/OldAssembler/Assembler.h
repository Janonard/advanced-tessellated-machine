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
#include <vector>
#include <memory>
#include "Assembler/CommandLine.h"
#include "Executable/ExecutableElement.h"

// An object of this class represents a file with assembler code.
// It reads the code, splits it into lines and assembles them.
class Assembler
{
public:
	/*
	 * Standart constructor
	 */
	Assembler();
	
	/*
	 * file name access
	 */
	const std::string& getFileName() const;
	void setFileName(const std::string fileName);
	
	/*
	 * included files access
	 */
	const std::vector<std::string>& getIncludedFiles() const;
	void setIncludedFiles(const std::vector<std::string>& includedFiles);
	
	/*
	 * memory offset access
	 */
	NODE_INT_TYPE getMemoryOffset() const;
	void setMemoryOffset(NODE_INT_TYPE newMemoryOffset);
	
	/*
	 * symbol vector access
	 */
	const std::vector<Symbol>& getSymbols() const;
	void setSymbols(const std::vector<Symbol>& symbols);

protected: // variables
	
	std::string _fileName;
	
	std::vector<std::string> _includedFiles;
	
	std::vector<CommandLine> _lines;
	
	NODE_INT_TYPE _memoryOffset;
	
	std::vector<Symbol> _symbols;
};

namespace AssemblerFunc
{
	/*
	 * Splits the given source code into words and lines while leaving out comments.
	 */
	void splitLinesFilterComments(std::iostream& textStream, std::vector<std::vector<std::string>>* outFilteredSourcecode);
	
	/*
	 * Does the whole assembly.
	 */
	std::shared_ptr<ExecutableElement> assembleText(Assembler* assFile, std::iostream& textStream);
}
