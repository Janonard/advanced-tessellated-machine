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

#include <cstdint>
#include <vector>
#include <memory>
#include "Executable/ExecutableElement.h"
#include "CPU/CPU_Enums.h"
#include "System/Node.h"

/*
* The CPU node. It executes a program written in the rom, one command per tick.
*/
class CPU : public Node
{
	typedef bool (CPU::*ExecFuncPtr)();
	
public: // methods
	CPU();
	
	~CPU() override;
	
	bool pretick() override;
	
	bool tick() override;
	
	std::string getDebugString() const override;
	
	void setMemory(std::shared_ptr<ExecutableElement> newMemory);
	
	void setVerbosePointer(bool* newVerbosePtr);
	
	void setSlowPointer(bool* newSlowPtr);
	
private: // properties

	/*
	 * The registers.
	 */
	NODE_INT_TYPE _acc;
	NODE_INT_TYPE _accSwap;
	NODE_INT_TYPE _bcc;
	NODE_INT_TYPE _bccSwap;
	NODE_INT_TYPE _ccc;
	NODE_INT_TYPE _cccSwap;
	NODE_INT_TYPE _programPointer;
	
	std::vector<NODE_INT_TYPE> _branchStack;

	/*
	* The flags
	*/
	bool _operationSuccessfull;
	bool _greater;
	bool _lower;

	/*
	 * The Memory
	 */
	std::shared_ptr<ExecutableElement> _memory;
	
	bool* _verbose;
	bool* _slow;
	
	/*
	 * This vector contains pointers to all exec functions in the required order.
	 */
	std::vector<ExecFuncPtr> _execPointerVector;

private: // methods
	
	bool execHalt();
	
	bool execSplash();
	
	bool execMoveAddressRegister8Bit();
	
	bool execMoveAddressRegister16Bit();
	
	bool execMoveRegAddressRegister8Bit();
	
	bool execMoveRegAddressRegister16Bit();
	
	bool execMoveChannelRegister();
	
	bool execMoveValueRegister8Bit();
	
	bool execMoveValueRegister16Bit();
	
	bool execMoveRegisterRegister();

	bool execMoveRegisterAddress8Bit();
	
	bool execMoveRegisterAddress16Bit();
	
	bool execMoveRegisterRegAddress8Bit();
	
	bool execMoveRegisterRegAddress16Bit();
	
	bool execMoveRegisterChannel();
	
	bool execAddAddressRegister8bit();
	
	bool execAddAddressRegister16bit();
	
	bool execAddRegAddressRegister8bit();
	
	bool execAddRegAddressRegister16bit();
	
	bool execAddChannelRegister();
	
	bool execAddValueRegister8bit();
	
	bool execAddValueRegister16bit();
	
	bool execAddRegisterRegister();
	
	bool execNegateRegister();
	
	bool execShiftRegisterLeftBitwise();
	
	bool execShiftRegisterLeftBytewise();
	
	bool execShiftRegisterRightBitwise();
	
	bool execShiftRegisterRightBytewise();
	
	bool execSwapRegister();

	bool execCompareRegisterRegister();
	
	bool execAbsoluteJump();

	bool execAbsoluteJumpOperationSuccessfull();
	
	bool execAbsoluteJumpOperationFailed();

	bool execAbsoluteJumpGreater();

	bool execAbsoluteJumpLower();

	bool execAbsoluteJumpEqual();
	
	bool execAbsoluteJumpNonEqual();
	
	bool execBranch();
	
	bool execReturn();
	
	bool execDebugSetVerbose();
	
	bool execDebugSpeed();
	
	bool execDebugCrash();

private: // helper methods
	
	std::string connectStringWithInt(std::string param0, int16_t param1) const;
	
	std::string connectStringWithUInt(std::string param0, uint16_t param1) const;
	
	bool setRegister(const NODE_INT_TYPE reg, const NODE_INT_TYPE value);
	
	bool setVarToRegister(NODE_INT_TYPE* var, const NODE_INT_TYPE reg);
	
	bool addToRegister(const NODE_INT_TYPE reg, const NODE_INT_TYPE value);
	
	bool loadRegister(NODE_INT_TYPE* reg);
	
	bool loadChannel(NODE_INT_TYPE* channel);
	
	bool setTargetRegister(NODE_INT_TYPE value);
	
	bool addTargetRegister(NODE_INT_TYPE value);
	
	bool getBaseRegisterValue(NODE_INT_TYPE* value);
};
