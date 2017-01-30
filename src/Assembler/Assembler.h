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

#include <vector>
#include <stack>
#include <string>
#include <memory>

#include "System/Uncopyable.h"
#include "Executable/ExecutableElement.h"

class Assembler : public Uncopyable
{
	
public: // methods
	Assembler();
	~Assembler() override;
	
	bool assembleText();
	
private: // methods
	
	bool assembleLine();
	
	void printErrorHeader();
	
public: // member access
	
	const std::string& getText() const;
	void setText(const std::string& text);
	
	std::vector<std::shared_ptr<ExecutableElement>> getElements() const;
	void resetElements();
	
	const std::string& getCurrentFilePath() const;
	void setBaseFilePath(std::string baseFilePath);
	
private: // members
	std::string _text;
	
	std::vector<std::string> _line;
	
	std::vector<std::shared_ptr<ExecutableElement>> _elements;
	
	std::stack<std::string> _filePathStack;
};
