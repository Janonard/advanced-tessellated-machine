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

#include <iostream>

#include "GraphicsAdapter/Shader.h"

Shader::Shader(const GLchar* source, const GLenum type) throw(std::invalid_argument)
{
	this->set(glCreateShader(type));
	glShaderSource(this->get(),1,&source,NULL);
	glCompileShader(this->get());
	
	GLint status;
	glGetShaderiv(this->get(),GL_COMPILE_STATUS,&status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetShaderInfoLog(this->get(),512,NULL,buffer);
		std::cerr << "ERROR: Could not compile shader!" << std::endl;
		std::cerr << buffer << std::endl;
		
		glDeleteShader(this->get());
		throw(std::invalid_argument("Could not compile shader!"));
	}
}

Shader::~Shader()
{
	glDeleteShader(this->get());
}

