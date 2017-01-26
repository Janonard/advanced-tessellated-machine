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

#include <unordered_map>
#include <array>
#include <chrono>
#include <memory>

#include "GraphicsAdapter/Constants.h"
#include "GraphicsAdapter/VertexArray.h"
#include "GraphicsAdapter/VertexBuffer.h"
#include "GraphicsAdapter/Shader.h"
#include "GraphicsAdapter/ShaderProgram.h"
#include "GraphicsAdapter/Texture.h"

#include "System/ExtensionNode.h"

/*
 * All commands the graphic mode understands.
 */
enum GA_Commands
{
	GA_GetProgressMode		=0x00,
	GA_SetProgressMode		=0x01,
	GA_GetPosition			=0x02,
	GA_SetPositionUpperByte	=0x03,
	GA_SetPositionLowerByte	=0x04,
	GA_GetChar				=0x05,
	GA_SetChar				=0x06,
	GA_ScrollLine			=0x07
};

/*
 * The second GL implementation of the graphics adapter.
 * It listens for incoming data and passes it the
 * currently active graphic mode object.
 */
class GraphicsAdapter : public ExtensionNode
{
public: // functions
	GraphicsAdapter();
	
	/*
	 * Configures the graphics adapter and it's graphic modes.
	 */
	bool configure(const Config& configuration, GLFWwindow* context) override;
	
	/*
	 * Deconfigures and cleans the graphics adapter and it's graphic
	 * modes.
	 */
	bool deconfigure() override;
	
	/*
	 * Executes a tick. When data arrives, it interprets it.
	 * First it checks wether it is a command to get or set
	 * the graphic mode. If this is not true, it passes the data
	 * to currently active graphic mode.
	 */
	bool tick() override;
	
	std::string getDebugString() const override;
	
private: // functions
	
	/*
	 * cleans the screen and renders all characters.
	 */
	void refreshWindow();
	
	/*
	 * Destroys all our currently used OpenGL resources.
	 * It is save to call this even if only some or no
	 * resources are currently bound.
	 */
	void destroyResources();
	
	/*
	 * Increases the splitted position pointer if we are in progress mode and the result
	 * is lower than GA_characters. The argument is the current position unified into one value.
	 * Since all callers calculate it on their own, it would be a wast of time to redo it.
	 */
	void increasePositionPointer(uint16_t connectedPosition);

	bool execGetProgressMode(Direction pulledDirection);
	
	bool execSetProgressMode(NODE_INT_TYPE data);
	
	bool execGetPosition(Direction pulledDirection);
	
	bool execSetPositionUpperByte(NODE_INT_TYPE data);
	
	bool execSetPositionLowerByte(NODE_INT_TYPE data);
	
	bool execGetChar(Direction pulledDirection);
	
	bool execSetChar(NODE_INT_TYPE data);
	
	bool execScrollLine();
	
private: // properties
	
	/*
	 * If true, _position will be increased every time when execGetChar() or execSetChar() gets called.
	 */
	bool _progressMode;
	
	/*
	 * The current position pointer split up in an upper and in a lower byte.
	 * execGetChar() and execSetChar() always return or override the character it points to.
	 */
	uint8_t _positionUpperByte;
	uint8_t _positionLowerByte;
	
	/*
	 * OpenGL objects.
	 */
	std::unique_ptr<VertexArray> _vertexArray;
	std::unique_ptr<VertexBuffer> _vertexBuffer;
	std::unique_ptr<Shader> _vertexShader;
	std::unique_ptr<Shader> _geometryShader;
	std::unique_ptr<Shader> _fragmentShader;
	std::unique_ptr<ShaderProgram> _shaderProgram;
	std::unique_ptr<Texture> _characterTexture;
	
	/*
	 * The timepoint when the screen was refreshed the last time.
	 */
	std::chrono::time_point<std::chrono::steady_clock> _lastScreenRefresh;
	
};
