#pragma once

struct RGB
{
	UINT16 r;
	UINT16 g;
	UINT16 b;
};

RGB rgb(UINT16 r, UINT16 g, UINT16 b)
{
	RGB c;
	c.r = r;
	c.g = g;
	c.b = b;
	return c;
}

RGB inverseColour(RGB c)
{
	return rgb(255 - c.r, 255 - c.g, 255 - c.b);
}

RGB proportionateColour(RGB c, float f)	// change colour brightness according to float value: 1.0 = same; 1.1 = +10%; 0.9 = -10%;
{
	return rgb(c.r * f, c.g * f, c.b * f);
}