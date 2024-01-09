#include <iostream>
#include <thread>

#include "GLFW/glfw3.h"

int main()
{
    std::cout << "Hello World!\n"; 
	int a = glfwInit();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	glfwTerminate();
	std::cout << a << std::endl;
}