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
#include "Executable/Executable.h"

#include <ios>
#include <string>
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include "System/Motherboard.h"

using namespace std;
using namespace Assembler;

Executable::Executable(const string& filePath) throw(LoadingException) :
	_elements(),
	_filePath(filePath)
{
	string fileText = this->readFile(filePath);
	
	if (this->isBinary(fileText))
	{
		this->loadExecutable(fileText);
	}
	else
	{
		this->assembleFile(fileText);
	}
}

shared_ptr<ExecutableElement> Executable::getElement(int x, int y, bool* successfull)
{
	*successfull = false;
	for (shared_ptr<ExecutableElement>& loopedObject : this->_elements)
	{
		if (loopedObject->getX() == x && loopedObject->getY() == y)
		{
			*successfull = true;
			return shared_ptr<ExecutableElement>(loopedObject);
		}
	}
	return shared_ptr<ExecutableElement>(nullptr);
}

string Executable::readFile(const string& filePath) const throw(LoadingException)
{
	fstream file = fstream(filePath, ios_base::in | ios_base::binary);
	if (file.is_open())
	{
		std::string fileText = std::string();
		char nextChar;
		while (file.get(nextChar)) // loop getting single characters
		{
			fileText.push_back(nextChar);
		}
		file.close();
		
		return fileText;
	}
	else
		throw(LoadingException("Could not open " + filePath));
}

bool Executable::isBinary(const string& fileText) const
{
	if (fileText.substr(0,4) == "ATMX" && fileText.substr(4,4) == "ATMO" && fileText[8] < motherboardNodesX && fileText[9] < motherboardNodesY)
		return true;
	else
		return false;
}

void Executable::loadExecutable(const std::string& fileText) throw(LoadingException)
{
	int index = 4;
	while (index+5 < fileText.size())
	{
		if (fileText.substr(index,4) != "ATMO")
		{
			this->_elements.clear();
			throw(LoadingException("Illegal element header"));
		}
		index += 4;
		
		int targetX = fileText[index];
		if (targetX >= motherboardNodesX)
		{
			this->_elements.clear();
			throw(LoadingException("Illegal target x coordinate"));
		}
		index++;
		
		int targetY = fileText[index];
		if (targetY >= motherboardNodesY)
		{
			this->_elements.clear();
			throw(LoadingException("Illegal target y coordinate"));
		}
		index++;
		
		NODE_INT_TYPE objectSize = NODE_INT_TYPE(fileText[index]) * 0x100;
		index++;
		objectSize += NODE_INT_TYPE(fileText[index]);
		index++;
		
		if (objectSize + index > fileText.size())
		{
			this->_elements.clear();
			throw(LoadingException("Illegal element size"));
		}
		
		shared_ptr<ExecutableElement> newElement;
		try
		{
			newElement = make_shared<ExecutableElement>();
		}
		catch(bad_alloc e)
		{
			this->_elements.clear();
			throw(LoadingException("Could not allocate memory for new elements"));
		}
		
		newElement->setMemory(fileText.substr(index,objectSize));
		index += objectSize;
		
		newElement->setX(targetX);
		newElement->setY(targetY);
		
		this->_elements.push_back(newElement);
	}
}

void Executable::saveExecutable() const throw(LoadingException)
{
	if (this->_elements.size() == 0)
		throw(LoadingException("Empty element list. Cannot create empty Executable"));
	
	string fileText("ATMX");
	for (shared_ptr<ExecutableElement> element : this->_elements)
	{
		fileText += "ATMO";
		
		fileText += char(element->getX());
		fileText += char(element->getY());
		
		fileText += char(element->getMemory().size() / 0x100);
		fileText += char(element->getMemory().size() % 0x100);
		
		for (uint8_t byte : element->getMemory())
		{
			fileText += char(byte);
		}
	}
	
	fstream file(this->_filePath+".atmx", ios_base::out | ios_base::binary | ios_base::trunc);
	if (file.is_open())
	{
		file << fileText;
		file.close();
	}
	else
		throw(LoadingException("Could not open executable for writing!"));
}

void Executable::assembleFile(const string& fileText) throw(LoadingException)
{
	string text = string(fileText);
	string currentLine = string();
	uint currentLineNumber = 1;
	bool successfull = true;
	
	if (text.back() != 4) // 4 is the ASCII character for end-of-file
		text.push_back(4);
	
	while (text.size() > 0)
	{
		if (text[0] == '\n' or text[0] == 4)
		{
			string includedCode = string("");
			if (! this->assembleLine(currentLine, currentLineNumber, &includedCode))
				successfull = false;
			if (includedCode.size() > 0)
			{
				text = includedCode.append(text);
			}
			currentLine = string();
			currentLineNumber++;
			
			if (text[0] == 4)
			{
				if (! this->_elements.back()->closeIncludedFile())
				{
					cerr << "ERROR while assembling!" << endl;
					cerr << "The source text contains more end-of-file characters (0x04)" << endl;
					cerr << "than included files!" << endl;
					throw(LoadingException("Assembly failed"));
				}
			}
		}
		else
		{
			currentLine.push_back(text[0]);
		}
		
		text.erase(0,1);
	}
	
	for (shared_ptr<ExecutableElement> element : this->_elements)
	{
		if (not element->linkLinesToMemory())
		{
			throw(LoadingException("Assembly failed"));
		}
	}
	
	if (not successfull)
	{
		throw(LoadingException("Assembly failed"));
	}
}

bool Executable::assembleLine(const std::string& line, uint lineNumber, string* outIncludedCode)
{
	shared_ptr<Assembler::Line> newLine(nullptr);
	try
	{
		newLine = make_shared<Assembler::Line>();
	}
	catch (std::bad_alloc e)
	{
		cerr << "ERROR while assembling!" << endl;
		cerr << "Could not allocate new memory!" << endl;
		return false;
	}
	
	newLine->setRawLine(line);
	newLine->setLineNumber(lineNumber);
	if (this->_elements.size() > 0 and this->_elements.back().get() != nullptr)
	{
		newLine->setFilePath(this->_elements.back()->getCurrentFilePath());
		newLine->setSymbols(this->_elements.back()->getSymbols());
		newLine->setMemoryLocation(this->_elements.back()->getNewLineLocation());
	}
	else
	{
		newLine->setFilePath(this->_filePath);
		newLine->setSymbols(shared_ptr<SymbolMap>(nullptr));
		newLine->setMemoryLocation(0);
	}
	
	if (! newLine->assembleLine())
		return false;
	
	// If the arguments were wrong, `newLine->assembleLine()` would have returned false.
	// Therefore we don't need to check this.
	if (newLine->getCommand().getType() == CommandType::NODE)
	{
		return this->addNewExecElement(newLine->getArgument0().getCode()[0]);
	}
	else
	if (newLine->getCommand().getType() == CommandType::MInclude)
	{
		return execInclude(newLine, outIncludedCode);
	}
	
	if (newLine->getCommand().getType() != CommandType::None)
	{
		if (this->_elements.size() > 0 and this->_elements.back().get() != nullptr)
		{
			this->_elements.back()->addLine(newLine);
			
			NODE_INT_TYPE oldNLL = this->_elements.back()->getNewLineLocation();
			NODE_INT_TYPE newNLL = oldNLL + newLine->getMemorySize();
			if (newNLL < oldNLL) // Which means that we hit the 64k limit
			{
				newLine->printErrorHeader();
				cerr << "The image size hit the limit of 64k!" << endl;
				return false;
			}
			this->_elements.back()->setNewLineLocation(newNLL);
			
			return true;
		}
		else
		{
			newLine->printErrorHeader();
			cerr << "Current target node is not specified!" << endl;
			cerr << "Maybe you forgot to use `NODE`?" << endl;
			return false;
		}
	}
	return true;
}

bool Executable::addNewExecElement(uint8_t code)
{
	shared_ptr<ExecutableElement> newExecElement;
	try
	{
		newExecElement = make_shared<ExecutableElement>();
	}
	catch (bad_alloc e)
	{
		cerr << "ERROR while assembling!" << endl;
		cerr << "Could not allocate new memory!" << endl;
		return false;
	}
	
	newExecElement->setBaseFilePath(this->_filePath);
	newExecElement->setX(code / 0x10);
	newExecElement->setY(code % 0x10);
	
	this->_elements.push_back(newExecElement);
	return true;
}

bool Executable::execInclude(shared_ptr<Assembler::Line> newLine, string* outIncludedCode)
{
	if (this->_elements.size() > 0 and this->_elements.back().get() != nullptr)
	{
		boost::filesystem::path pathToFile(newLine->getFilePath());
		pathToFile.remove_filename();
		pathToFile.append(newLine->getArgument0().getSymbolName());
		
		if (this->_elements.back()->addIncludedFile(pathToFile.string()))
		{
			fstream file(pathToFile.c_str(), ios_base::in | ios_base::binary);
			if (! file)
			{
				newLine->printErrorHeader();
				cerr << "Could not open " << pathToFile << "!" << endl;
				return false;
			}
			
			char nextChar;
			while (file.get(nextChar))
				outIncludedCode->push_back(nextChar);
			
			file.close();
			
			if (outIncludedCode->back() != 4)
				outIncludedCode->push_back(4);
		}
		// When the file was already included, we will simply ignore it.
		// This makes programming easier, since you can always list all files
		// you need without worrying about multiple or circular inclusion.
	}
	else
	{
		newLine->printErrorHeader();
		cerr << "Current target node is not specified!" << endl;
		cerr << "Maybe you forgot to use `NODE`?" << endl;
		return false;
	}
	
	return true;
}
