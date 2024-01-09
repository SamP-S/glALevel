#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> loadFile(const char * filePath)
{
	std::ifstream FILE;		// IN Fie Stream
	FILE.open(filePath);	// Opens file from passed filePath

	std::string string;		// String to read file line-by-line through
	std::vector<std::string> file_string;

	// Check file has been found and opened correcty
	if (!FILE.is_open())
	{
		//std::cout << "ERROR: file => " << file.is_open << std::endl;
		std::cout << "ERROR: Cannot open file -> " << filePath << "\n";
		return file_string;
	}
	
	// Loop for every line in the file
	while (std::getline(FILE, string))
	{
		file_string.push_back(string);		// Adds line to a dynamic array of strings
	}

	if (file_string.size() == 0)			// If there is no data in the file
	{
		std::cout << "ERROR: File is empty\n";	// Error message
		return file_string;
	}
	
	std::cout << "OK: File successfully loaded\n";
	return file_string;
}