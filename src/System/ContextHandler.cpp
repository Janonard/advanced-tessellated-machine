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
#include "System/ContextHandler.h"
#include "System/Motherboard.h"

ContextHandler::ContextHandler(bool fullscreenWindow, float screenSizeMulitplier) throw(std::runtime_error)
{
	if (! glfwInit())
		throw(std::runtime_error("Could initialize GLFW!"));
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	if (fullscreenWindow)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
		this->_context = glfwCreateWindow(vidmode->width,
											vidmode->height,
											"advanced tesselated machine",
											monitor,
											nullptr);
	}
	else
	{
		this->_context = glfwCreateWindow(motherboardScreenSizeX*screenSizeMulitplier,
											motherboardScreenSizeY*screenSizeMulitplier,
											"advanced tesselated machine",
											nullptr,
											nullptr);
	}
	
	if (this->_context == NULL)
		throw(std::runtime_error("Could create window!"));
	
	glfwMakeContextCurrent(this->_context);
}

ContextHandler::~ContextHandler()
{
	glfwDestroyWindow(this->_context);
}

GLFWwindow * ContextHandler::getContext() const
{
	return this->_context;
}

