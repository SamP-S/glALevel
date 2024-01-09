#pragma once

#include <ctime>

class FrameRate
{
private:
	std::clock_t start_time;

public:
	double avg_rate = 0;
	int frame_num = 0;

	void start()
	{
		start_time = std::clock();
	}

	double getDuration()
	{
		return (std::clock() - start_time) / (double) CLOCKS_PER_SEC;
	}

	void frame()
	{
		frame_num =  frame_num + 1;
		avg_rate = frame_num / getDuration();
	}
};