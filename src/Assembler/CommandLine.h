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
#include "System/Node.h"
#include "Assembler/Symbol.h"

/*
 * The types of arguments
 */
enum ArgumentType
{
	NoArgument,Number8Bit,Number16Bit,SymbolNumber,Address,SymbolAddress,RegisterAddress,Register,Channel,FileName
};

/* An object of this class represents a line of assembler code.
 * It takes a line and assembles it.
 */
class CommandLine
{
public:
	
	CommandLine();
	
	/*
	 * loads a code line and assembles it
	 */
	bool loadLine(const std::vector<std::string>& line, const std::string& fileName, int lineNumber, NODE_INT_TYPE memPositio);
	
	/*
	 * looks up the symbols that are needed to assemble the line
	 */
	bool solveSymbols(const std::vector<Symbol>& symbols);
	
	std::vector<uint8_t> getMachinecode() const;
	
	const std::vector<Symbol>& getSymbols() const;
	
private:
	/*
	 * takes the first word of our line and creates a symbol with it.
	 */
	bool generateSymbol(const std::string& word, NODE_INT_TYPE position);
	
	/*
	 * identifies the type of the command
	 */
	bool identifyCommand(const std::string& word);
	
	/*
	 * identifies the type of the argument and returns the binary code
	 * that represents it.
	 */
	bool identifyArgument(const std::string& word, ArgumentType* argument, std::vector<uint8_t>* argumentCode);
	
	/*
	 * returns our line as a whole string.
	 */
	std::string getLineAsString() const;
	
	/*
	 * prints the file name, the line number and the line to cerr.
	 */
	void printStandartErrorHeader();
	
private:
	
	/*
	 * the path of the file we belong to
	 */
	std::string _fileName;
	
	/*
	 * our assembler code line
	 */
	std::vector<std::string> _line;
	
	/*
	 * our number line in our file
	 */
	int _lineNumber;
	
	NODE_INT_TYPE _memPosition;
	
	bool _properCommand;
	
	/*
	 * the assembled code of our line, splitted into the code for the command,
	 * the first argument and the second argument.
	 */
	char _commandCode;
	std::vector<uint8_t> _argument1Code;
	std::vector<uint8_t> _argument2Code;
	
	/*
	 * the types of our arguments
	 */
	ArgumentType _argument1;
	ArgumentType _argument2;
	
	/*
	 * the word offset that is caused by the label
	 */
	NODE_INT_TYPE _commandOffset;
	
	/*
	 * all symbols that are defined in this line
	 */
	std::vector<Symbol> _symbols;
	
};
