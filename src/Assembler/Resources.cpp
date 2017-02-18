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
 * but WITHOUT ANY WARRANTY), without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Assembler/Resources.h"

Assembler::Resources::Resources() :
	cNumberIdentifier('#'),
	cAddressIdentifier('$'),
	cOpenBracket('<'),
	cClosedBracket('>'),
	cSpace(' '),
	cTab('\t'),
	cSymbolIdentifier(':'),
	cEightBitNumberLength(3),
	cSixteenBitNumberLength(5),
	cComment(';'),
	
	vRegisters({{"null","acc","bcc","ccc"}}),
	vAddressRegisters({{"$acc","$bcc","$ccc"}}),
	vChannels({{"up","upLeft","left","downLeft","down","downRight","right","upRight"}}),
	vCommands({{"halt", "splash", "move", "move8", "move16", "add", "add8", "add16", "negate", "bit_shift_l", "byte_shift_l", "bit_shift_r", "byte_shift_r", "swap", "compare", "jump", "jump_ops", "jump_opf", "jump_greater", "jump_lower", "jump_equal", "jump_nequal", "branch", "return", "dVerbose", "dSlow", "dCrash", "#define", "#var", "#position", "#space", "#include", "#binary", "NODE"}})
{
	
}
