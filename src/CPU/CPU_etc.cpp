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
#include "CPU/CPU.h"

bool CPU::execHalt()
{
	this->_isRunning = false;
	return true;
}

bool CPU::execSplash()
{
	return true;
}

bool CPU::execNegateRegister()
{
	NODE_INT_TYPE reg = 0;
	if (! this->loadRegister(&reg))
		return false;
	
	switch (reg)
	{
		case 0:
			return true;
		case 1:
			this->_acc = (NODE_INT_TYPE) (((int16_t) this->_acc) * -1);
			return true;
		case 2:
			this->_bcc = (NODE_INT_TYPE) (((int16_t) this->_bcc) * -1);
			return true;
		case 3:
			this->_ccc = (NODE_INT_TYPE) (((int16_t) this->_ccc) * -1);
			return true;
		default:
			return false;
	}
}

bool CPU::execShiftRegisterLeftBitwise()
{
	NODE_INT_TYPE reg = 0;
	if (! this->loadRegister(&reg))
		return false;
	
	switch (reg)
	{
		case 0:
			return true;
		case 1:
			this->_acc *= 0x2;
			return true;
		case 2:
			this->_bcc *= 0x2;
			return true;
		case 3:
			this->_ccc *= 0x2;
			return true;
		default:
			return false;
	}
}

bool CPU::execShiftRegisterLeftBytewise()
{
	NODE_INT_TYPE reg = 0;
	if (! this->loadRegister(&reg))
		return false;
	
	switch (reg)
	{
		case 0:
			return true;
		case 1:
			this->_acc *= 0x100;
			return true;
		case 2:
			this->_bcc *= 0x100;
			return true;
		case 3:
			this->_ccc *= 0x100;
			return true;
		default:
			return false;
	}
}

bool CPU::execShiftRegisterRightBitwise()
{
	NODE_INT_TYPE reg = 0;
	if (! this->loadRegister(&reg))
		return false;
	
	switch (reg)
	{
		case 0:
			return true;
		case 1:
			this->_acc /= 0x2;
			return true;
		case 2:
			this->_bcc /= 0x2;
			return true;
		case 3:
			this->_ccc /= 0x2;
			return true;
		default:
			return false;
	}
}

bool CPU::execShiftRegisterRightBytewise()
{
	NODE_INT_TYPE reg = 0;
	if (! this->loadRegister(&reg))
		return false;
	
	switch (reg)
	{
		case 0:
			return true;
		case 1:
			this->_acc /= 0x100;
			return true;
		case 2:
			this->_bcc /= 0x100;
			return true;
		case 3:
			this->_ccc /= 0x100;
			return true;
		default:
			return false;
	}
}

bool CPU::execSwapRegister()
{
	NODE_INT_TYPE reg = 0;
	if (! this->loadRegister(&reg))
		return false;
	
	NODE_INT_TYPE third;
	switch ((Direction) reg)
	{
		case ACC:
			third = this->_acc;
			this->_acc = this->_accSwap;
			this->_accSwap = third;
			return true;
		case BCC:
			third = this->_bcc;
			this->_bcc = this->_bccSwap;
			this->_bccSwap = third;
			return true;
		case CCC:
			third = this->_ccc;
			this->_ccc = this->_cccSwap;
			this->_cccSwap = third;
			return true;
		default:
			return false;
	}
}

bool CPU::execCompareRegisterRegister()
{
	NODE_INT_TYPE leftValue = 0;
	if (! this->getBaseRegisterValue(&leftValue))
		return false;
	
	NODE_INT_TYPE rightValue = 0;
	if (! this->getBaseRegisterValue(&rightValue))
		return false;
	
	if (leftValue > rightValue)
	{
		this->_greater = true;
		this->_lower = false;
	}
	else if (leftValue < rightValue)
	{
		this->_greater = false;
		this->_lower = true;
	}
	else
	{
		this->_greater = false;
		this->_lower = false;
	}
	
	return true;
}

bool CPU::execDebugSetVerbose()
{
	if (this->_verbose != nullptr)
	{
		uint8_t value = this->_memory->get8BitValue(this->_programPointer);
		this->_programPointer++;
		
		if (value == (uint8_t)true)
		{
			*this->_verbose = true;
		}
		else
		{
			*this->_verbose = false;
		}
	}
	return true;
}

bool CPU::execDebugSpeed()
{
	if (this->_slow != nullptr)
	{
		uint8_t value = this->_memory->get8BitValue(this->_programPointer);
		this->_programPointer++;
		
		if (value == (uint8_t)true)
		{
			*this->_slow = true;
		}
		else
		{
			*this->_slow = false;
		}
	}
	return true;
}

bool CPU::execDebugCrash()
{
	return false;
}
