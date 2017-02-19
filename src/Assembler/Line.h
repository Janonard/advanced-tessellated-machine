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
#include "Assembler/Resources.h"

namespace Assembler
{
	/*
	 * An object of Line assembles a single line and contains all required
	 * information.
	 */
	class Line
	{
	public: // methods
		Line();
		
		/*
		 * Tries to assemble the code in _rawLine. Returns true if successfull,
		 * false if not. If it isn't successfull, it will also write an error
		 * log to cerr.
		 */
		bool assembleLine();
		
		/*
		 * This method solves all symbols, if required, and appends the code
		 * of the command and the first and second argument to the referenced
		 * memory. Returns true if successfull, false if not. If it isn't
		 * successfull, it will also write an error log to cerr.
		 */
		bool linkLine(Memory& outMemory);
		
		/*
		 * Prints a header containing the file name, the line number and the
		 * raw line to cerr.
		 */
		void printErrorHeader();
		
	private: // methods
		
		/*
		 * Tries to split _rawLine into single words and saves the result in
		 * _splitLine. Also, it removes comments. Returns true if successfull,
		 * false if not. If it isn't successfull, it will also write an error
		 * log to cerr.
		 */
		bool splitLineInWords();
		
		/*
		 * Looks up whether the _splitLine contains a symbol notation. If it
		 * does, it runs addSymbol with the symbol's name and our current
		 * _memoryLocation. If addSymbol returns true, it will also increase
		 * _wordOffset to 1. Returns true if successfull, false if not.
		 * If it isn't successfull, it will also write an error log to cerr.
		 */
		bool identifySymbol();
		
		/*
		 * Identifies the argument with the given number and sets _argument0
		 * or _argument1 accordingly. Returns true if successfull, false if not.
		 * If it isn't successfull, it will also write an error log to cerr.
		 */
		bool identifyArgument(uint argumentNumber);
		
		/*
		 * Identifies the command in _rawLine and sets _command accordingly.
		 * It only looks at the command, not the combination of the command
		 * and the two arguments. Returns true if successfull, false if not.
		 * If it isn't successfull, it will also write an error log to cerr.
		 */
		bool identifyCommand();
		
		/*
		 * Identifies the command/argument combination based upon the results
		 * of identifyArgument and identifyCommand. Returns true if successfull,
		 * false if not. If it isn't successfull, it will also write an error
		 * log to cerr.
		 */
		bool identifyCombination();
		
		/*
		 * Adds a symbol to _symbols if the given name wasn't used before and
		 * may not be missinterpreted as another argument. Returns true if
		 * successfull, false if not. If it isn't successfull, it will also
		 * write an error log to cerr.
		 */
		bool addSymbol(std::string name, NODE_INT_TYPE location);
		
		/*
		 * Defines a symbol. Returns true if successfull, false if not. If it
		 * isn't successfull, it will also write an error log to cerr.
		 */
		bool execDefine();
		
		/*
		 * Fills the memory until the following command will have the given
		 * position. Returns true if successfull, false if not. If it isn't
		 * successfull, it will also write an error log to cerr.
		 */
		bool execPosition();
		
		/*
		 * Fills the memory with a given amount of zeroes. Returns true if
		 * successfull, false if not. If it isn't successfull, it will also
		 * write an error log to cerr.
		 */
		bool execSpace();
		
		/*
		 * Loads a binary and saves it as the _command's code. Returns true if
		 * successfull, false if not. If it isn't successfull, it will also
		 * write an error log to cerr.
		 */
		bool execBinary();
		
	public: // property access
		
		/*
		 * Returns a constant refernce to _rawLine.
		 */
		const std::string& getRawLine() const;
		
		/*
		 * Setter for _rawLine
		 */
		void setRawLine(const std::string& rawLine);
		
		/*
		 * Getter for _lineNumber
		 */
		uint getLineNumber() const;
		
		/*
		 * Setter for _lineNumber
		 */
		void setLineNumber(uint lineNumber);
		
		/*
		 * Returns a constant refernce to _filePath.
		 */
		const std::string& getFilePath() const;
		
		/*
		 * Setter for _filePath
		 */
		void setFilePath(const std::string& filePath);
		
		/*
		 * Getter for _symbols
		 */
		std::shared_ptr<SymbolMap> getSymbols() const;
		
		/*
		 * Setter for _symbols
		 */
		void setSymbols(std::shared_ptr<SymbolMap> symbols);
		
		/*
		 * Getter for _memoryLocation
		 */
		NODE_INT_TYPE getMemoryLocation() const;
		
		/*
		 * Setter for _memoryLocation
		 */
		void setMemoryLocation(NODE_INT_TYPE memoryLocation);
		
		/*
		 * Getter for _memorySize
		 */
		NODE_INT_TYPE getMemorySize() const;
		
		/*
		 * Returns a constant refernce to _argument0
		 */
		const Argument& getArgument0() const;
		
		/*
		 * Returns a constant refernce to _argument1
		 */
		const Argument& getArgument1() const;
		
		/*
		 * Returns a constant refernce to _command
		 */
		const Command& getCommand() const;
		
		/*
		 * Getter for _resources
		 */
		std::shared_ptr<Resources> getResources() const;
		
		/*
		 * Setter for _resources
		 */
		void setResources(std::shared_ptr<Resources> resources);
		
	private: // properties
		
		/*
		 * The raw line that we are assembling. Only
		 * used by splitLineInWords and printErrorHeader.
		 */
		std::string _rawLine;
		
		/*
		 * The split line generated by splitLineInWords.
		 * All `identify` commands use this.
		 */
		std::vector<std::string> _splitLine;
		
		/*
		 * Our line number, as set by the object owner,
		 * mainly the Executable.
		 */
		uint _lineNumber;
		
		/*
		 * The path to the file that contains our _rawLine.
		 */
		std::string _filePath;
		
		/*
		 * The shared pointer to the global symbol map.
		 */
		std::shared_ptr<SymbolMap> _symbols;
		
		/*
		 * Our final location in the memory.
		 */
		NODE_INT_TYPE _memoryLocation;
		
		/*
		 * Our final size in the memory. This isn't always
		 * equal to the code of the command and the arguments.
		 */
		NODE_INT_TYPE _memorySize;
		
		/*
		 * The word offset for the `identify` methods.
		 * If the line contains a symbol, this is 1.
		 */
		uint _wordOffset;
		
		/*
		 * Our first argument
		 */
		Argument _argument0;
		
		/*
		 * Our second argument
		 */
		Argument _argument1;
		
		/*
		 * Our command
		 */
		Command _command;
		
		/*
		 * A shared pointer to the global assembly resources
		 * object.
		 */
		std::shared_ptr<Resources> _resources;
		
	};
	
}
