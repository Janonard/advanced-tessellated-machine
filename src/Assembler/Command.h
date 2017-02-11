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

namespace Assembler
{
	
	enum class CommandType
	{
		Halt=0, Splash=1, Move=2, Move8=3, Move16=4, Add=5, Add8=6, Add16=7, Negate=8, BitShiftL=9, ByteShiftL=10, BitShiftR=11, ByteShiftR=12, Swap=13, Compare=14, Jump=15, JumpOPS=16, JumpOPF=17, JumpGreater=18, JumpLower=19, JumpEqual=20, JumpNequal=21, Branch=22, Return=23, DVerbose=24, DSlow=25, DCrash=26, MDefine=27, MVar=28, MPosition=29, MSpace=30, MInclude=31, MBinary=32, NODE=33, None=34
	};
	
	class Command
	{
	public: // methods
		Command();
		
		CommandType getType() const;
		void setType(CommandType type);
		
		std::shared_ptr<Memory> getCode() const;
		void setCode(std::shared_ptr<Memory> code);
		
	private: // properties
		CommandType _type;
		
		std::shared_ptr<Memory> _code;
	};
	
}
