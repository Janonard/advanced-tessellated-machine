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
#include "Assembler/Line.h"

namespace Assembler
{

	class File : public Uncopyable
	{
		
	public: // methods
		File();
		~File() override;
		
		bool assembleText();
		
	private: // methods
		
		bool assembleLine(std::string line, uint lineNumber);
		
		void printErrorHeader();
		
	public: // member access
		
		const std::string& getText() const;
		void setText(const std::string& text);
		
		std::vector<std::shared_ptr<ExecutableElement>> getElements() const;
		void resetElements();
		
		std::shared_ptr<SymbolMap> getSymbols() const;
		
		const std::string& getBaseFilePath() const;
		void setBaseFilePath(std::string baseFilePath);
		
	private: // members
		std::string _text;
		
		std::vector<std::shared_ptr<Assembler::Line>> _lines;
		
		std::vector<std::shared_ptr<ExecutableElement>> _elements;
		
		std::shared_ptr<SymbolMap> _symbols;
		
		std::string _baseFilePath;
	};

}
