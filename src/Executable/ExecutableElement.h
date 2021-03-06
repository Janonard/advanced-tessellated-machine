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
#include <stack>

#include "System/Node.h"
#include "Assembler/Line.h"
#include "Executable/Definitions.h"

/*
 * This contains the memory and the coordinates of an executable element.
 */
class ExecutableElement
{
public:
	/*
	 * Standart constructor
	 */
	ExecutableElement();
	
	/*
	 * Full access to _x
	 */
	int getX() const 
		{return this->_x;}
	void setX(int x)
		{this->_x = x;}
	
	/*
	 * Full access to _y
	 */
	int getY() const
		{return this->_y;}
	void setY(int y)
		{this->_y = y;}
	
	/*
	 * Adds a new line to _lines.
	 */
	void addLine(std::shared_ptr<Assembler::Line> newLine)
		{this->_lines.push_back(newLine);};
	
	/*
	 * Getter for _symbols
	 */
	std::shared_ptr<Assembler::SymbolMap> getSymbols() const
		{return this->_symbols;};
	
	/*
	 * Getter for _newLineLocation.
	 */
	NODE_INT_TYPE getNewLineLocation() const
		{return this->_newLineLocation;}
	
	/*
	 * Setter for _newLineLocation.
	 */
	void setNewLineLocation(NODE_INT_TYPE newNLL)
		{this->_newLineLocation = newNLL;};
	
	/*
	 * Iterates through our _lines, calls their lineLine
	 * method and adds the result to our memory. Returns true
	 * if successfull, false if not. If it isn't successfull,
	 * it will also write an error log to cerr.
	 */
	bool linkLinesToMemory();
	
	/*
	 * Returns the current file path, which is the top of
	 * _filePathStack.
	 */
	const std::string& getCurrentFilePath() const;
	
	/*
	 * Returns true if the file witht the given path is
	 * already included.
	 */
	bool isFileIncluded(std::string filePath) const;
	
	/*
	 * Clears _filePathStack and _includedFiles and
	 * pushes baseFilePath to them.
	 */
	void setBaseFilePath(std::string baseFilePath);
	
	/*
	 * Adds a new file to the _filePathStack and
	 * _includedFiles.
	 */
	bool addIncludedFile(std::string filePath);
	
	/*
	 * Pops the top of _filePathStack.
	 */
	bool closeIncludedFile();
	
	/*
	 * Returns a copy of the whole memory.
	 */
	Memory getMemory() const
		{return this->_memory;}
	
	/*
	 * Sets _memory
	 */
	void setMemory(Memory newMemory)
		{this->_memory = newMemory;}
	
	/*
	 * Converts every character in the string to
	 * an uint and adds it to _memory.
	 */
	void setMemory(std::string memoryFileText);
	
	/*
	 * returns the byte at the given position
	 */
	uint8_t get8BitValue(const NODE_INT_TYPE address) const;
	
	/*
	 * sets the byte at the given position to the given value
	 */
	void set8BitValue(const NODE_INT_TYPE address, uint8_t value);
	
	/*
	 * returns the byte at the given position and the following one
	 */
	uint16_t get16BitValue(const NODE_INT_TYPE address) const;
	
	/*
	 * sets the byte at the given positon and the following one
	 */
	void set16BitValue(const NODE_INT_TYPE address, uint16_t value);
	
private:
	
	/*
	 * The coordinates of our element.
	 */
	int _x, _y;
	
	/*
	 * Our memory
	 */
	Memory _memory;
	
	/*
	 * Our lines
	 */
	std::vector<std::shared_ptr<Assembler::Line>> _lines;
		
	/*
	 * A shared pointer to the global symbol map.
	 */
	std::shared_ptr<Assembler::SymbolMap> _symbols;
	
	/*
	 * A stack containing the currently open files.
	 */
	std::stack<std::string> _filePathStack;
	
	/*
	 * A vector containing all included file paths.
	 */
	std::vector<std::string> _includedFiles;
	
	/*
	 * The position of the next line.
	 */
	NODE_INT_TYPE _newLineLocation;
	
};
