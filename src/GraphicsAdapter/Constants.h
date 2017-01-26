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
#include <string>
#include <array>

/*
 * Global constants
 */

// screen width in pixels
const int GA_screenWidth = 570;

// screen height in pixels
const int GA_screenHeight = 456;

// character width in pixels
const int GA_charWidth = 19;

// character height in pixels
const int GA_charHeight = 19;

// Number of all characters on the screen
const int GA_characters = (GA_screenWidth / GA_charWidth) * (GA_screenHeight / GA_charHeight);

// The name of the texture file
const std::string GA_textureName = "charset.png";

// The paths where the texture file may lay
const std::array<std::string,3> GA_texturePaths = {{"./", "/usr/share/atm/", "/usr/local/share/atm/"}};
