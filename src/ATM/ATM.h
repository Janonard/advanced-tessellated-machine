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
#include "CPU/CPU.h"
#include "GraphicsAdapter/GraphicsAdapter.h"
#include "KeyboardAdapter/KeyboardAdapter.h"
#include "TimeAdapter/TimeAdapter.h"
#include "System/Motherboard.h"
#include "Executable/Executable.h"

#include <memory>

std::unique_ptr<Motherboard> motherboard;

std::array<std::unique_ptr<CPU>, 4> cpus;

std::unique_ptr<GraphicsAdapter> graphicsAdapter;

std::unique_ptr<KeyboardAdapter> keyboardAdapter;

std::unique_ptr<TimeAdapter> timeAdapter;

// global functions

void printUsage();

bool loadExecutable(std::string pathToExe, std::shared_ptr<Executable>& exec);

bool runExecutable(std::string filePath);

bool buildSystem();

bool assignElements(const std::shared_ptr<Executable>& exec);

bool buildExecutable(std::string filePath);
