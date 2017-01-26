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
#include <SOIL/SOIL.h>
#include <iostream>
#include <fstream>
#include "GraphicsAdapter/GraphicsAdapter.h"

using namespace std;
using namespace chrono;

const GLchar* vertexShaderSource =
	"#version 150 \n"
	"in ivec2 inCharacter;"
	"out vec2 geomTextureCoordinate;"
	
	"vec2 calculatePosition(int memoryPosition, int fieldSizeX, int fieldSizeY)"
	"{"
		"vec2 position;"
		"position[0] = float(memoryPosition % fieldSizeX);"
		"position[0] = position[0] / float(fieldSizeX);"
		"position[0] = (position[0] * 2.0) - 1.0;"
		"position[1] = float(memoryPosition / fieldSizeX);"
		"position[1] = position[1] / float(fieldSizeY);"
		"position[1] = (position[1] * -2.0) + 1.0;"
		"return position;"
	"}"
	
	"vec2 calculateTexturePosition(int character)"
	"{"
		"vec2 position;"
		"position[0] = float(character % 16);"
		"position[0] = position[0] / 16.0;"
		"position[1] = float(character / 16);"
		"position[1] = position[1] - 0x2;"
		"position[1] = position[1] / 6.0;"
		"position[1] = position[1];"
		"return position;"
	"}"
	
	"void main()"
	"{"
		"int screenSizeX = 30;"
		"int screenSizeY = 24;"
		
		"gl_Position = vec4(calculatePosition(inCharacter[0], 30, 24), 0.0, 1.0);"
		
		"if (inCharacter[1] >= 0x20 && inCharacter[1] <= 0x7F) // the range of printable characters\n"
			"geomTextureCoordinate = calculateTexturePosition(inCharacter[1]);"
		"else "
			"geomTextureCoordinate = vec2(0.0,1.0); // the upper left corner, which represents a space.\n"
	"}";

const GLchar* geometryShaderSource =
	"#version 150 \n"
	"layout(points) in;"
	"layout(triangle_strip, max_vertices =5) out;"
	
	"in vec2 geomTextureCoordinate[];"
	"out vec2 fragTextureCoordinate;"
	
	"void main()"
	"{"
		"float offsetX = 1.0 / 15.0;"
		"float offsetY = (1.0 / 12.0) * -1.0;"
		"float textureOffsetX = 1.0 / 16.0;"
		"float textureOffsetY = (1.0 / 6.0);"
		
		"fragTextureCoordinate = geomTextureCoordinate[0];"
		"gl_Position = gl_in[0].gl_Position;"
		"EmitVertex();"
		
		"fragTextureCoordinate = geomTextureCoordinate[0] + vec2(textureOffsetX,0);"
		"gl_Position = gl_in[0].gl_Position + vec4(offsetX, 0.0, 0.0, 0.0);"
		"EmitVertex();"
		
		"fragTextureCoordinate = geomTextureCoordinate[0] + vec2(textureOffsetX,textureOffsetY);"
		"gl_Position = gl_in[0].gl_Position + vec4(offsetX, offsetY, 0.0, 0.0);"
		"EmitVertex();"
		
		"fragTextureCoordinate = geomTextureCoordinate[0] + vec2(0,textureOffsetY);"
		"gl_Position = gl_in[0].gl_Position + vec4(0.0, offsetY, 0.0, 0.0);"
		"EmitVertex();"
		
		"fragTextureCoordinate = geomTextureCoordinate[0];"
		"gl_Position = gl_in[0].gl_Position;"
		"EmitVertex();"
		
		"EndPrimitive();"
	"}";

const GLchar* fragmentShaderSource =
	"#version 150 \n"
	
	"in vec2 fragTextureCoordinate;"
	"uniform sampler2D tex;"
	"out vec4 outColor;"
	
	"void main()"
	"{"
		"outColor = texture(tex,fragTextureCoordinate);"
	"}";

GraphicsAdapter::GraphicsAdapter() :
	_progressMode(false),
	_positionUpperByte(0),
	_positionLowerByte(0),
	_vertexArray(nullptr),
	_vertexBuffer(nullptr),
	_vertexShader(nullptr),
	_geometryShader(nullptr),
	_fragmentShader(nullptr),
	_shaderProgram(nullptr),
	_characterTexture(nullptr),
	_lastScreenRefresh(steady_clock::now())
{
}

bool GraphicsAdapter::configure(const Config& configuration, GLFWwindow* context)
{
	this->_context = context;
	
	/*
	 * Vertex Array
	 */
	try { this->_vertexArray.reset(new VertexArray); }
	catch (std::bad_alloc e)
	{
		cerr << "ERROR: Could not allocate more memory!" << endl;
		this->destroyResources();
		return false;
	}
	
	/*
	 * Vertex Buffer
	 */
	try { this->_vertexBuffer.reset(new VertexBuffer); }
	catch (std::bad_alloc e)
	{
		cerr << "ERROR: Could not allocate more memory!" << endl;
		this->destroyResources();
		return false;
	}
	
	/*
	 * Vertex Shader
	 */
	try { this->_vertexShader.reset(new Shader(vertexShaderSource,GL_VERTEX_SHADER)); }
	catch (std::bad_alloc e)
	{
		cerr << "ERROR: Could not allocate more memory!" << endl;
		this->destroyResources();
		return false;
	}
	catch (std::invalid_argument e)
	{
		this->destroyResources();
		return false;
	}
	
	/*
	 * Geometry Shader
	 */
	try{ this->_geometryShader.reset(new Shader(geometryShaderSource,GL_GEOMETRY_SHADER)); }
	catch (std::invalid_argument e)
	{
		this->destroyResources();
		return false;
	}
	catch (std::bad_alloc e)
	{
		cerr << "ERROR: Could not allocate more memory!" << endl;
		this->destroyResources();
		return false;
	}
	
	/*
	 * Fragment Shader
	 */
	try { this->_fragmentShader.reset(new Shader(fragmentShaderSource,GL_FRAGMENT_SHADER)); }
	catch (std::invalid_argument e)
	{
		this->destroyResources();
		return false;
	}
	catch (std::bad_alloc e)
	{
		cerr << "ERROR: Could not allocate more memory!" << endl;
		this->destroyResources();
		return false;
	}
	
	/*
	 * Shader Program
	 */
	try { this->_shaderProgram.reset(new ShaderProgram(this->_vertexShader->get(), this->_geometryShader->get(), this->_fragmentShader->get())); }
	catch (std::bad_alloc e)
	{
		cerr << "ERROR: Could not allocate more memory!" << endl;
		this->destroyResources();
		return false;
	}
	
	/*
	 * Shader Program
	 */
	try { this->_characterTexture.reset(new Texture()); }
	catch (std::invalid_argument e)
	{
		this->destroyResources();
		return false;
	}
	catch (std::bad_alloc e)
	{
		cerr << "ERROR: Could not allocate more memory!" << endl;
		this->destroyResources();
		return false;
	}
	
	return true;
}

bool GraphicsAdapter::deconfigure()
{
	this->destroyResources();
	
	return true;
}

bool GraphicsAdapter::tick()
{
	NODE_INT_TYPE rawData;
	bool dataPulled = false;
	Direction pulledDirection = Direction::UP;
	for (Direction direction : this->getWatchDirections())
	{
		if (this->pull(direction, &rawData))
		{
			dataPulled = true;
			pulledDirection = direction;
			break;
		}
	}
	
	if (dataPulled)
	{
		NODE_INT_TYPE command = rawData / 0x100;
		NODE_INT_TYPE data = rawData % 0x100;
		switch (command)
		{
		case GA_GetProgressMode:
			this->execGetProgressMode(pulledDirection);
			break;
		case GA_SetProgressMode:
			this->execSetProgressMode(data);
			break;
		case GA_GetPosition:
			this->execGetPosition(pulledDirection);
			break;
		case GA_SetPositionUpperByte:
			this->execSetPositionUpperByte(data);
			break;
		case GA_SetPositionLowerByte:
			this->execSetPositionLowerByte(data);
			break;
		case GA_GetChar:
			this->execGetChar(pulledDirection);
			break;
		case GA_SetChar:
			this->execSetChar(data);
			break;
		case GA_ScrollLine:
			this->execScrollLine();
			break;
		default:
			break;
		}
	}
	
	this->refreshWindow();
	
	return true;
}

std::string GraphicsAdapter::getDebugString() const
{
	return "graphics adapter";
}

void GraphicsAdapter::refreshWindow()
{
	if (duration_cast<milliseconds>(steady_clock::now() - this->_lastScreenRefresh).count() >= 50) // targeted fps: 20
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_POINTS,0,GA_characters);
		glfwSwapBuffers(this->_context);
		this->_lastScreenRefresh = steady_clock::now();
	}
}

void GraphicsAdapter::destroyResources()
{
	this->_characterTexture.reset(nullptr);
	this->_vertexShader.reset(nullptr);
	this->_geometryShader.reset(nullptr);
	this->_fragmentShader.reset(nullptr);
	this->_vertexBuffer.reset(nullptr);
	this->_vertexArray.reset(nullptr);
}

void GraphicsAdapter::increasePositionPointer(uint16_t connectedPosition)
{
	if (this->_progressMode && connectedPosition+1 < GA_characters)
	{
		if (this->_positionLowerByte == 0xFF)
		{
				this->_positionLowerByte = 0;
				this->_positionUpperByte++;
		}
		else
		{
			this->_positionLowerByte++;
		}
	}
}

bool GraphicsAdapter::execGetProgressMode(Direction pulledDirection)
{
	this->addPushQueueElement(pulledDirection, (NODE_INT_TYPE) this->_progressMode);
	return true;
}

bool GraphicsAdapter::execSetProgressMode(NODE_INT_TYPE data)
{
	if (data < 2)
	{
		if (data == (NODE_INT_TYPE)true)
			this->_progressMode =  true;
		else
			this->_progressMode = false;
	}
	return true;
}

bool GraphicsAdapter::execGetPosition(Direction pulledDirection)
{
	this->addPushQueueElement(pulledDirection, this->_positionUpperByte * 0x100 + this->_positionLowerByte);
	return true;
}

bool GraphicsAdapter::execSetPositionUpperByte(NODE_INT_TYPE data)
{
	this->_positionUpperByte = data;
	return true;
}

bool GraphicsAdapter::execSetPositionLowerByte(NODE_INT_TYPE data)
{
	this->_positionLowerByte = data;
	return true;
}

bool GraphicsAdapter::execGetChar(Direction pulledDirection)
{
	uint16_t connectedPosition = this->_positionUpperByte * 0x100 + this->_positionLowerByte;
	if (connectedPosition < GA_characters)
	{
		NODE_INT_TYPE character = 0;
		
		try
		{
			character = (NODE_INT_TYPE) this->_vertexBuffer->getCharacter(connectedPosition);
		}
		catch (std::out_of_range e)
		{
			std::cerr << "ERROR: Graphic Adapter's character pointer is out of range!" << std::endl;
			return false;
		}
		
		this->addPushQueueElement(pulledDirection, character);
		this->increasePositionPointer(connectedPosition);
	}
	return true;
}

bool GraphicsAdapter::execSetChar(NODE_INT_TYPE data)
{
	uint16_t connectedPosition = this->_positionUpperByte * 0x100 + this->_positionLowerByte;
	if (connectedPosition < GA_characters)
	{
		try
		{
			this->_vertexBuffer->setCharacter(connectedPosition,(GLint) data);
		}
		catch (std::out_of_range e)
		{
			std::cerr << "ERROR: Graphic Adapter's character pointer is out of range!" << std::endl;
			return false;
		}
		
		this->increasePositionPointer(connectedPosition);
	}
	return true;
}

bool GraphicsAdapter::execScrollLine()
{
	NODE_INT_TYPE currentPosition = 0;
	int charCountX = GA_screenWidth / GA_charWidth;
	
	while (currentPosition < (GA_characters - charCountX))
	{
		try
		{
			GLint movingChar = this->_vertexBuffer->getCharacter(currentPosition + charCountX);
			this->_vertexBuffer->setCharacter(currentPosition + charCountX, movingChar);
		}
		catch (std::out_of_range e)
		{
			std::cerr << "ERROR: Graphic Adapter's character pointer is out of range!" << std::endl;
			return false;
		}
		
		currentPosition++;
	}
	
	currentPosition = GA_characters - charCountX;
	while (currentPosition < GA_characters)
	{
		try
		{
			this->_vertexBuffer->setCharacter(currentPosition + charCountX, (GLint) ' ');
		}
		catch (std::out_of_range e)
		{
			std::cerr << "ERROR: Graphic Adapter's character pointer is out of range!" << std::endl;
			return false;
		}
		currentPosition++;
	}
	
	return true;
}
