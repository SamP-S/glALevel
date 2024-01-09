#pragma once

#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "3D_Math.h" 
#include "Arrays.h"

/*
Wavefront .obj Files:
They follow a normal standard but can have different data dependant on how the file is saved.
Some will have UV, some will have normal or both.
All, however, must have verticie data as standard.

More Complex items also rely on a heirarchy system of objects and groups:
Objects
	|
Groups (Standard/Smoothing/Merging)
	|
Face Data
	|
Verticies ( / UV / Normal)

I have tried to compensate to make the loader general purpose but expect many files to be unreadable,
especially if using new/advanced techniques
More work needs to be done but should be usable for the time

v = vertex
vt = texture vertex (UV)
vn = normal vertex (Normal)
*/

class Obj
{
private:
	// Types:
	char t_vertex = *"v";
	char t_uv[2] = { *"v", *"t" };
	char t_normal[2] = { *"v", *"n" };
	char t_face = *"f";

	char t_object = *"o";
	char t_group = *"g";
	char t_smooth_group = *"s";
	char t_merge_group[2] = {*"m", *"g" };

	char t_comment = *"#";
	char t_use_material[5] = {*"u", *"s", *"m", *"t", *"l"};

	// Boolean Values set according to data present
	bool bool_v = false;
	bool bool_vt = false;
	bool bool_vn = false;


public:
	// Arrays of Vertex Data
	// Each mesh reuses verticies across groups/objects
	std::vector<vec3_t> verticies;
	std::vector<vec3_t> uvs;
	std::vector<vec3_t> normals;

	std::vector<vec3_t> faces;
	std::vector<std::vector<vec3_t>> groups;
	std::vector<std::vector<std::vector<vec3_t>>> objects;

	std::vector<unsigned int> elementBufferObject_int;

	std::vector<float> verticieBufferObject_float;
	std::vector<float> uvsBufferObject_float;
	std::vector<float> normalsBufferObject_float;


	int num_objects = -1;
	int ebo_size = -1;

	bool ReadOBJ(const char * filePath)
	{

		std::ifstream FILE;		// IN Fie Stream
		FILE.open(filePath);	// Opens file from passed filePath

		std::string string;		// String to read file line-by-line through

		/*
		Line Structure:
		Type [Num] [Num] [Num]
		*/

		// Check file has been found and opened correcty
		if (!FILE.is_open())
		{
			//std::cout << "ERROR: file => " << file.is_open << std::endl;
  			std::cout << "ERROR: Cannot open obj file" << std::endl;
			return false;
		}

		// Loop for every line in the file
		while (std::getline(FILE, string))
		{
			std::cout << string << std::endl;			// Output line read from file

			char line_arr[1024];						// Generally large char array to copy string into
			strcpy(line_arr, string.c_str());			// Copy string into char array
			int size_line = sizeof(line_arr) / sizeof(line_arr[0]); // Size of array for late use

			// line_arr[0] =  type of data on the line

			if (line_arr[0] == t_comment)
			{
				std::cout << "TYPE: Comment - ";
				print_char_array(line_arr, sizeof(line_arr) / sizeof(line_arr[0]), 2, sizeof(line_arr) / sizeof(line_arr[0]) - 2);
				std::cout << std::endl;
			}
			else if (line_arr[0] == t_vertex)
			{
				if (line_arr[1] == *" ")
				{
					//std::cout << "TYPE: Vertex\n";		// Output type
					vec3_t v = charArr_to_Vec3(line_arr, size_line, 2, size_line - 3, *" ");
					verticies.push_back(v);
					bool_v = true;
				}
				else if (line_arr[1] == t_uv[1])
				{
					//std::cout << "TYPE: UV\n";

					vec3_t uv = charArr_to_Vec3(line_arr, size_line, 3, size_line - 4, *" ");
					uvs.push_back(uv);
					bool_vt = true;
				}
				else if (line_arr[1] == t_normal[1])
				{
					//std::cout << "TYPE: Normal\n";
					vec3_t normal = charArr_to_Vec3(line_arr, size_line, 3, size_line - 4, *" ");
					normals.push_back(normal);
					bool_vn = true;
				}
			}

			else if (line_arr[0] == t_face)
			{
				/* Continue
					Indexing issues
				*/
				//std::cout << "TYPE: Face\n";
				std::vector<int> char_pos_arr;
				find_all_of_char(line_arr, ' ', size_line, char_pos_arr);
				int string_size = find_index_of_first_char(line_arr, '\0', size_line);
				vec3_t vertex1 = charArr_to_Vec3(line_arr, size_line, char_pos_arr[0] + 1, char_pos_arr[1] - char_pos_arr[0] - 1, '/');
				vec3_t vertex2 = charArr_to_Vec3(line_arr, size_line, char_pos_arr[1] + 1, char_pos_arr[2] - char_pos_arr[1] - 1, '/');
				vec3_t vertex3 = charArr_to_Vec3(line_arr, size_line, char_pos_arr[2] + 1, string_size - char_pos_arr[2] - 1, '/');

				faces.push_back(vertex1);
				faces.push_back(vertex2);
				faces.push_back(vertex3);

				std::cout << "v: x" << vertex1.x << " y" << vertex1.y << " z" << vertex1.z << std::endl;
				std::cout << "v: x" << vertex2.x << " y" << vertex2.y << " z" << vertex2.z << std::endl;
				std::cout << "v: x" << vertex3.x << " y" << vertex3.y << " z" << vertex3.z << std::endl;

				/* Face format:
				vertexN x=v y=vt z=vn
				*/
			}
			else if (line_arr[0] == t_object)
			{
				//std::cout << "TYPE: Object\n";
				new_object();
			}

			else if (line_arr[0] == t_group)
			{
				//std::cout << "TYPE: Group\n";
					new_group();
			}
			else if (line_arr[0] == t_smooth_group)			
			{
				//std::cout << "TYPE: Smoothing Group\n";
				new_group();
			}			// Add later
			
			else if (line_arr[0] == t_merge_group[0] && line_arr[1] == t_merge_group[1])
			{
				//std::cout << "TYPE: Merging Group\n";
				new_group();
			}			// Add later

			else if (line_arr[0] == *"u" && line_arr[1] == *"s" && line_arr[2] == *"e" && line_arr[3] == *"m" && line_arr[4] == *"t" && line_arr[5] == *"l")
				//std::cout << "TYPE: Use Material\n";			// Add later
			{
			}
			
			else if (line_arr[0] == *"" || line_arr[0] == *" ")
			{
			}
				//std::cout << "TYPE: None\n";
			
			else
				//std::cout << "ERROR: Type Unknown\n";
			{
			}
			
		}

		/* 
		If the object is simple or not made using a mainstream modelling software then
		verticies are not grouped and objects are not defined. This will push the vertex
		data collected into a single object
		*/

		if (verticies.size() == 0)
		{
			std::cout << "ERROR: No Verticie Data\n";
			return false;
		}

		if (uvs.size() == 0)
		{
			std::cout << "WARNING: No UV Data\n";
			bool_vt = false;
		}
		if (normals.size() == 0)
		{
			std::cout << "WARNING: No Normal Data\n";
			bool_vn = false;
		}

		if (objects.size() == 0)
		{
			if (groups.size() == 0)
			{
				if (faces.size() == 0)
				{
					std::cout << "ERROR: I am not to sure how you got here\n Check you have face data\n";
					return false;
				}
				else
				{
					std::cout << "WARNING: No Defined Object(s)\n";
					std::cout << "WARNING: NO Defined Group(s)\n";

					new_group();
					new_object();
				}
			}
			else
			{
				std::cout << "WARNING: No Defined Object(s)\n";
				std::cout << "OK: Defined Group(s)\n";
				new_object();
			}
		}
		else
		{
			std::cout << "OK: Defined Object(s)\n";
			std::cout << "OK: Defined Group(s)\n";
		}

		setup_ebo();

		FILE.close();	// Closes file

		// No Errors - What a Miracle.
		std::cout << "OK: " << filePath << " Loaded" << std::endl;
		return true;
	};

	void new_group()
	{
		if (faces.size() != 0)					// If there has been previous face data
		{
			groups.push_back(faces);			// Pushes data into the group before initialising a new group for the face data following
			faces.clear();
		}
		else
		{
			std::cout << "OK: First Group in object\n";
		}
		std::cout << "OK: New Group\n";
	}

	void new_object()
	{	
		if (groups.size() != 0)					// If there has been a previous group
		{
			objects.push_back(groups);			// Push group into object before creating new object
			groups.clear();
		}
		else
		{
			std::cout << "OK: First Object in Mesh\n";
		}
		std::cout << "OK: New Object\n";
	}

	bool setup_ebo()
	{
		std::vector<std::vector<vec3_t>> group;
		std::vector<vec3_t> face;
		int face_size = 0;

		for (int o = 0; o < objects.size(); o++)
		{
			group = objects[o];

			for (int g = 0; g < group.size(); g++)
			{
				face = group[g];

				if (face.size() % 3 == 1)
				{
					ebo_size = face.size() - 1;
				}
				else if (face.size() % 3 == 2)
				{
					ebo_size = face.size() - 2;
				}
				else
				{
					ebo_size = face.size();
				}
				
				for (int f = 0; f < ebo_size; f++)
				{
					elementBufferObject_int.push_back(face[f].x);
					elementBufferObject_int.push_back(face[f].y);
					//elementBufferObject_float.push_back(face[f].z);
				}
			}
		}

		if (elementBufferObject_int.size() == 0)
		{
			std::cout << "ERROR: EBO Length = 0\n";
			return false;
		}
		else
		{
			std::cout << "OK: EBO created\n";
			return true;
		}
	}
 };