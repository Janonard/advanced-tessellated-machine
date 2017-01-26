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
#include "GraphicsAdapter/VertexArray.h"

VertexArray::VertexArray()
{
	GLuint vertexArray = 0;
	glGenVertexArrays(1, &(vertexArray));
	glBindVertexArray(vertexArray);
	this->set(vertexArray);
}

VertexArray::~VertexArray()
{
	GLuint vertexArray = this->get();
	glDeleteVertexArrays(1, &vertexArray);
}

