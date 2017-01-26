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

#include <stdexcept>

#include "GraphicsAdapter/GLResource.h"

/*
 * Class for loading the texture
 */
class Texture : GLResource
{
public:
	/*
	 * Default constructor. It will throw an invalid_argument if the
	 * texture could not be loaded.
	 */
	Texture() throw(std::invalid_argument);
	/*
	 * Default destructor.
	 */
	~Texture() override;
};
