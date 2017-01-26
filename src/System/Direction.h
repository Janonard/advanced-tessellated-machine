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

/*
* enum Direction
* This is an enum created to be a parameter for the Node class. It discribes each of the six sides of
* a node.
*/
enum Direction
{
	UP			=0,
	UP_LEFT		=1,
	LEFT		=2,
	DOWN_LEFT	=3,
	DOWN		=4,
	DOWN_RIGHT	=5,
	RIGHT		=6,
	UP_RIGHT	=7
};
