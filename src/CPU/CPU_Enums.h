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

// CPU Enums!

enum Command
{
Halt								=0x00,
Splash								=0x01,
Move_Address_Register_8_bit			=0x02,
Move_Address_Register_16_bit		=0x03,
Move_RegAddress_Register_8_bit		=0x04,
Move_RegAddress_Register_16_bit		=0x05,
Move_Channel_Register				=0x06,
Move_Value_Register_8_bit			=0x07,
Move_Value_Register_16_bit			=0x08,
Move_Register_Register				=0x09,
Move_Register_Address_8_bit			=0x0A,
Move_Register_Address_16_bit		=0x0B,
Move_Register_RegAddress_8_bit		=0x0C,
Move_Register_RegAddress_16_bit		=0x0D,
Move_Register_Channel				=0x0E,
Add_Address_Register_8_bit			=0x0F,
Add_Address_Register_16_bit			=0x10,
Add_RegAddress_Register_8_bit		=0x11,
Add_RegAddress_Register_16_bit		=0x12,
Add_Channel_Register				=0x13,
Add_Value_Register_8_bit			=0x14,
Add_Value_Register_16_bit			=0x15,
Add_Register_Register				=0x16,
Negate_Register						=0x17,
Shift_Register_Left_Bitwise			=0x18,
Shift_Register_Left_Bytewise		=0x19,
Shift_Register_Right_Bitwise		=0x1A,
Shift_Register_Right_Bytewise		=0x1B,
Swap_Register						=0x1C,
Compare_Register_Register			=0x1D,
Absolute_Jump						=0x1E,
Absolute_Jump_OperationSuccessfull	=0x1F,
Absolute_Jump_OperationFailed		=0x20,
Absolute_Jump_Greater				=0x21,
Absolute_Jump_Lower					=0x22,
Absolute_Jump_Equal					=0x23,
Absolute_Jump_Non_Equal				=0x24,
Branch								=0x25,
Return								=0x26,
Debug_Set_Verbose					=0x27,
Debug_Speed							=0x28,
Debug_Crash							=0x29
};

enum Register
{
NUL=0,ACC=1,BCC=2,CCC=3
};
