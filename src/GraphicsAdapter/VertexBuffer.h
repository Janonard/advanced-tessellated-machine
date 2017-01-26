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

#include <stdexcept>
#include <memory>

#include "GraphicsAdapter/Constants.h"
#include "GraphicsAdapter/GLResource.h"

/*
 * Class for creation, usage and destruction of the default vertex buffer.
 */
class VertexBuffer : public GLResource
{
public:
	/*
	 * Default constructor
	 */
	VertexBuffer();
	/*
	 * Default destructor
	 */
	~VertexBuffer() override;
	
	/*
	 * Returns the character at the given index.
	 * Throws an out_of_range excepetion if the index
	 * is greater than GA_characters.
	 */
	GLint getCharacter(const int index) const throw(std::out_of_range);
	
	/*
	 * Sets the character at the given index.
	 * Throws an out_of_range excepetion if the index
	 * is greater than GA_characters.
	 */
	void setCharacter(const int index, const GLint newCharacter) throw(std::out_of_range);
	
private:
	/*
	 * The array containing all vertices.
	 */
	GLint _vertices[GA_characters*2];
};
