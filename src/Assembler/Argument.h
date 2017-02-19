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
	/*
	 * An enum listing all possible types of arguments.
	 */
	enum class ArgumentType
	{
		NoArgument, Number8Bit, Number16Bit, Register, Symbol, SymbolNumber, Address, AddressRegister, Channel, Filename
	};
	
	/*
	 * This is a container class for an argument, that contains
	 * The type, the code and the used symbol.
	 */
	class Argument
	{
	public: // methods
		Argument();
		
		/*
		 * If our type is a symbol or a symbol number, this function tries
		 * to lookup our symbol name in the map. If it finds it, it will set
		 * our code to the contained value, if not, it throw an out_of_range
		 * exception.
		 */
		void solveSymbols(std::shared_ptr<SymbolMap> symbols) throw(std::out_of_range);
		
		/*
		 * Getter for _type
		 */
		ArgumentType getType() const;
		
		/*
		 * Setter for _type
		 */
		void setType(ArgumentType type);
		
		/*
		 * Returns a constant reference to _code for read-only access.
		 */
		const Memory& getCode() const;
		
		/*
		 * Returns a pointer to _code for read-write access.
		 */
		Memory* getCodePointer();
		
		/*
		 * Setter for _code
		 */
		void setCode(const Memory& memory);
		
		/*
		 * Returns a constant reference to _symbolName for read-only access.
		 */
		const std::string& getSymbolName() const;
		
		/*
		 * Setter for _symbolName
		 */
		void setSymbolName(const std::string& symbolName);
		
	private: // properties
		
		/*
		 * Our argument type
		 */
		ArgumentType _type;
		
		/*
		 * Our code which will end up in the binary representing us.
		 */
		Memory _code;
		
		/*
		 * The name of the symbol we are using.
		 */
		std::string _symbolName;
		
	};
	
}
