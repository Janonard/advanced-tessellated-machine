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
#pragma once
#include "CPU/CPU_Enums.h"

#define MNE_SPLASH			"splash"
#define MNE_HALT			"halt"
#define MNE_MOVE			"move"
#define MNE_MOVE8			"move8"
#define MNE_MOVE16			"move16"
#define MNE_ADD				"add"
#define MNE_ADD8			"add8"
#define MNE_ADD16			"add16"
#define MNE_NEGATE			"negate"
#define MNE_BIT_SHIFT_L		"bit_shift_l"
#define MNE_BYTE_SHIFT_L	"byte_shift_l"
#define MNE_BIT_SHIFT_R		"bit_shift_r"
#define MNE_BYTE_SHIFT_R	"byte_shift_r"
#define MNE_SWAP			"swap"
#define MNE_COMPARE			"compare"
#define MNE_A_JUMP			"jump"
#define MNE_A_JUMP_OPS		"jump_ops"
#define MNE_A_JUMP_OPF		"jump_opf"
#define MNE_A_JUMP_GREATER	"jump_greater"
#define MNE_A_JUMP_LOWER	"jump_lower"
#define	MNE_A_JUMP_EQUAL	"jump_equal"
#define MNE_A_JUMP_NON_EQUAL	"jump_nequal"
#define MNE_BRANCH			"branch"
#define MNE_RETURN			"return"
#define MNE_META_VAR		"#var"
#define MNE_META_DEFINE		"#define"
#define MNE_META_POSITION	"#position"
#define MNE_META_SPACE		"#space"
#define MNE_META_INCLUDE	"#include"
#define MNE_META_BINARY		"#binary"
#define MNE_DEBUG_SET_VERBOSE "dVerbose"
#define MNE_DEBUG_SLOW		"dSlow"
#define MNE_DEBUG_CRASH		"dCrash"
