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
#include <iostream>
#include <sstream>
#include "CPU/CPU.h"

CPU::CPU() : 
	_acc(0),
	_accSwap(0),
	_bcc(0),
	_bccSwap(0),
	_ccc(0),
	_cccSwap(0),
	_programPointer(0),
	_branchStack(),
	_operationSuccessfull(false),
	_greater(false),
	_lower(false),
	_memory(nullptr),
	_verbose(nullptr),
	_execPointerVector()
{
	this->_execPointerVector.push_back(&CPU::execHalt);
	
	this->_execPointerVector.push_back(&CPU::execSplash);
	
	this->_execPointerVector.push_back(&CPU::execMoveAddressRegister8Bit);
	
	this->_execPointerVector.push_back(&CPU::execMoveAddressRegister16Bit);
	
	this->_execPointerVector.push_back(&CPU::execMoveRegAddressRegister8Bit);
	
	this->_execPointerVector.push_back(&CPU::execMoveRegAddressRegister16Bit);
	
	this->_execPointerVector.push_back(&CPU::execMoveChannelRegister);
	
	this->_execPointerVector.push_back(&CPU::execMoveValueRegister8Bit);
	
	this->_execPointerVector.push_back(&CPU::execMoveValueRegister16Bit);
	
	this->_execPointerVector.push_back(&CPU::execMoveRegisterRegister);

	this->_execPointerVector.push_back(&CPU::execMoveRegisterAddress8Bit);
	
	this->_execPointerVector.push_back(&CPU::execMoveRegisterAddress16Bit);
	
	this->_execPointerVector.push_back(&CPU::execMoveRegisterRegAddress8Bit);
	
	this->_execPointerVector.push_back(&CPU::execMoveRegisterRegAddress16Bit);
	
	this->_execPointerVector.push_back(&CPU::execMoveRegisterChannel);
	
	this->_execPointerVector.push_back(&CPU::execAddAddressRegister8bit);
	
	this->_execPointerVector.push_back(&CPU::execAddAddressRegister16bit);
	
	this->_execPointerVector.push_back(&CPU::execAddRegAddressRegister8bit);
	
	this->_execPointerVector.push_back(&CPU::execAddRegAddressRegister16bit);
	
	this->_execPointerVector.push_back(&CPU::execAddChannelRegister);
	
	this->_execPointerVector.push_back(&CPU::execAddValueRegister8bit);
	
	this->_execPointerVector.push_back(&CPU::execAddValueRegister16bit);
	
	this->_execPointerVector.push_back(&CPU::execAddRegisterRegister);
	
	this->_execPointerVector.push_back(&CPU::execNegateRegister);
	
	this->_execPointerVector.push_back(&CPU::execShiftRegisterLeftBitwise);
	
	this->_execPointerVector.push_back(&CPU::execShiftRegisterLeftBytewise);
	
	this->_execPointerVector.push_back(&CPU::execShiftRegisterRightBitwise);
	
	this->_execPointerVector.push_back(&CPU::execShiftRegisterRightBytewise);
	
	this->_execPointerVector.push_back(&CPU::execSwapRegister);

	this->_execPointerVector.push_back(&CPU::execCompareRegisterRegister);
	
	this->_execPointerVector.push_back(&CPU::execAbsoluteJump);

	this->_execPointerVector.push_back(&CPU::execAbsoluteJumpOperationSuccessfull);
	
	this->_execPointerVector.push_back(&CPU::execAbsoluteJumpOperationFailed);

	this->_execPointerVector.push_back(&CPU::execAbsoluteJumpGreater);

	this->_execPointerVector.push_back(&CPU::execAbsoluteJumpLower);

	this->_execPointerVector.push_back(&CPU::execAbsoluteJumpEqual);
	
	this->_execPointerVector.push_back(&CPU::execAbsoluteJumpNonEqual);
	
	this->_execPointerVector.push_back(&CPU::execBranch);
	
	this->_execPointerVector.push_back(&CPU::execReturn);
	
	this->_execPointerVector.push_back(&CPU::execDebugSetVerbose);
	
	this->_execPointerVector.push_back(&CPU::execDebugSpeed);
	
	this->_execPointerVector.push_back(&CPU::execDebugCrash);
}

CPU::~CPU()
{
	Node::~Node();
}

bool CPU::pretick()
{
	if (this->_isRunning)
	{
		uint8_t currentByte = _memory->get8BitValue(this->_programPointer);
		if (currentByte == uint8_t(Command::Move_Register_Channel))
		{
			this->_programPointer++;
			this->_hasPreticked = true;
			return this->execMoveRegisterChannel();
		}
		else
		{
			this->_hasPreticked = false;
			return true;
		}
	}
	else
		return true;
}

bool CPU::tick()
{
	if (!Node::tick())
		return false;
	
	if ((this->_hasPreticked == false) && this->_isRunning)
	{
		uint8_t currentByte = _memory->get8BitValue(this->_programPointer);
		this->_programPointer++;
		if (currentByte < this->_execPointerVector.size())
		{
			return (this->*(this->_execPointerVector[currentByte]))();
		}
		else
			return false;
	}
	else
		return true;
}

std::string CPU::getDebugString() const
{
	if (this->_isRunning)
	{
		std::string debugString = this->connectStringWithUInt("acc=",  this->_acc);
		debugString += "|";
		debugString = this->connectStringWithInt(debugString, (int16_t) this->_acc);
		debugString += " accSwap=";
		debugString = this->connectStringWithUInt(debugString, this->_accSwap);
		debugString += "|";
		debugString = this->connectStringWithInt(debugString, (int16_t) this->_accSwap);
		debugString += " bcc=";
		debugString = this->connectStringWithUInt(debugString, this->_bcc);
		debugString += "|";
		debugString = this->connectStringWithInt(debugString, (int16_t) this->_bcc);
		debugString += " bccSwap=";
		debugString = this->connectStringWithUInt(debugString, this->_bccSwap);
		debugString += "|";
		debugString = this->connectStringWithInt(debugString, (int16_t) this->_bccSwap);
		debugString += " ccc=";
		debugString = this->connectStringWithUInt(debugString, this->_ccc);
		debugString += "|";
		debugString = this->connectStringWithInt(debugString, (int16_t) this->_ccc);
		debugString += " cccSwap=";
		debugString = this->connectStringWithUInt(debugString, this->_cccSwap);
		debugString += "|";
		debugString = this->connectStringWithInt(debugString, (int16_t) this->_cccSwap);
		debugString += " branchStack=";
		if (this->_branchStack.size() > 0)
		{
			for (NODE_INT_TYPE element : this->_branchStack)
			{
				debugString = this->connectStringWithInt(debugString, element) + " ";
			}
		}
		else
		{
			debugString += "- ";
		}
		debugString += "pp=";
		debugString = this->connectStringWithUInt(debugString, this->_programPointer);
		debugString += " flags=";
		if (this->_operationSuccessfull)
			debugString += "1";
		else
			debugString += "0";
		if (this->_greater)
			debugString += "1";
		else
			debugString += "0";
		if (this->_lower)
			debugString += "1";
		else
			debugString += "0";
		debugString += " mem=";
		debugString = this->connectStringWithUInt(debugString, this->_memory->get8BitValue(this->_programPointer));
		debugString += " ";
		debugString = this->connectStringWithUInt(debugString, this->_memory->get8BitValue(this->_programPointer+1));
		debugString += " ";
		debugString = this->connectStringWithUInt(debugString, this->_memory->get8BitValue(this->_programPointer+2));
		debugString += " ";
		debugString = this->connectStringWithUInt(debugString, this->_memory->get8BitValue(this->_programPointer+3));
		debugString += " ";
		debugString = this->connectStringWithUInt(debugString, this->_memory->get8BitValue(this->_programPointer+4));
		debugString += " ";
		debugString = this->connectStringWithUInt(debugString, this->_memory->get8BitValue(this->_programPointer+5));
		return debugString;
	}
	else
		return "";
}

void CPU::setMemory(std::shared_ptr<ExecutableElement> newMemory)
{
	this->_memory = newMemory;
}

void CPU::setVerbosePointer(bool* newVerbosePtr)
{
	if (newVerbosePtr != nullptr)
		this->_verbose = newVerbosePtr;
}

void CPU::setSlowPointer(bool* newSlowPtr)
{
	if (newSlowPtr != nullptr)
		this->_slow = newSlowPtr;
}
