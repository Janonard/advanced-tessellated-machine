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

#include <string>
#include <unordered_map>

class Config
{
public:
	Config();

	// Loads the config file and returns a Config object with the read entries.
	// Will return an empty Config object when failed.
	static Config loadFromFile(std::string pathToFile);

	// Looks up a configuration entry by it's name and writes the result into outEntry.
	// Returns true if successfull and false if not.
	bool getEntry(std::string name, std::string* outEntry) const;

private:

	// Contains all configuration entries.
	std::unordered_map<std::string, std::string> _entries;
};
