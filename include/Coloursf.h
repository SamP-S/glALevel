#pragma once

/* RGBA Colours */
#define RGBA_BLACK 0.0f, 0.0f, 0.0f, 1.0f
#define RGBA_WHITE 1.0f, 1.0f, 1.0f, 1.0f

#define RGBA_RED 1.0f, 0.0f, 0.0f, 1.0f
#define RGBA_GREEN 0.0f, 1.0f, 0.0f, 1.0f
#define RGBA_BLUE 0.0f, 0.0f, 1.0f, 1.0f

#define RGBA_CYAN 0.0f, 1.0f, 1.0f, 1.0f
#define RGBA_YELLOW 1.0f, 1.0f, 0.0f, 1.0f
#define RGBA_MAGENTA 1.0f, 0.0f, 0.0f, 1.0f

#define RGBA_TRANSPARENT 1.0f, 1.0f, 1.0f, 0.0f
#define RGBA_TRANSLUCENT 1.0f, 1.0f, 1.0f, 0.5f

/* ARGB Colours */
#define ARGB_BLACK 1.0f, 0.0f, 0.0f, 0.0f
#define ARGB_WHITE 1.0f, 1.0f, 1.0f, 1.0f

#define ARGB_RED 1.0f, 1.0f, 0.0f, 0.0f
#define ARGB_GREEN 1.0f, 0.0f, 1.0f, 0.0f
#define ARGB_BLUE 1.0f, 0.0f, 0.0f, 1.0f

#define ARGB_CYAN 1.0f, 0.0f, 1.0f, 1.0f
#define ARGB_YELLOW 1.0f, 1.0f, 1.0f, 0.0f
#define ARGB_MAGENTA 1.0f, 1.0f, 0.0f, 0.0f

#define ARGB_TRANSPARENT 0.0f, 1.0f, 1.0f, 1.0f
#define ARGB_TRANSLUCENT 0.5f, 1.0f, 1.0f, 1.0f

/* RGB Colours*/
#define RGB_BLACK 0.0f, 0.0f, 0.0f
#define RGB_WHITE 1.0f, 1.0f, 1.0f

#define RGB_RED 1.0f, 0.0f, 0.0f
#define RGB_GREEN 0.0f, 1.0f, 0.0f
#define RGB_BLUE 0.0f, 0.0f, 1.0f

#define RGB_CYAN 0.0f, 1.0f, 1.0f
#define RGB_YELLOW 1.0f, 1.0f, 0.0f
#define RGB_MAGENTA 1.0f, 0.0f, 0.0f


class Colours
{
public:
	float RGB_Black[3] = { RGB_BLACK };
	float RGB_White[3] = { RGB_WHITE };
	float RGB_Red[3] = { RGB_RED };
	float RGB_Green[3] = { RGB_GREEN };
	float RGB_Blue[3] = { RGB_BLUE };
	float RGB_Cyan[3] = { RGB_CYAN };
	float RGB_Yellow[3] = { RGB_YELLOW};
	float RGB_Magenta[3] = { RGB_MAGENTA };

	float RGBA_Black[4] = { RGBA_BLACK };
	float RGBA_White[4] = { RGBA_WHITE };
	float RGBA_Red[4] = { RGBA_RED };
	float RGBA_Green[4] = { RGBA_GREEN };
	float RGBA_Blue[4] = { RGBA_BLUE };
	float RGBA_Cyan[4] = { RGBA_CYAN };
	float RGBA_Yellow[4] = { RGBA_YELLOW };
	float RGBA_Magenta[4] = { RGBA_MAGENTA };

	float ARGB_Black[4] = { ARGB_BLACK };
	float ARGB_White[4] = { ARGB_WHITE };
	float ARGB_Red[4] = { ARGB_RED };
	float ARGB_Green[4] = { ARGB_GREEN };
	float ARGB_Blue[4] = { ARGB_BLUE };
	float ARGB_Cyan[4] = { ARGB_CYAN };
	float ARGB_Yellow[4] = { ARGB_YELLOW };
	float ARGB_Magenta[4] = { ARGB_MAGENTA };
};