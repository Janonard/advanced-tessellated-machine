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

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "GraphicsAdapter/Constants.h"

#include "GraphicsAdapter/Texture.h"

using namespace std;

Texture::Texture() throw(std::invalid_argument)
{
	bool fileFound = false;
	string filePath = "";
	for (string path : GA_texturePaths)
	{
		fstream file;
		file.open(path+GA_textureName, fstream::in | fstream::binary);
		if (file)
		{
			file.close();
			fileFound = true;
			filePath = path+GA_textureName;
			break;
		}
	}
	
	if (!fileFound)
	{
		cerr << "ERROR: Could not load character texture!" << endl;
		throw(std::invalid_argument("Could not load character texture!"));
	}
	
	int width, height;
	unsigned char* loadedImage = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	
	GLuint pointer = 0;
	glGenTextures(1,&pointer);
	glBindTexture(GL_TEXTURE_2D, pointer);
	this->set(pointer);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImage);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SOIL_free_image_data(loadedImage);
}

Texture::~Texture()
{
}
