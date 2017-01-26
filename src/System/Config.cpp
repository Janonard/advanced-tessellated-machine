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
#include "System/Config.h"
#include <fstream>

using namespace std;

Config::Config()
{
	this->_entries = unordered_map<string, string>();
}

Config Config::loadFromFile(string pathToFile)
{
	Config parsedConfig;

	fstream configFile;
	configFile.open(pathToFile.c_str(), ios_base::in);
	if (!configFile)
		return Config();

	while (! configFile.eof())
	{
		string line;
		getline(configFile, line);
		if (line[0] != '#')
		{
			int equalSignPos = line.find('=');
			if (equalSignPos != string::npos)
			{
				string name = line.substr(0, equalSignPos);
				string entry = line.substr(equalSignPos+1, line.length());
				parsedConfig._entries.emplace(name, entry);
			}
		}
	}

	configFile.close();

	return parsedConfig;
}

bool Config::getEntry(std::string name, std::string* outEntry) const
{
	unordered_map<string, string>::const_iterator entry = this->_entries.find(name);
	if (entry != this->_entries.end())
	{
		*outEntry = entry->second;
		return true;
	}
	else
	{
		return false;
	}
	return true;
}
