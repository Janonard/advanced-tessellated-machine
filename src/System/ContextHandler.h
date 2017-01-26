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
#include "System/Uncopyable.h"
#include <stdexcept>
#include <GLFW/glfw3.h>

/*
 * Class for handling a GLFW context.
 * When an object of this class is created, it initializes GLFW
 * and an context. At destruction, this context will be deleted.
 */
class ContextHandler : public Uncopyable
{
public:
	/*
	 * Creates a window with the given width, height and monitor
	 */
	ContextHandler(int width, int height, GLFWmonitor* monitor) throw(std::runtime_error);
	/*
	 * Destroys the window.
	 */
	~ContextHandler() override;
	
	GLFWwindow* getContext() const;
	
private:
	GLFWwindow* _context;
	
};
