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

#include <memory>

#include "Executable/Definitions.h"
#include "Assembler/Definitions.h"

namespace Assembler
{
	
	enum class ArgumentType
	{
		NoArgument, Number8Bit, Number16Bit, Register, Symbol, SymbolNumber, Address, AddressRegister, Channel, Filename
	};
	
	class Argument
	{
	public: // methods
		Argument();
		
		ArgumentType getType() const;
		void setType(ArgumentType type);
		
		const Memory& getCode() const;
		Memory* getCodePointer();
		void setCode(const Memory& memory);
		
		const std::string& getSymbolName() const;
		void setSymbolName(const std::string& symbolName);
		
		void solveSymbols(std::shared_ptr<SymbolMap> symbols) throw(std::out_of_range);
		
	private: // properties
		
		ArgumentType _type;
		
		Memory _code;
		
		std::string _symbolName;
		
	};
	
}
