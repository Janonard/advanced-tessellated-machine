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
#include "CPU/CPU_Enums.h"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace Assembler;

#define COMMAND_COMBI(ARGUMENT0, ARGUMENT1, CODE) \
	if (this->_argument0.getType() == ARGUMENT0 && this->_argument1.getType() == ARGUMENT1) \
	{ this->_command.getCodePointer()->push_back(uint8_t(CODE)); this->_memorySize++; return true; }

#define SINGLE_COMBI_COMMAND(COMMAND, ARGUMENT0, ARGUMENT1, CODE) \
	if (this->_command.getType() == COMMAND && this->_argument0.getType() == ARGUMENT0 && this->_argument1.getType() == ARGUMENT1) \
	{ this->_command.getCodePointer()->push_back(uint8_t(CODE)); this->_memorySize++; return true; }
	
#define COMBI_ERROR() \
	{ this->printErrorHeader(); cerr << "Unsupported command/argument combination!" << endl; return false; }

bool Assembler::Line::identifyCombination()
{
	if (this->_command.getType() != CommandType::None)
	{
		if (this->_command.getType() == CommandType::Move)
		{
				COMMAND_COMBI(ArgumentType::Number8Bit,			ArgumentType::Register,			CommandCodes::Move_Value_Register_8_bit)
			else
				COMMAND_COMBI(ArgumentType::Number16Bit,		ArgumentType::Register,			CommandCodes::Move_Value_Register_16_bit)
			else
				COMMAND_COMBI(ArgumentType::SymbolNumber,		ArgumentType::Register,			CommandCodes::Move_Value_Register_16_bit)
			else
				COMMAND_COMBI(ArgumentType::Channel,			ArgumentType::Register,			CommandCodes::Move_Channel_Register)
			else
				COMMAND_COMBI(ArgumentType::Register,			ArgumentType::Channel,			CommandCodes::Move_Channel_Register)
			else
				COMMAND_COMBI(ArgumentType::Register,			ArgumentType::Register,			CommandCodes::Move_Register_Register)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::Move8)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::Register,			CommandCodes::Move_Address_Register_8_bit)
			else
				COMMAND_COMBI(ArgumentType::Register,			ArgumentType::Address,			CommandCodes::Move_Register_Address_8_bit)
			else
				COMMAND_COMBI(ArgumentType::Symbol,				ArgumentType::Register,			CommandCodes::Move_Address_Register_8_bit)
			else
				COMMAND_COMBI(ArgumentType::Register,			ArgumentType::Symbol,			CommandCodes::Move_Register_Address_8_bit)
			else
				COMMAND_COMBI(ArgumentType::AddressRegister,	ArgumentType::Register,			CommandCodes::Move_RegAddress_Register_8_bit)
			else
				COMMAND_COMBI(ArgumentType::Register,			ArgumentType::AddressRegister,	CommandCodes::Move_Register_RegAddress_8_bit)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::Move16)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::Register,			CommandCodes::Move_Address_Register_16_bit)
			else
				COMMAND_COMBI(ArgumentType::Register,			ArgumentType::Address,			CommandCodes::Move_Register_Address_16_bit)
			else
				COMMAND_COMBI(ArgumentType::Symbol,				ArgumentType::Register,			CommandCodes::Move_Address_Register_16_bit)
			else
				COMMAND_COMBI(ArgumentType::Register,			ArgumentType::Symbol,			CommandCodes::Move_Register_Address_16_bit)
			else
				COMMAND_COMBI(ArgumentType::AddressRegister,	ArgumentType::Register,			CommandCodes::Move_RegAddress_Register_16_bit)
			else
				COMMAND_COMBI(ArgumentType::Register,			ArgumentType::AddressRegister,	CommandCodes::Move_Register_RegAddress_16_bit)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::Add)
		{
				COMMAND_COMBI(ArgumentType::Number8Bit,			ArgumentType::Register,			CommandCodes::Add_Value_Register_8_bit)
			else
				COMMAND_COMBI(ArgumentType::Number16Bit,		ArgumentType::Register,			CommandCodes::Add_Value_Register_16_bit)
			else
				COMMAND_COMBI(ArgumentType::SymbolNumber,		ArgumentType::Register,			CommandCodes::Add_Value_Register_16_bit)
			else
				COMMAND_COMBI(ArgumentType::Channel,			ArgumentType::Register,			CommandCodes::Add_Channel_Register)
			else
				COMMAND_COMBI(ArgumentType::Register,			ArgumentType::Register,			CommandCodes::Add_Register_Register)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::Add8)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::Register,			CommandCodes::Add_Address_Register_8_bit)
			else
				COMMAND_COMBI(ArgumentType::Symbol,			ArgumentType::Register,			CommandCodes::Add_Address_Register_8_bit)
			else
				COMMAND_COMBI(ArgumentType::AddressRegister,	ArgumentType::Register,			CommandCodes::Add_RegAddress_Register_8_bit)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::Add16)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::Register,			CommandCodes::Add_Address_Register_16_bit)
			else
				COMMAND_COMBI(ArgumentType::Symbol,			ArgumentType::Register,			CommandCodes::Add_Address_Register_16_bit)
			else
				COMMAND_COMBI(ArgumentType::AddressRegister,	ArgumentType::Register,			CommandCodes::Add_RegAddress_Register_16_bit)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::Jump)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::NoArgument,		CommandCodes::Absolute_Jump)
			else
				COMMAND_COMBI(ArgumentType::Symbol,				ArgumentType::NoArgument,		CommandCodes::Absolute_Jump)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::JumpOPS)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_OperationSuccessfull)
			else
				COMMAND_COMBI(ArgumentType::Symbol,				ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_OperationSuccessfull)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::JumpOPF)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_OperationFailed)
			else
				COMMAND_COMBI(ArgumentType::Symbol,				ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_OperationFailed)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::JumpGreater)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_Greater)
			else
				COMMAND_COMBI(ArgumentType::Symbol,				ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_Greater)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::JumpLower)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_Lower)
			else
				COMMAND_COMBI(ArgumentType::Symbol,				ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_Lower)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::JumpEqual)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_Equal)
			else
				COMMAND_COMBI(ArgumentType::Symbol,				ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_Equal)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::JumpNequal)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_Non_Equal)
			else
				COMMAND_COMBI(ArgumentType::Symbol,				ArgumentType::NoArgument,		CommandCodes::Absolute_Jump_Non_Equal)
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::Branch)
		{
				COMMAND_COMBI(ArgumentType::Address,			ArgumentType::NoArgument,		CommandCodes::Branch)
			else
				COMMAND_COMBI(ArgumentType::Symbol,				ArgumentType::NoArgument,		CommandCodes::Branch)
			else
				COMBI_ERROR()
		}
		else
			SINGLE_COMBI_COMMAND(CommandType::Halt,			ArgumentType::NoArgument,	ArgumentType::NoArgument,	CommandCodes::Halt)
		else
			//SINGLE_COMBI_COMMAND(CommandType::Splash,		ArgumentType::NoArgument,	ArgumentType::NoArgument,	CommandCodes::Splash)
			if (this->_command.getType() == CommandType::Splash && this->_argument0.getType() == ArgumentType::NoArgument && this->_argument1.getType() == ArgumentType::NoArgument)
			{
				//cout << this->_memoryLocation<< endl;
				 this->_command.getCodePointer()->push_back(uint8_t(CommandCodes::Splash));
				 return true; 
			}
		else
			SINGLE_COMBI_COMMAND(CommandType::Negate,		ArgumentType::Register,		ArgumentType::NoArgument,	CommandCodes::Negate_Register)
		else
			SINGLE_COMBI_COMMAND(CommandType::BitShiftL,	ArgumentType::Register,		ArgumentType::NoArgument,	CommandCodes::Shift_Register_Left_Bitwise)
		else
			SINGLE_COMBI_COMMAND(CommandType::ByteShiftL,	ArgumentType::Register,		ArgumentType::NoArgument,	CommandCodes::Shift_Register_Left_Bytewise)
		else
			SINGLE_COMBI_COMMAND(CommandType::BitShiftR,	ArgumentType::Register,		ArgumentType::NoArgument,	CommandCodes::Shift_Register_Right_Bitwise)
		else
			SINGLE_COMBI_COMMAND(CommandType::ByteShiftR,	ArgumentType::Register,		ArgumentType::NoArgument,	CommandCodes::Shift_Register_Right_Bytewise)
		else
			SINGLE_COMBI_COMMAND(CommandType::Swap,			ArgumentType::Register,		ArgumentType::NoArgument,	CommandCodes::Swap_Register)
		else
			SINGLE_COMBI_COMMAND(CommandType::Compare,		ArgumentType::Register,		ArgumentType::Register,		CommandCodes::Compare_Register_Register)
		else
			SINGLE_COMBI_COMMAND(CommandType::Return,		ArgumentType::NoArgument,	ArgumentType::NoArgument,	CommandCodes::Return)
		else
			SINGLE_COMBI_COMMAND(CommandType::DVerbose,		ArgumentType::Number8Bit,	ArgumentType::NoArgument,	CommandCodes::Debug_Set_Verbose)
		else
			SINGLE_COMBI_COMMAND(CommandType::DSlow,		ArgumentType::Number8Bit,	ArgumentType::NoArgument,	CommandCodes::Debug_Speed)
		else
			SINGLE_COMBI_COMMAND(CommandType::DCrash,		ArgumentType::NoArgument,	ArgumentType::NoArgument,	CommandCodes::Debug_Crash)
		else
		if (this->_command.getType() == CommandType::MVar)
		{
			if (this->_argument0.getType() == ArgumentType::Number8Bit and this->_argument1.getType() == ArgumentType::NoArgument)
			{
				return true;
			}
			else
			if (this->_argument0.getType() == ArgumentType::Number16Bit and this->_argument1.getType() == ArgumentType::NoArgument)
			{
				return true;
			}
			else
				COMBI_ERROR()
		}
		else
		if (this->_command.getType() == CommandType::MDefine && this->_argument0.getType() == ArgumentType::Symbol && this->_argument1.getType() == ArgumentType::Address)
		{
			return this->execDefine();
		}
		else
		if (this->_command.getType() == CommandType::MPosition && this->_argument0.getType() == ArgumentType::Address && this->_argument1.getType() == ArgumentType::NoArgument)
		{
			return this->execPosition();
		}
		else
		if (this->_command.getType() == CommandType::MSpace && this->_argument0.getType() == ArgumentType::Address && this->_argument1.getType() == ArgumentType::NoArgument)
		{
			return this->execSpace();
		}
		else
		if (this->_command.getType() == CommandType::NODE && this->_argument0.getType() == ArgumentType::Number8Bit && this->_argument1.getType() == ArgumentType::NoArgument)
		{
			return true; // the executable will care about this!
		}
		else
			COMBI_ERROR()
	}
	return true;
}
