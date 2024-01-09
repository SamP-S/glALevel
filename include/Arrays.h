#pragma once


void copy_array_from_index(char arr[], char new_arr[], int start_index)
{
	int a = sizeof(*arr);
	int b = sizeof(arr[0]);

	int c = sizeof(*new_arr);
	int d = sizeof(new_arr[0]);

	for (int i = start_index; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		new_arr[i - start_index] = arr[i];
	}
}

/*
When SizeOf() is used in a function on a passed array (Possibly variable, check)
it returns the size of the pointer to the array not the actual size of the array
Pass the size as a parameter as using * to get the value does not work
*/

int find_index_of_first_char(char arr[], char c, int arr_size)
{
	for (int i = 0; i < arr_size; i++)
	{
	if (arr[i] == c)
		{
			return i;
		}
	}
	return -1;
}

bool find_all_of_char(char arr[], char c, int arr_size, std::vector<int> &char_pos_arr)
{

	for (int i = 0; i < arr_size; i++)
	{
		if (arr[i] == c)
		{
			char_pos_arr.push_back(i);
		}
	}

	if (char_pos_arr.size() == 0)
		return false;
	else
		return true;
}

bool find_all_of_char_between(char arr[], char c, int arr_size, int start_pos, int count, std::vector<int> &char_pos_arr)
{
	if (start_pos < 0 || start_pos >= arr_size)
	{
		std::cout << "ERROR: Start Position Out of Bounds\n";
		return false;
	}

	if (start_pos >= arr_size)
	{
		std::cout << "ERROR: Count Out of Bounds\n";
		return false;
	}

	if (start_pos + count > arr_size)
	{
		std::cout << "ERROR: Index Out of Bounds\n";
		return false;
	}

	for (int i = start_pos; i < start_pos + count; i++)
	{
		if (arr[i] == c)
		{
			char_pos_arr.push_back(i);
		}
	}

	return true;
}

bool print_char_array(char arr[], int arr_size, int start_pos, int count)
{
	if (start_pos < 0 || start_pos > arr_size)
	{
		std::cout << "ERROR: Start Position Out of Bounds\n";
		return false;
	}

	if (count <= 0 || count > arr_size)
	{
		std::cout << "ERROR: Count Out of Bounds\n";
		return false;
	}

	if (start_pos + count > arr_size)
	{
		std::cout << "ERROR: Index Out of Bounds\n";
		return false;
	}

	for (int i = start_pos; i < start_pos + count; i++)
	{
		if (arr[i] == '\0')
		{
			return true;
		}
			
		std::cout << arr[i];
	}
		
	return true;
}

// Converts a char array
float charArr_to_float(char arr[], int start_pos, int count)
{
	// Validation
	if (start_pos < 0)
	{
		std::cout << "ERROR: Invalid start position: " << start_pos << std::endl;
		return 0.0f;
	}
	if (count < 0)
	{
		std::cout << "ERROR: Invalid count: " << count << std::endl;
		return 0.0f;
	}
	if (count == 0)
	{
		std::cout << "WARNING: No Texture Data\n";
		return 0.0f;
	}
		
	// Variables
	float f = 0.0f;					// Variable to store final result
	float farr[1024];				// Float array to convert char array to float values
	int decimal_offset = 0;			// Offset to manage index because of decimal point
	int decimalPointIndex = -1;		// Decimal Place index location
	int positive = 1;				// Multiply through at the end and change negative if a negative number
	int positive_offset = 0;
	

	// Convert Char array to float array
	for (int i = 0; i < count; i++)
	{
		char a = arr[start_pos + i];
		if (arr[start_pos + i] == *".")
		{
			if (decimal_offset != -1)
			{
				decimal_offset = -1.0;
				decimalPointIndex = i + positive_offset;
			}
			else
			{
				std::cout << "ERROR: Invalid Number of Decimal Places\n";
				return 0.0f;
			}
		}
		else if (arr[start_pos + i] == *"-")
		{
			if (positive == -1)
			{
				std::cout << "ERROR: Multiple negative signs\n";
				return 0.0f;
			}
			positive = -1;
			positive_offset = -1;
		}
		else
		{
			farr[i + decimal_offset + positive_offset] = (float)(arr[start_pos + i] - '0');
		}
	}

	// Gets size of array with valid numbers
	int farr_size = 0;
	for (int i = 0; i < sizeof(farr) / sizeof(farr[0]); i++)
	{
		if (farr[i] >= 0.0f && farr[i] <= 9.0f)
			farr_size = farr_size + 1;
		else
			break; 
	}

	if (farr_size == 0)
	{
		std::cout << "ERROR: Length of valid digits = 0\n";
		return 0.0f;
	}
	
	// Convert Float array to single float value by multiplying each value by its corresponding position
	float p = 0;
		if (decimalPointIndex == -1) {
			for (int i = 0; i < farr_size; i++)
			{
				if (farr[i] >= 0 && farr[0] <= 9)
				{
					p = farr[i] * powf(10.0f, farr_size -i - 1);
					f = f + p;
				}
			}
		}
		else
		{
			for (int i = 0; i < farr_size; i++)
			{
				p = farr[i] * powf(10.0f, decimalPointIndex - 1 - i);
				f = f + p;
			}
		}
		f = f * positive;
	return f;
};

/* Convert 1D array of chars to integer value */

int charArr_to_int(char arr[], int start_pos, int count)
{
	if (start_pos < 0)
	{
		std::cout << "ERROR: Invalid start position: " << start_pos << std::endl;
		return 0.0f;
	}
	if (count <= 0)
	{
		std::cout << "ERROR: Invalid count: " << count << std::endl;
		return 0.0f;
	}

	int num = 0;					// Variable to store final result
	int iarr[1024];				// Float array to convert char array to float values
	int decimal_offset = 0;			// Offset to manage index because of decimal point
	int decimalPointIndex = -1;		// Decimal Place index location
	int positive = 1;				// Multiply through at the end and change negative if a negative number
	int positive_offset = 0;

	for (int i = 0; i < count; i++)
	{
		char a = iarr[start_pos + i];
		if (arr[start_pos + i] == *".")
		{
			if (decimal_offset != -1)
			{
				decimal_offset = -1.0;
				decimalPointIndex = i;
			}
			else
			{
				std::cout << "ERROR: Invalid Number of Decimal Places\n";
				return 0.0f;
			}
		}
		else if (arr[start_pos + i] == *"-")
		{
			if (positive == -1)
			{
				std::cout << "ERROR: Multiple negative signs\n";
				return 0.0f;
			}
			positive = -1;
			positive_offset = -1;
		}
		else
		{
			iarr[i + decimal_offset + positive_offset] = (float)(arr[start_pos + i] - '0');
		}
	}

	// Gets size of array with valid numbers
	int iarr_size = 0;
	for (int i = 0; i < sizeof(iarr) / sizeof(iarr[0]); i++)
	{
		if (iarr[i] >= 0.0f && iarr[i] <= 9.0f)
			iarr_size = iarr_size + 1;
		else
			break;
	}

	if (iarr_size == 0)
	{
		std::cout << "ERROR: Length of valid digits = 0\n";
		return 0.0f;
	}

	// Convert Float array to single float value by multiplying each value by its corresponding position
	float p = 0;
	if (decimalPointIndex == -1) {
		for (int i = 0; i < iarr_size; i++)
		{
			if (iarr[i] >= 0 && iarr[0] <= 9)
			{
				num = num + pow(iarr[i], iarr_size - i);
			}
		}
	}
	else
	{
		for (int i = 0; i < iarr_size; i++)
		{
			p = iarr[i] * powf(10.0f, decimalPointIndex - 1 - i);
			i = num + p;
		}
	}
	num = num * positive;
	return num;
}

/* Uses previous function ^^^ above to combine into one vec3*/
vec3_t charArr_to_Vec3(char arr[], int arr_size, int start_pos, int count, char seperator)
{

	if (start_pos < 0 || start_pos >= arr_size)
	{
		std::cout << "ERROR: Start Position Out of Bounds\n";
		return vec3(0.0f, 0.0f, 0.0f);
	}

	if (start_pos >= arr_size)
	{
		std::cout << "ERROR: Count Out of Bounds\n";
		return vec3(0.0f, 0.0f, 0.0f);
	}

	if (start_pos + count > arr_size)
	{
		std::cout << "ERROR: Index Out of Bounds\n";
		return vec3(0.0f, 0.0f, 0.0f);
	}

	std::vector<int> char_pos_arr;		// Array for indexs of " " characters
	find_all_of_char_between(arr, seperator, arr_size, start_pos, count, char_pos_arr);

	if (char_pos_arr.size() == 3)
	{
		if (char_pos_arr[0] == start_pos)
		{
			start_pos = start_pos + 1;
			char_pos_arr.erase(char_pos_arr.begin());
		}

		// Parse line into respective float values
		float fx = charArr_to_float(arr, start_pos, char_pos_arr[0] - start_pos);
		float fy = charArr_to_float(arr, char_pos_arr[0] + 1, char_pos_arr[1] - char_pos_arr[0] - 1);
		float fz = charArr_to_float(arr, char_pos_arr[1] + 1, arr_size - char_pos_arr[1] - 1);
		//std::cout << "X: " << fx << "\n" << "Y: " << fy << "\n" << "Z: " << fz << std::endl;
		// Accumulate float values into 3x1 vector then add to verticies array
		return vec3(fx, fy, fz);

	}
	else if (char_pos_arr.size() == 2)
	{
		// Parse line into respective float values
		float fx = charArr_to_float(arr, start_pos, char_pos_arr[0] - start_pos);
		float fy = charArr_to_float(arr, char_pos_arr[0] + 1, char_pos_arr[1] - char_pos_arr[0] - 1);
		float fz = charArr_to_float(arr, char_pos_arr[1] + 1, arr_size - char_pos_arr[1] - 1);
		//std::cout << "X: " << fx << "\n" << "Y: " << fy << "\n" << "Z: " << fz << std::endl;
		// Accumulate float values into 3x1 vector then add to verticies array
		return vec3(fx, fy, fz);
	}
	else if (char_pos_arr.size() == 1)
	{
		float fx = charArr_to_float(arr, start_pos, char_pos_arr[0] - start_pos);
		float fy = charArr_to_float(arr, char_pos_arr[0] + 1, arr_size - char_pos_arr[0] - 1);
		//std::cout << "WARNING: Only 2 attributes per vertex\n";
		return vec3(fx, fy, 0.0f);
	}
	else if (char_pos_arr.size() == 0)
	{
		float fx = charArr_to_float(arr, start_pos, arr_size - start_pos);				// May have some serious indexing issues here. Do some test for it
		//std::cout << "X: " << fx << "\n" << "WARNING: Only 1 attributes per vertex\n";
		return vec3(fx, 0.0f, 0.0f);
	}
	else
	{
		std::cout << "ERROR: Invalid Vertex Data Format => Unexpected Number of Seperators\n";
		return vec3(0.0f, 0.0f, 0.0f);
	}
}