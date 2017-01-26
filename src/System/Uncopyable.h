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
 * This is a base class for everything that
 * must not be copied.
 */
class Uncopyable
{
public:
	Uncopyable(); // empty constructor
	virtual ~Uncopyable(); // empty destructor
	
private:
	Uncopyable(const Uncopyable&); // inaccessible copy constructer
	Uncopyable& operator=(const Uncopyable&); // inaccessible assignment operator
};
