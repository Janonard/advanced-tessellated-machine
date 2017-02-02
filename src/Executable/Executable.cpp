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

#include "System/Motherboard.h"
#include "Assembler/File.h"

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
	shared_ptr<File> newAssFile = make_shared<File>();
	newAssFile->setBaseFilePath(this->_filePath);
	newAssFile->setText(fileText);
	
	if (newAssFile->assembleText())
	{
		this->_elements = newAssFile->getElements();
	}
	else
	{
		throw(LoadingException("Assembly failed"));
	}
}
