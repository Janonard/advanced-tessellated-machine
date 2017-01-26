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

#include "GraphicsAdapter/ShaderProgram.h"

ShaderProgram::ShaderProgram(const GLuint vertexShader, const GLuint geometryShader, const GLuint fragmentShader)
{
	GLuint pointer = glCreateProgram();
	glAttachShader(pointer, vertexShader);
	glAttachShader(pointer, geometryShader);
	glAttachShader(pointer, fragmentShader);
	glBindFragDataLocation(pointer, 0, "outColor");
	glLinkProgram(pointer);
	glUseProgram(pointer);
	
	this->_characterAttribute = glGetAttribLocation(pointer, "inCharacter");
	glEnableVertexAttribArray(this->_characterAttribute);
	glVertexAttribIPointer(this->_characterAttribute, 2, GL_INT, 0, 0);
	
	glUniform1i(glGetUniformLocation(this->get(), "tex"),0);
}

ShaderProgram::~ShaderProgram()
{
	
}
