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

#include <GLFW/glfw3.h>

#include "GraphicsAdapter/GLResource.h"

/*
 * Class for handling shader program con- and destruction.
 */
class ShaderProgram : public GLResource
{
public:
	/*
	 * Constructor that takes these three shaders and links them to a program.
	 */
	ShaderProgram(const GLuint vertexShader, const GLuint geometryShader, const GLuint fragmentShader);
	
	/*
	 * Default destructor
	 */
	~ShaderProgram();
	
private:
	/*
	 * The attribute for the character alignment.
	 */
	GLint _characterAttribute;
};
