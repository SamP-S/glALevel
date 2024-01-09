#pragma once

#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include "File.h"

typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

std::string char_to_hex(char c)
{
	char hex_chars[] = { '0', '1', '2', '3', '4', '5', '6', '7','8','9', 'a', 'b', 'c', 'd', 'e', 'f' };
	// "a" = 61 = ab
	std::string s;
	uint8_t x = c;
	int a = x % 16;
	x = x - (x % 16) * 16;
	int b = x;

	s.push_back(hex_chars[a]);
	s.push_back(hex_chars[b]);
	return s;
}

std::string string_to_hex(std::string s)
{
	std::string hex;
	for (int i = 0; i < s.size(); i++)
	{
		hex.append(char_to_hex(s[i]));
	}
	return hex;
}

class BMP
{
private:
	struct pixel
	{
		int red;
		int green;
		int blue;
	};

public:
	unsigned int width;
	unsigned int height;

	uint8_t * dataBuffer[2] = { NULL, NULL };
	uint8_t * pixels = NULL;

	BITMAPFILEHEADER* bmpHeader = NULL;
	BITMAPINFOHEADER* bmpInfo = NULL;

	/* Output bmp file to console */
	void read_bmp(const char * filePath)
	{
		std::vector<std::string> file_raw = loadFile(filePath);
		std::vector<std::string> file_hex;
		for (int i = 0; i < file_raw.size(); i++)
		{
			file_hex.push_back(string_to_hex(file_raw[i]));

			std::cout << file_hex[i] << "\n";
		}
	}

	/* Load bmp file to access data */
	int load_bmp(const char * location)
	{
		// Use fstream to open file and check it is open
		std::ifstream FILE(location, std::ios::binary);
		if (!FILE)
		{
			std::cout << "ERROR: Cannot open file -> " << location << std::endl;
			return -1;
		}

		// Allocate memory for the headers
		dataBuffer[0] = new uint8_t[sizeof(BITMAPFILEHEADER)];
		dataBuffer[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];

		// Get the values with the data buffers
		FILE.read((char*)dataBuffer[0], sizeof(BITMAPFILEHEADER));
		FILE.read((char*)dataBuffer[1], sizeof(BITMAPINFOHEADER));

		// Construct loaded data into headers
		bmpHeader = (BITMAPFILEHEADER*)dataBuffer[0];
		bmpInfo = (BITMAPINFOHEADER*)dataBuffer[1];

		// Check the file is actually a bmp
		if (bmpHeader->bfType != 0x4D42)
		{
			std::cout << "ERROR: File -> " << location << " isn't a bitmap file\n";
			return -1;
		}

		// Allocate pixel memory
		pixels = new uint8_t[bmpInfo->biSizeImage];

		// Go to image data starts then read image data
		FILE.seekg(bmpHeader->bfOffBits);
		FILE.read((char*)pixels, bmpInfo->biSizeImage);

		/*
		Bitmaps store image data as BGR rather than RGB, so data must be converted
		*/

		// Convert from BGR to RGB
		uint8_t tempRGB = 0;	// Buffer
		for (unsigned long i = 0; i < bmpInfo->biSizeImage; i += 3)
		{
			tempRGB = pixels[i];
			pixels[i] = pixels[i + 2];
			pixels[i + 2] = tempRGB;
		}

		// Set width and height values from header
		width = bmpInfo->biWidth;
		height = bmpInfo->biHeight;

		/*std::string h;

		for (int y = height - 1; y > -1; y--)
		{
			std::cout << y << " ";
			for (int x = 0; x < width; x++)
			{
				for (int c = 0; c < 3; c++)
				{
					h = char_to_hex(pixels[y * 24 + x * 3 + c]);
					std::cout << h << " ";
				}
			}
			std::cout << std::endl;
		}*/
	}

};