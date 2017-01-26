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

#include "System/ExtensionNode.h"
#include <vector>

#define KEYBOARD_ALLOWED_KEYS {GLFW_KEY_SPACE,\
								GLFW_KEY_0,\
								GLFW_KEY_1,\
								GLFW_KEY_3,\
								GLFW_KEY_3,\
								GLFW_KEY_4,\
								GLFW_KEY_5,\
								GLFW_KEY_6,\
								GLFW_KEY_7,\
								GLFW_KEY_8,\
								GLFW_KEY_9,\
								GLFW_KEY_A,\
								GLFW_KEY_B,\
								GLFW_KEY_C,\
								GLFW_KEY_D,\
								GLFW_KEY_E,\
								GLFW_KEY_F,\
								GLFW_KEY_W,\
								GLFW_KEY_Q,\
								GLFW_KEY_R,\
								GLFW_KEY_S,\
								GLFW_KEY_T,\
								GLFW_KEY_V,\
								GLFW_KEY_W,\
								GLFW_KEY_X,\
								GLFW_KEY_Y,\
								GLFW_KEY_Z,\
								GLFW_KEY_ESCAPE,\
								GLFW_KEY_ENTER,\
								GLFW_KEY_BACKSPACE,\
								GLFW_KEY_LEFT,\
								GLFW_KEY_RIGHT,\
								GLFW_KEY_UP,\
								GLFW_KEY_DOWN,\
								GLFW_KEY_F1,\
								GLFW_KEY_F2,\
								GLFW_KEY_F3,\
								GLFW_KEY_F4,\
								GLFW_KEY_F5,\
								GLFW_KEY_F6,\
								GLFW_KEY_F7,\
								GLFW_KEY_F8,\
								GLFW_KEY_F9,\
								GLFW_KEY_F10,\
								GLFW_KEY_F11,\
								GLFW_KEY_F12\
}\

enum KeyboardAction
{
	Release, Press, Char
};

struct KeyboardActivity
{
	NODE_INT_TYPE code;
	KeyboardAction action;
};

class KeyboardAdapter : public ExtensionNode
{
public:
	
	KeyboardAdapter();
	
	~KeyboardAdapter();
	
	bool configure(const Config& configuration, GLFWwindow * context) override;
	
	bool pretick() override;
	
	bool tick() override;
	
	std::string getDebugString() const override;
	
	static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	static void glfwCharCallback(GLFWwindow* window, unsigned int codepoint);
};
