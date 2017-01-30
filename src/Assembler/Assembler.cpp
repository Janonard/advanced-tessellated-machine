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
#include "Assembler/Assembler.h"
#include <iostream>

using namespace std;

Assembler::Assembler() :
	_text(),
	_line(),
	_elements(),
	_filePathStack()
{
}

Assembler::~Assembler()
{
}

bool Assembler::assembleText()
{
	this->_line = vector<string>();
	this->_line.push_back("");
	
	bool commenting = false;
	bool bracketing = false;
	
	while (this->_text.size() > 0)
	{
		char c = this->_text[0];
		this->_text.erase(0,1);
		
		if (bracketing)
		{
			if (c == '\n')
			{
				this->printErrorHeader();
				cerr << "Cannot start a new line while having a bracket open!" << endl;
				return false;
			}
			else if (c == ';')
			{
				this->printErrorHeader();
				cerr << "Cannot start a comment while having a bracket open!" << endl;
				return false;
			}
			else if (c == '>')
			{
				this->_line.back().push_back(c);
				bracketing = false;
			}
			else
			{
				this->_line.back().push_back(c);
			}
		}
		else
		{
			if (c == '\n')
			{
				if (! this->assembleLine())
					return false;
				
				this->_line = vector<string>();
				this->_line.push_back("");
				commenting = false;
			}
			else if (! commenting)
			{
				if (c == '<')
				{
					this->_line.back().push_back(c);
					bracketing = true;
				}
				else if (c == ';')
				{
					commenting = true;
				}
				else if ((c == ' ') or (c == '\t'))
				{
					if (this->_line.back().size() > 0)
					{
						this->_line.push_back("");
					}
				}
				else
				{
					this->_line.back().push_back(c);
				}
			}
		}
	}
	
	if (this->_line[0].size() > 0) // which means that there is something
	{
		return this->assembleLine();
	}
	else
		return true;
}

bool Assembler::assembleLine()
{
	for (string word : this->_line)
	{
		cout << word << "|";
	}
	cout << endl;
	return true;
}

void Assembler::printErrorHeader()
{
	cerr << "ERROR while assembling file '" << _filePathStack.top() << "'!" << endl;
}

const std::string & Assembler::getText() const
{
	return this->_text;
}

void Assembler::setText(const std::string& text)
{
	this->_text = text;
}

vector<shared_ptr<ExecutableElement>> Assembler::getElements() const
{
	return this->_elements;
}

void Assembler::resetElements()
{
	this->_elements = vector<shared_ptr<ExecutableElement>>();
}

const std::string & Assembler::getCurrentFilePath() const
{
	return this->_filePathStack.top();
}

void Assembler::setBaseFilePath(std::string baseFilePath)
{
	this->_filePathStack = stack<string>();
	this->_filePathStack.push(baseFilePath);
}
