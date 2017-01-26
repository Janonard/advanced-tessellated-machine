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
#include "KeyboardAdapter/KeyboardAdapter.h"
#include <iostream>
#include <algorithm>
#include <queue>

std::queue<KeyboardActivity>* activityQueue;

std::vector<int>* allowedKeys;

KeyboardAdapter::KeyboardAdapter()
{
	activityQueue = new std::queue<KeyboardActivity>;
	allowedKeys = new std::vector<int>(KEYBOARD_ALLOWED_KEYS);
}

KeyboardAdapter::~KeyboardAdapter()
{
	delete activityQueue;
	activityQueue = nullptr;
	delete allowedKeys;
	allowedKeys = nullptr;
}

bool KeyboardAdapter::configure(const Config& configuration, GLFWwindow* context)
{
	this->_context = context;
	glfwSetKeyCallback(this->_context, &KeyboardAdapter::glfwKeyCallback);
	glfwSetCharCallback(this->_context, &KeyboardAdapter::glfwCharCallback);
	return true;
}


bool KeyboardAdapter::pretick()
{
	if (activityQueue != nullptr)
	{
		if (! activityQueue->empty())
		{
			KeyboardActivity activity = activityQueue->front();
			NODE_INT_TYPE data = activity.code;
			
			if (activity.action == KeyboardAction::Press)
				data += 0x0000;
			else if (activity.action == KeyboardAction::Release)
				data += 0x1000;
			else
				data += 0x2000;
			
			bool successfull = this->push(this->getWatchDirections()[0],data);
			
			if (successfull)
			{
				activityQueue->pop();
			}
		}
	}
	return true;
}

bool KeyboardAdapter::tick()
{
	if (activityQueue != nullptr)
	{
		NODE_INT_TYPE buffer;
		if (this->pull(this->getWatchDirections()[0], &buffer))
		{
			delete activityQueue;
			activityQueue = new std::queue<KeyboardActivity>;
		}
	}
	
	return true;
}

std::string KeyboardAdapter::getDebugString() const
{
	return "keyboard adapter";
}

void KeyboardAdapter::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (allowedKeys != nullptr && activityQueue != nullptr)
	{
		std::vector<int>::iterator it = std::find(allowedKeys->begin(), allowedKeys->end(), key);
		if (it != allowedKeys->end()) // the key is in the list
		{
			KeyboardActivity activity;
			
			activity.code = (NODE_INT_TYPE) key;
			
			if (action == GLFW_PRESS)
				activity.action = KeyboardAction::Press;
			else if (action == GLFW_RELEASE)
				activity.action = KeyboardAction::Release;
			
			activityQueue->push(activity);
		}
	}
}

void KeyboardAdapter::glfwCharCallback(GLFWwindow* window, unsigned int codepoint)
{
	if (activityQueue != nullptr && codepoint <= 0xFFF) // otherwise it would be to big to fit
	{
		KeyboardActivity activity;
		activity.code = (NODE_INT_TYPE) codepoint;
		activity.action = KeyboardAction::Char;
		activityQueue->push(activity);
	}
}

