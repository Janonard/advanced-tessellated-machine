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
#define GLEW_STATIC
#include <GL/glew.h>
#include "GraphicsAdapter/VertexBuffer.h"

VertexBuffer::VertexBuffer() :
	_vertices{0}
{
	for (int i=0; i < GA_characters; i++)
	{
		this->_vertices[i*2] = i;
		this->_vertices[i*2+1] = (int) ' ';
	}
	
	GLuint pointer = this->get();
	glGenBuffers(1,&pointer);
	glBindBuffer(GL_ARRAY_BUFFER,pointer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(this->_vertices),this->_vertices,GL_DYNAMIC_DRAW);
	this->set(pointer);
}

VertexBuffer::~VertexBuffer()
{
	GLuint pointer = this->get();
	glDeleteBuffers(1, &pointer);
}

GLint VertexBuffer::getCharacter(const int index) const throw(std::out_of_range)
{
	if (index < GA_characters)
	{
		return this->_vertices[index*2+1];
	}
	else
	{
		throw(std::out_of_range("VertexBuffer::getCharacter"));
	}
}

void VertexBuffer::setCharacter(const int index, const GLint newCharacter) throw(std::out_of_range)
{
	if (index < GA_characters)
	{
		this->_vertices[index*2+1] = newCharacter;
		glBufferSubData(GL_ARRAY_BUFFER,
					sizeof(GLint)*(index*2+1),
					sizeof(GLint),
					&this->_vertices[index*2+1]);
	}
	else
	{
		throw(std::out_of_range("VertexBuffer::setCharacter"));
	}
}

