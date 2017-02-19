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

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <fstream>

#include "System/Uncopyable.h"
#include "Executable/LoadingException.h"

#include "Executable/ExecutableElement.h"

const std::string executableNodeCommand = "NODE";

/*
 * Class for loading, assembling, saving and handling ATM executables.
 */
class Executable : public Uncopyable
{
public: // methods
	
	/*
	 * Constructor. It loads the content of the file from the file path
	 * and checks whether it is an executable or an assembler source code.
	 * If it is an executable, it will simply load it, if it is a source
	 * code, it will assemble it. The outcome of these options is the same.
	 * 
	 * When anything goes wrong, it will throw a loading exception containing
	 * a short description of the problem.
	 */
	explicit Executable(const std::string& filePath) throw(LoadingException);
	
	/*
	 * Tries to find the element with the given coordinates. If it was
	 * successfull, it will return a shared pointer to it and set successfull to true.
	 * If not, the pointer will point to nullptr and successfull will be false.
	 */
	std::shared_ptr<ExecutableElement> getElement(int x, int y, bool* successfull);
	
	/*
	 * Saves the our elements to the file {_filePath}.atmx
	 * Throws a LoadingException if it wasn't successfull.
	 */
	void saveExecutable() const throw(LoadingException);
	
private: // methods
	
	/*
	 * Opens the file, loads it's content and returns it.
	 * If the file could not be opened, it will throw a loading exception
	 * containing a short description of the problem.
	 */
	std::string readFile(const std::string& filePath) const throw(LoadingException);
	
	/*
	 * Checks whether the file text is a proper executable file.
	 * 
	 * This done by looking up the header, which should be "ATMXATMO", and checking
	 * whether the first coordinates are valid.
	 */
	bool isBinary(const std::string& fileText) const;
	
	/*
	 * Loads all elements in the file text and adds them to _elements.
	 * If anything goes wrong, it will throw a loading exception containing a short
	 * description of the problem.
	 */
	void loadExecutable(const std::string& fileText) throw(LoadingException);
	
	/*
	 * Assembles the given fileText and saves the result in _elements.
	 * If it wasn't successfull, it will write an error log to cerr and throw
	 * a LodingException.
	 */
	void assembleFile(const std::string& fileText) throw(LoadingException);
	
	/*
	 * Assembles a single line. This also requires it's line number and a pointer to
	 * the Resources object. If new source code were included, it will be written to
	 * outIncludedLine. Returns true if successfull, false if not. If it isn't
	 * successfull, it will also write an error log to cerr.
	 */
	bool assembleLine(const std::string& line, uint lineNumber, std::shared_ptr<Assembler::Resources> resources, std::string* outIncludedLine);
	
	/*
	 * Adds a new element to _elements, with the given code containing the coordinates.
	 * Returns true if successfull, false if not. If it isn't successfull, it will
	 * also write an error log to cerr.
	 */
	bool addNewExecElement(uint8_t code);
	
	/*
	 * Executes an inclusion. Returns true if successfull, false if not. If it isn't
	 * successfull, it will also write an error log to cerr.
	 */
	bool execInclude(std::shared_ptr<Assembler::Line> line, std::string* outIncludedCode);
	
private: // properties
	
	/*
	 * A vector with all elements.
	 */
	std::vector<std::shared_ptr<ExecutableElement>> _elements;
	
	/*
	 * The path to the file we are working with.
	 */
	std::string _filePath;
	
};
