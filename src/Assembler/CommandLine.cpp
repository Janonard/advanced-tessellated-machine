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
#include "Assembler/CommandLine.h"

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <bitset>

#include "Assembler/Mnemonics.h"
#include "Assembler/Assembler.h"


using namespace std;

CommandLine::CommandLine() :
	_fileName(),
	_includedFiles(),
	_line(),
	_lineNumber(0),
	_memPosition(0),
	_properCommand(false),
	_commandCode(0),
	_argument1Code(),
	_argument2Code(),
	_argument1(NoArgument),
	_argument2(NoArgument),
	_commandOffset(0),
	_symbols()
{
}

bool CommandLine::loadLine(const vector<string>& line, const std::string& fileName, int lineNumber, NODE_INT_TYPE memPosition)
{
	this->_line = line;
	this->_fileName = fileName;
	this->_lineNumber = lineNumber;
	this->_memPosition = memPosition;
	
	if (line[0].back() == ':')
	{
		this->_commandOffset = 1;
		std::string label = line[0];
		label.pop_back(); // removing the ':'
		if (! this->generateSymbol(label, memPosition))
			return false;
	}
	
	if ((line.size() == this->_commandOffset) | (line[0].size() == 0))
		return true; // this is an empty line, it's okay
	
	if (line[_commandOffset] != "")
	{
		if (line.size() >= this->_commandOffset+2 && line[this->_commandOffset+1] != "")
		{
			if (! this->identifyArgument(line[this->_commandOffset+1], &this->_argument1, &this->_argument1Code)) return false;
		}
		if (line.size() >= this->_commandOffset+3 && line[this->_commandOffset+2] != "")
		{
			if (! this->identifyArgument(line[this->_commandOffset+2], &this->_argument2, &this->_argument2Code)) return false;
		}
		if (! this->identifyCommand(line[this->_commandOffset])) return false;
	}
	return true;
}

bool CommandLine::solveSymbols(const vector< Symbol >& symbols)
{
	if (this->_argument1 == SymbolAddress || this->_argument1 == SymbolNumber)
	{
		string symbolName = this->_line[this->_commandOffset+1];
		
		if (this->_argument1 == SymbolNumber)
		{
			symbolName.pop_back();				// cutting off the hashtags
			symbolName = symbolName.substr(1);
		}
		
		for (Symbol singleSymbol : symbols)
		{
			if (singleSymbol.name == symbolName)
			{
				this->_argument1Code[0] = singleSymbol.position / 0x100;
				this->_argument1Code[1] = singleSymbol.position % 0x100;
				return true;
			}
		}
		this->printStandartErrorHeader();
		cerr << "The symbol '" << symbolName << "' doesn't exist!" << endl;
		return false;
	}
	else
	if (this->_argument2 == SymbolAddress)
	{
		for (Symbol singleSymbol : symbols)
		{
			if (singleSymbol.name == this->_line[this->_commandOffset+2])
			{
				this->_argument2Code[0] = singleSymbol.position / 0x100;
				this->_argument2Code[1] = singleSymbol.position % 0x100;
				return true;
			}
		}
		this->printStandartErrorHeader();
		cerr << "The symbol '" << this->_line[this->_commandOffset+2] << "' doesn't exist!" << endl;
		return false;
	}
	else
		return true;
}

bool CommandLine::generateSymbol(const string& word, NODE_INT_TYPE position)
{
	if (word[0] == '#')
	{
		this->printStandartErrorHeader();
		cerr << "A symbol may not begin with '#'!" << endl;
		return false;
	}
	else
	if (word[0] == '$')
	{
		this->printStandartErrorHeader();
		cerr << "A symbol may not begin with '$'!" << endl;
		return false;
	}
	else
	if (word[0] == '@')
	{
		this->printStandartErrorHeader();
		cerr << "A symbol may not begin with '@'!" << endl;
		return false;
	}
	else
	if ((word == "null") | (word == "acc") | (word == "bcc") | (word == "ccc"))
	{
		this->printStandartErrorHeader();
		cerr << "A symbol may not be a register name!" << endl;
		return false;
	}
	else
	if ((word == "up") | (word == "upRight") | (word == "right") | (word == "downRight") | (word == "down") | (word == "downLeft") | (word == "left") | (word == "upLeft"))
	{
		this->printStandartErrorHeader();
		cerr << "A symbol may not be a channel name!" << endl;
		return false;
	}
	else
	if ((word == MNE_SPLASH) | (word == MNE_HALT) | (word == MNE_MOVE) | (word == MNE_MOVE8) | (word == MNE_MOVE16) | (word == MNE_ADD) | (word == MNE_ADD8) | (word == MNE_ADD16) | (word == MNE_NEGATE) | (word == MNE_BIT_SHIFT_L) | (word == MNE_BIT_SHIFT_R) | (word == MNE_BYTE_SHIFT_L) | (word == MNE_BYTE_SHIFT_R) | (word == MNE_SWAP) | (word == MNE_COMPARE) | (word == MNE_A_JUMP) | (word == MNE_A_JUMP_OPS) | (word == MNE_A_JUMP_OPF) | (word == MNE_A_JUMP_EQUAL) | (word == MNE_A_JUMP_NON_EQUAL) | (word == MNE_A_JUMP_GREATER) | word == (MNE_A_JUMP_LOWER) | (word == MNE_BRANCH) | (word == MNE_RETURN) | (word == MNE_META_VAR) | (word == MNE_META_DEFINE) | (word == MNE_META_POSITION) | (word == MNE_META_SPACE) | (word == MNE_META_INCLUDE) | (word == MNE_META_BINARY))
	{
		this->printStandartErrorHeader();
		cerr << "A symbol may not be a command name!" << endl;
		return false;
	}
	else
	{
		Symbol symbol = Symbol();
		symbol.name = word;
		symbol.position = position;
		this->_symbols.push_back(symbol);
		return true;
	}
}

vector<uint8_t> CommandLine::getMachinecode() const
{
	vector<uint8_t> machinecode;
	
	if (this->_properCommand)
	{
		machinecode.push_back(this->_commandCode);
	}
	
	for (char code : this->_argument1Code)
	{
		machinecode.push_back(code);
	}
	
	for (char code : this->_argument2Code)
	{
		machinecode.push_back(code);
	}
	
	return machinecode;
}

const vector<Symbol>& CommandLine::getSymbols() const
{
	return this->_symbols;
}

const std::vector<std::string> & CommandLine::getIncludedFiles() const
{
	return this->_includedFiles;
}

void CommandLine::setIncludedFiles(const std::vector<std::string>& includedFiles)
{
	this->_includedFiles = includedFiles;
}

bool CommandLine::identifyCommand(const string& word)
{
	if (word == MNE_SPLASH)
	{
		if (this->_argument1 == NoArgument && this->_argument2 == NoArgument)
		{
			this->_commandCode = (char) Command::Splash;
			this->_properCommand = true;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Too many arguments!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_HALT)
	{
		if (this->_argument1 == NoArgument && this->_argument2 == NoArgument)
		{
			this->_commandCode = (char) Command::Halt;
			this->_properCommand = true;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Too many arguments!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_MOVE)
	{
		if ((this->_argument1 == NoArgument) | (this->_argument2 == NoArgument))
		{
			this->printStandartErrorHeader();
			cerr << "Not enough arguments!" << endl;
			return false;
		}
		else
		if ((this->_argument1 == SymbolNumber) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Value_Register_16_bit;
			return true;
		}
		else
		if ((this->_argument1 == Channel) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Channel_Register;
			return true;
		}
		else
		if ((this->_argument1 == Register) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Register_Register;
			return true;
		}
		else
		if ((this->_argument1 == Register) && (this->_argument2 == Channel))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Register_Channel;
			return true;
		}
		else
		if ((this->_argument1 == Number8Bit) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Value_Register_8_bit;
			return true;
		}
		else
		if ((this->_argument1 == Number16Bit) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Value_Register_16_bit;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_MOVE8)
	{
		if ((this->_argument1 == NoArgument) | (this->_argument2 == NoArgument))
		{
			this->printStandartErrorHeader();
			cerr << "Not enough arguments!" << endl;
			return false;
		}
		else
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Address_Register_8_bit;
			return true;
		}
		else
		if ((this->_argument1 == RegisterAddress) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_RegAddress_Register_8_bit;
			return true;
		}
		else
		if ((this->_argument1 == Register) && ( (this->_argument2 == Address) | (this->_argument2 == SymbolAddress) ))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Register_Address_8_bit;
			return true;
		}
		else
		if ((this->_argument1 == Register) && (this->_argument2 == RegisterAddress))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Register_RegAddress_8_bit;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_MOVE16)
	{
		if ((this->_argument1 == NoArgument) | (this->_argument2 == NoArgument))
		{
			this->printStandartErrorHeader();
			cerr << "Not enough arguments!" << endl;
			return false;
		}
		else
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Address_Register_16_bit;
			return true;
		}
		else
		if ((this->_argument1 == RegisterAddress) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_RegAddress_Register_16_bit;
			return true;
		}
		else
		if ((this->_argument1 == Register) && ( (this->_argument2 == Address) | (this->_argument2 == SymbolAddress) ))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Register_Address_16_bit;
			return true;
		}
		else
		if ((this->_argument1 == Register) && (this->_argument2 == RegisterAddress))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Move_Register_RegAddress_16_bit;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_ADD)
	{
		if ((this->_argument1 == NoArgument) | (this->_argument2 == NoArgument))
		{
			this->printStandartErrorHeader();
			cerr << "Not enough arguments!" << endl;
			return false;
		}
		else
		if ((this->_argument1 == Channel) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Add_Channel_Register;
			return true;
		}
		else
		if ((this->_argument1 == Register) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Add_Register_Register;
			return true;
		}
		else
		if ((this->_argument1 == Number8Bit) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Add_Value_Register_8_bit;
			return true;
		}
		else
		if ((this->_argument1 == Number16Bit) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Add_Value_Register_16_bit;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_ADD8)
	{
		if ((this->_argument1 == NoArgument) | (this->_argument2 == NoArgument))
		{
			this->printStandartErrorHeader();
			cerr << "Not enough arguments!" << endl;
			return false;
		}
		else
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Add_Address_Register_8_bit;
			return true;
		}
		else
		if ((this->_argument1 == RegisterAddress) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Add_RegAddress_Register_8_bit;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_ADD16)
	{
		if ((this->_argument1 == NoArgument) | (this->_argument2 == NoArgument))
		{
			this->printStandartErrorHeader();
			cerr << "Not enough arguments!" << endl;
			return false;
		}
		else
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Add_Address_Register_16_bit;
			return true;
		}
		else
		if ((this->_argument1 == RegisterAddress) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = (char) Command::Add_RegAddress_Register_16_bit;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_NEGATE)
	{
		if ((this->_argument1 == Register) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Negate_Register;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_BIT_SHIFT_L)
	{
		if ((this->_argument1 == Register) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Shift_Register_Left_Bitwise;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_BYTE_SHIFT_L)
	{
		if ((this->_argument1 == Register) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Shift_Register_Left_Bytewise;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_BIT_SHIFT_R)
	{
		if ((this->_argument1 == Register) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Shift_Register_Right_Bitwise;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_BYTE_SHIFT_R)
	{
		if ((this->_argument1 == Register) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Shift_Register_Right_Bytewise;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_SWAP)
	{
		if ((this->_argument1 == Register) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Swap_Register;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_COMPARE)
	{
		if ((this->_argument1 == Register) && (this->_argument2 == Register))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Compare_Register_Register;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_A_JUMP)
	{
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Absolute_Jump;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_A_JUMP_OPS)
	{
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Absolute_Jump_OperationSuccessfull;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	if (word == MNE_A_JUMP_OPF)
	{
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Absolute_Jump_OperationFailed;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_A_JUMP_GREATER)
	{
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Absolute_Jump_Greater;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_A_JUMP_LOWER)
	{
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Absolute_Jump_Lower;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_A_JUMP_EQUAL)
	{
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Absolute_Jump_Equal;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_A_JUMP_NON_EQUAL)
	{
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Absolute_Jump_Non_Equal;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_BRANCH)
	{
		if (( (this->_argument1 == Address) | (this->_argument1 == SymbolAddress) ) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Branch;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_RETURN)
	{
		if ((this->_argument1 == NoArgument) && (this->_argument2 == NoArgument))
		{
			this->_properCommand = true;
			this->_commandCode = Command::Return;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Return requires no arguments!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_META_VAR)
	{
		this->_properCommand = false;
		
		if ((this->_argument1 == Number8Bit) | (this->_argument1 == Number16Bit))
		{
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_META_DEFINE)
	{
		this->_properCommand = false;
		
		if ((this->_argument1 == SymbolAddress) && (this->_argument2 == Address))
		{
			// generate the symbol
			uint32_t address = this->_argument2Code[1];
			address += this->_argument2Code[0] * 0x100;
			
			
			// we need to delete all assembled data,
			// because we don't show up in the assembled binary.
			this->_argument1 = NoArgument;
			this->_argument2 = NoArgument;
			this->_argument1Code = vector<uint8_t>();
			this->_argument2Code = vector<uint8_t>();
			this->_properCommand = false;
			
			return this->generateSymbol(this->_line[this->_commandOffset+1], address);
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_META_POSITION)
	{
		this->_properCommand = false;
		
		if ((this->_argument1 == Address) && (this->_argument2 == NoArgument))
		{
			uint32_t address = this->_argument1Code[1];
			address += this->_argument1Code[0] * 0x100;
			this->_argument1Code = vector<uint8_t>();
			
			if (address <= this->_memPosition)
			{
				this->printStandartErrorHeader();
				cerr << "Targeted position is lower than our position!" << endl;
				cerr << "Cannot move backwards!" << endl;
				return false;
			}
			
			for (uint32_t position = this->_memPosition; position < address; position++)
			{
				this->_argument1Code.push_back(0x00);
			}
			
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_META_SPACE)
	{
		this->_properCommand = false;
		
		if ((this->_argument1 == Address) && (this->_argument2 == NoArgument))
		{
			uint32_t spaceLength = this->_argument1Code[1];
			spaceLength += this->_argument1Code[0] * 0x100;
			this->_argument1Code = vector<uint8_t>();
			
			for (uint32_t index = 0; index < spaceLength; index++)
			{
				this->_argument1Code.push_back(0x00);
			}
			
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_META_INCLUDE)
	{
		this->_properCommand = false;
		
		if ((this->_argument1 == FileName) && (this->_argument2 == NoArgument))
		{
			string pathArg = this->_line[this->_commandOffset + 1];
			if (pathArg.size() <= 2)
			{
				this->printStandartErrorHeader();
				cerr << "No path given!" << endl;
				return false;
			}
			pathArg.pop_back();
			pathArg = pathArg.substr(1); // removing the bracelets
			
			boost::filesystem::path pathToFile(this->_fileName);
			pathToFile.remove_filename();
			pathToFile.append(pathArg);
			
			if (find(this->_includedFiles.begin(), this->_includedFiles.end(), pathToFile.string()) != this->_includedFiles.end())
			{
				return true;
			}
			
			this->_includedFiles.push_back(pathToFile.string());
			
			fstream sourceFileStream(pathToFile.string(), ios_base::in);
			
			std::shared_ptr<Assembler> includedAssemblerFile = std::make_shared<Assembler>();
			includedAssemblerFile->setMemoryOffset(this->_memPosition);
			includedAssemblerFile->setIncludedFiles(this->_includedFiles);
			includedAssemblerFile->setFileName(pathToFile.string());
			includedAssemblerFile->setSymbols(this->_symbols);
			
			shared_ptr<ExecutableElement> execElement = AssemblerFunc::assembleText(includedAssemblerFile.get(), sourceFileStream);
			
			this->_includedFiles = includedAssemblerFile->getIncludedFiles();
			this->_symbols = includedAssemblerFile->getSymbols();
			
			if (execElement.get() == nullptr)
			{
				this->printStandartErrorHeader();
				cerr << "Could not assemble file!" << endl;
				return false;
			}
			
			for (uint8_t character : execElement->getMemory())
			{
				this->_argument1Code.push_back((char) character);
			}
			for (Symbol newSymbol : includedAssemblerFile->getSymbols())
			{
				this->_symbols.push_back(newSymbol);
			}
			
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_META_BINARY)
	{
		this->_properCommand = false;
		if ((this->_argument1 == FileName) && (this->_argument2 == NoArgument))
		{
			string pathArg = this->_line[this->_commandOffset + 1];
			if (pathArg.size() <= 2)
			{
				this->printStandartErrorHeader();
				cerr << "No path given!" << endl;
				return false;
			}
			pathArg.pop_back();
			pathArg = pathArg.substr(1); // removing the bracelets
			
			boost::filesystem::path pathToFile(this->_fileName);
			pathToFile.remove_filename();
			pathToFile.append(pathArg);
			
			fstream file(pathToFile.c_str(), ios_base::in | ios_base::binary);
			if (! file)
			{
				this->printStandartErrorHeader();
				cerr << "Error while opening " << pathToFile << "!" << endl;
				cerr << "Are you sure this file exists?" << endl;
				return false;
			}
			
			char nextChar;
			while (file.get(nextChar))
				this->_argument1Code.push_back(nextChar);
			
			file.close();
			
			return true;			
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_DEBUG_SET_VERBOSE)
	{
		this->_properCommand = true;
		if ((this->_argument1 == Number8Bit) && (this->_argument2 == NoArgument))
		{
			this->_commandCode = Debug_Set_Verbose;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_DEBUG_SLOW)
	{
		this->_properCommand = true;
		if ((this->_argument1 == Number8Bit) && (this->_argument2 == NoArgument))
		{
			this->_commandCode = Debug_Speed;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	if (word == MNE_DEBUG_CRASH)
	{
		this->_properCommand = true;
		if ((this->_argument1 == NoArgument) && (this->_argument2 == NoArgument))
		{
			this->_commandCode = Debug_Crash;
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unsupported argument combination!" << endl;
			return false;
		}
	}
	else
	{
		this->printStandartErrorHeader();
		cerr << "Unknown command " << word << endl;
		return false;
	}
}

bool CommandLine::identifyArgument(const std::string& word, ArgumentType* argument, vector<uint8_t>* argumentCode)
{
	if (word[0] == '#')
	{
		if (word.back() == '#')
		{
			*argument = ArgumentType::SymbolNumber;
			argumentCode->push_back(0); // a placeholder
			argumentCode->push_back(0);
			return true;
		}
		else
		if (word.size() == 3)
		{
			*argument = ArgumentType::Number8Bit;
			
			uint8_t nextChar = 0;
			try
			{
				nextChar = stoi(word.substr(1,2),nullptr,16);
			}
			catch (invalid_argument e)
			{
				this->printStandartErrorHeader();
				cerr << "Invalid number, only hexadecimal values are allowed." << endl;
				return false;
			}
			
			argumentCode->push_back(nextChar);
			return true;
		}
		else
		if (word.size() == 5)
		{
			*argument = ArgumentType::Number16Bit;
			uint8_t firstChar = 0;
			uint8_t secondChar = 0;
			
			try
			{
				firstChar = stoi(word.substr(1,2),nullptr,16);
				secondChar = stoi(word.substr(3,2),nullptr,16);
			}
			catch (invalid_argument e)
			{
				this->printStandartErrorHeader();
				cerr << "Invalid number, only hexadecimal values are allowed." << endl;
				return false;
			}
			
			argumentCode->push_back(firstChar);
			argumentCode->push_back(secondChar);
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Unknown number length!" << endl;
			return false;
		}
	}
	else
	if (word[0] == '$')
	{
		if (word == "$acc")
		{
			*argument = ArgumentType::RegisterAddress;
			argumentCode->push_back(0x01);
			return true;
		}
		else
		if (word == "$bcc")
		{
			*argument = ArgumentType::RegisterAddress;
			argumentCode->push_back(0x02);
			return true;
		}
		else
		if (word == "$ccc")
		{
			*argument = ArgumentType::RegisterAddress;
			argumentCode->push_back(0x03);
			return true;
		}
		else
		if (word.size() == 5)
		{
			*argument = ArgumentType::Address;
			uint8_t firstChar = 0;
			uint8_t secondChar = 0;
			
			try
			{
				firstChar = stoi(word.substr(1,2),nullptr,16);
				secondChar = stoi(word.substr(3,2),nullptr,16);
			}
			catch (invalid_argument e)
			{
				this->printStandartErrorHeader();
				cerr << "Invalid number, only hexadecimal values are allowed." << endl;
				return false;
			}
			
			argumentCode->push_back(firstChar);
			argumentCode->push_back(secondChar);
			return true;
		}
		else
		{
			this->printStandartErrorHeader();
			cerr << "Invalid address!" << endl;
			return false;
		}
	}
	else
	if (word == "null")
	{
		*argument = ArgumentType::Register;
		argumentCode->push_back(0x00);
		return true;
	}
	else
	if (word == "acc")
	{
		*argument = ArgumentType::Register;
		argumentCode->push_back(0x01);
		return true;
	} 
	else
	if (word == "bcc")
	{
		*argument = ArgumentType::Register;
		argumentCode->push_back(0x02);
		return true;
	}
	else
	if (word == "ccc")
	{
		*argument = ArgumentType::Register;
		argumentCode->push_back(0x03);
		return true;
	}
	else
	if (word == "up")
	{
		*argument = ArgumentType::Channel;
		argumentCode->push_back(0x00);
		return true;
	}
	else
	if (word == "upLeft")
	{
		*argument = ArgumentType::Channel;
		argumentCode->push_back(0x01);
		return true;
	}
	else
	if (word == "left")
	{
		*argument = ArgumentType::Channel;
		argumentCode->push_back(0x02);
		return true;
	}
	else
	if (word == "downLeft")
	{
		*argument = ArgumentType::Channel;
		argumentCode->push_back(0x03);
		return true;
	}
	else
	if (word == "down")
	{
		*argument = ArgumentType::Channel;
		argumentCode->push_back(0x04);
		return true;
	}
	else
	if (word == "downRight")
	{
		*argument = ArgumentType::Channel;
		argumentCode->push_back(0x05);
		return true;
	}
	else
	if (word == "right")
	{
		*argument = ArgumentType::Channel;
		argumentCode->push_back(0x06);
		return true;
	}
	else
	if (word == "upRight")
	{
		*argument = ArgumentType::Channel;
		argumentCode->push_back(0x07);
		return true;
	}
	else
	if (word[0] == '<' && word.back() == '>')
	{
		*argument = ArgumentType::FileName;
		return true;
	}
	else
	{
		*argument = ArgumentType::SymbolAddress;
		argumentCode->push_back(0); // a placeholder
		argumentCode->push_back(0);
		return true;
	}
}

std::string CommandLine::getLineAsString() const
{
	std::string lineAsString = this->_line[0];
	for (uint32_t i=1; i < this->_line.size(); i++)
	{
		lineAsString += " " + this->_line[i];
	}
	return lineAsString;
}

void CommandLine::printStandartErrorHeader()
{
	cerr << "Error in file '" << this->_fileName << "' in line " << this->_lineNumber << ':' << endl;
	cerr << this->getLineAsString() << endl;
}
