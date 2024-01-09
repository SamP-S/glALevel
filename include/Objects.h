
#pragma once

#include <ctime>
#include "Arrays.h"
#include "File.h"
#include "3D_Math.h"
#include "Texture_2D.h"
#include "Timer.h"

class Object
{
private:
	std::vector<vec3_t> vertices;
	std::vector<vec3_t> uvs;
	std::vector<vec3_t> normals;

	GLuint vao;
	GLuint vao_wireframe;
	GLuint ebo;
	GLuint ebo_wireframe;

	GLuint shaderProgramFull;
	GLuint shaderProgramWireframe;

	GLuint buffer_v;
	GLuint buffer_vt;
	GLuint buffer_vn;

	GLuint attrib_v;
	GLuint attrib_vt;
	GLuint attrib_vn;

	GLuint buffer_v_wireframe;

	GLuint attrib_v_wireframe;
	
	GLint uniMatrix;
	GLint uniPersp;
	GLint uniView;
	GLint uniPos;
	GLint uniRot;
	GLint uniScl;
	GLint uniTex;
	GLint uniLightCol;
	GLint uniLightPos;
	GLint uniCameraPos;

	GLint uniMatrix_wireframe;
	GLint uniPersp_wireframe;
	GLint uniView_wireframe;
	GLint uniPos_wireframe;
	GLint uniRot_wireframe;
	GLint uniScl_wireframe;
	GLint uniTex_wireframe;

	std::vector<unsigned int> ebo_v;
	std::vector<unsigned int> ebo_vt;
	std::vector<unsigned int> ebo_vn;

	std::vector<unsigned int> ebo_wireframe_lines;

	Texture_2D tex;
	Timer timer;

	public:
	float x_pos = 0;
	float y_pos = 0;
	float z_pos = 0;

	float x_rot = 0;
	float y_rot = 0;
	float z_rot = 0;

	float x_scl = 1;
	float y_scl = 1;
	float z_scl = 1;

	bool bool_v = false;
	bool bool_vt = false;
	bool bool_vn = false;

	int size = 0;
	clock_t start_time;
	
	/* Returns true if a -> b -> c is clockwise */
	bool clockDir(vec3_t a, vec3_t b, vec3_t c)
	{
		vec3_t n = v3_Cross(a, c);
		mat3_t equ = mat3
		(
			n.x, a.x, c.x,
			n.y, a.y, c.y,
			n.z, a.z, c.z
		);
		vec3_t ans = m3_MulV(m3_Inverse(equ), b);
		
		if (ans.x >= 0)
		{
			return true;	// Clockwise
		}
		else
		{
			return false;	// Anti-Clockwise
		}
	}

	void calculateSmoothNormals()
	{
		vec3_t a;
		vec3_t b;
		vec3_t c;

		vec3_t normal;

		normals.clear();
		ebo_vn.clear();

		std::vector<vec3_t> faceNormals;
		for (int i = 0; i < ebo_v.size(); i += 3)
		{
			a = vertices[ebo_v[i]];
			b = vertices[ebo_v[i + 1]];
			c = vertices[ebo_v[i + 2]];

			normal = v3_Cross(v3_Sub(a, b), v3_Sub(b, c));
			normal = v3_Normalise(normal);

			faceNormals.push_back(normal);
		}

		normals.resize(vertices.size());

		for (int i = 0; i < ebo_v.size(); i++)
		{
			normals[ebo_v[i]] = v3_Add(normals[ebo_v[i]], faceNormals[floor(i / 3)]);
		}

		for (int i = 0; i < normals.size(); i++)
		{
			normals[i] = v3_Normalise(normals[i]);
		}

		ebo_vn = ebo_v;


		std::cout << "OK: Normals Calculated\n";
	}

	void calculateNormals()
	{
		vec3_t a;
		vec3_t b;
		vec3_t c;

		vec3_t normal;

		bool clockwise;

		ebo_vn.clear();

		for (int i = 0; i < ebo_v.size(); i += 3)
		{
			a = vertices[ebo_v[i]];
			b = vertices[ebo_v[i + 1]];
			c = vertices[ebo_v[i + 2]];

			clockwise = clockDir(a, b, c);
			
			normal = v3_Cross(v3_Sub(a, b), v3_Sub(b, c));
			normal = v3_Normalise(normal);
			//if (clockwise)
			//normal = v3_MulS(normal, -1);

			normals.push_back(normal);

			for (int n = 0; n < 3; n++)
			{
				ebo_vn.push_back(i / 3);
			}
		}

		std::cout << "OK: Normals Generated\n";
	}
	
	// Loading
	bool load(const char * meshLocation, const char * textureLocation)
	{
		std::vector<std::string> mesh_file = loadFile(meshLocation);
		loadMesh(mesh_file);
		tex.SetupTexture(textureLocation);

		/*
			If the object is simple or not made using a mainstream modelling software then
			vertices are not grouped and objects are not defined. This will push the vertex
			data collected into a single object
		*/

		if (vertices.size() == 0)
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
			bool_vn = true;
			calculateSmoothNormals();
		}

		std::cout << "OK: Mesh successfully loaded\n";

	}

	void loadMesh(std::vector<std::string> file_string)
	{
		for (int i = 0; i < file_string.size(); i++)
		{
			char line_arr[1024];
			int arr_size;
			int start = file_string[i].find_first_not_of(' ');
			int end = file_string[i].find_last_not_of(' ');

			if (!(start == -1 || end == -1))
			{
				file_string[i] = file_string[i].substr(start, end - start + 1);
				// Generally large char array to copy string into
				strcpy(line_arr, file_string[i].c_str());		// Copy string into char array
				arr_size = sizeof(line_arr) / sizeof(line_arr[0]); // Size of array for late use
			}
			else
			{
				arr_size = 0;
			}

			int string_size = file_string[i].size();

			//std::cout << line_arr << std::endl;

			// line_arr[0] =  type of data on the line
			if (arr_size == 0)
			{

			}
			else if (line_arr[0] == '#')
			{
				std::cout << "TYPE: Comment - ";
				print_char_array(line_arr, sizeof(line_arr) / sizeof(line_arr[0]), 1, sizeof(line_arr) / sizeof(line_arr[0]) - 2);
				std::cout << std::endl;
			}
			else if (line_arr[0] == 'v')
			{
				if (line_arr[1] == ' ')
				{
					//std::cout << "TYPE: Vertex\n";		// Output type
					vec3_t v = charArr_to_Vec3(line_arr, string_size, 2, string_size - 3, ' ');
					vertices.push_back(v);
					bool_v = true;
				}
				else if (line_arr[1] == 't')
				{
					//std::cout << "TYPE: UV\n";

					vec3_t uv = charArr_to_Vec3(line_arr, string_size, 3, string_size - 4, ' ');
					uvs.push_back(uv);
					bool_vt = true;
				}
				else if (line_arr[1] == 'n')
				{
					//std::cout << "TYPE: Normal\n";
					vec3_t normal = charArr_to_Vec3(line_arr, string_size, 3, string_size - 4, ' ');
					normals.push_back(normal);
					bool_vn = true;
				}
			}

			else if (line_arr[0] == 'f')
			{
				/* Face format:
				f v1 v2 v3
				f v1/vt1 v2/vt2 v3/vt3
				f v1//vn1 v2//vn2 v3//vn3
				
				Can have 4th point to draw square

				face x=v y=vt z=vn

				*/
				//::cout << "TYPE: Face\n";
				
				std::vector<int> char_pos_arr;
				find_all_of_char(line_arr, ' ', string_size, char_pos_arr);
				
				/*
				Make a seperate routine to read a single face and return the data
				*/

				vec3_t f1 = charArr_to_Vec3(line_arr, string_size, char_pos_arr[0] + 1, char_pos_arr[1] - char_pos_arr[0] - 1, '/');
				vec3_t f2 = charArr_to_Vec3(line_arr, string_size, char_pos_arr[1] + 1, char_pos_arr[2] - char_pos_arr[1] - 1, '/');
				vec3_t f3;
				vec3_t f4;


				if (char_pos_arr.size() == 3)
				{
					f3 = charArr_to_Vec3(line_arr, string_size, char_pos_arr[2] + 1, string_size - char_pos_arr[2] - 1, '/');

					// "-1" needed to start indexing at 0

					ebo_v.push_back(f1.x - 1);
					ebo_vt.push_back(f1.y - 1);
					ebo_vn.push_back(f1.z - 1);

					ebo_v.push_back(f2.x - 1);
					ebo_vt.push_back(f2.y - 1);
					ebo_vn.push_back(f2.z - 1);

					ebo_v.push_back(f3.x - 1);
					ebo_vt.push_back(f3.y - 1);
					ebo_vn.push_back(f3.z - 1);

					size = size + 1;
				}
				else if (char_pos_arr.size() == 4)
				{
					f3 = charArr_to_Vec3(line_arr, string_size, char_pos_arr[2] + 1, char_pos_arr[3] - char_pos_arr[2] - 1, '/');
					f4 = charArr_to_Vec3(line_arr, string_size, char_pos_arr[3] + 1, string_size - char_pos_arr[3] - 1, '/');

					// "-1" needed to start indexing at 0

					ebo_v.push_back(f1.x - 1);
					ebo_vt.push_back(f1.y - 1);
					ebo_vn.push_back(f1.z - 1);

					ebo_v.push_back(f2.x - 1);
					ebo_vt.push_back(f2.y - 1);
					ebo_vn.push_back(f2.z - 1);

					ebo_v.push_back(f3.x - 1);
					ebo_vt.push_back(f3.y - 1);
					ebo_vn.push_back(f3.z - 1);

					ebo_v.push_back(f1.x - 1);
					ebo_vt.push_back(f1.y - 1);
					ebo_vn.push_back(f1.z - 1);

					ebo_v.push_back(f3.x - 1);
					ebo_vt.push_back(f3.y - 1);
					ebo_vn.push_back(f3.z - 1);
					
					ebo_v.push_back(f4.x - 1);
					ebo_vt.push_back(f4.y - 1);
					ebo_vn.push_back(f4.z - 1);

					size = size + 2;
				}
				else
				{
					std::cout << "ERROR: Invalid number of vertices to make a primative" << "\n";
				}


			}
			else if (line_arr[0] == 'o')
			{
				std::cout << "TYPE: Object\n";
			}
			else if (line_arr[0] == 'g')
			{
				std::cout << "TYPE: Group\n";
			}
			else if (line_arr[0] == 's')
			{
				std::cout << "TYPE: Smoothing Group\n";
			}			// Add later

			else if (line_arr[0] == 'm' && line_arr[1] == 'g')
			{
				std::cout << "TYPE: Merging Group\n";
			}			// Add later

			else if (line_arr[0] == 'u' && line_arr[1] == 's' && line_arr[2] == 'e' && line_arr[3] == 'm' && line_arr[4] == 't' && line_arr[5] == 'l')
			{
				std::cout << "TYPE: Use Material\n";			// Add later
			}
			else if (line_arr[0] == *"" || line_arr[0] == ' ')
			{
				std::cout << "TYPE: None\n";
			}
			else
			{
				std::cout << "ERROR: Type Unknown\n";
			}
		}

		vertices.shrink_to_fit();
		uvs.shrink_to_fit();
		normals.shrink_to_fit();

		checkEBO();

		std::cout << "OK: Data Successfully Loaded\n";

	}

	void loadWireframeLines()
	{
		ebo_wireframe_lines.resize(ebo_v.size() * 4 / 3);
		for (int face = 0; face < ebo_v.size() / 3; face++)
		{ 
			ebo_wireframe_lines[face * 4] = ebo_v[face * 3];
			ebo_wireframe_lines[face * 4 + 1] = ebo_v[face * 3 + 1];
			ebo_wireframe_lines[face * 4 + 2] = ebo_v[face * 3 + 1];
			ebo_wireframe_lines[face * 4 + 3] = ebo_v[face * 3 + 2];
		}
		std::cout << "OK: EBO Wireframe Lines Loaded\n";
	}

	// Setup

	void checkEBO()
	{

		bool face_v = false;
		bool face_vt = false;
		bool face_vn = false;

		for (int i = 0; i < ebo_v.size(); i++)
		{
			if (ebo_v[i] > 0)
			{
				face_v = true;
			}

			if (ebo_vt[i] > 0)
			{
				face_vt = true;
			}

			if (ebo_vn[i] > 0)
			{
				face_vn = true;
			}
		}

		if (!face_v)
		{
			ebo_v.clear();
		}

		if (!face_vt)
		{
			ebo_vt.clear();
		}

		if (!face_vn)
		{
			ebo_vn.clear();
		}

		if (bool_v && !face_v)
		{
			bool_v = false;
		}

		if (bool_vt && !face_vt)
		{
			bool_vt = false;
		}

		if (bool_vn && !face_vn)
		{
			bool_vn = false;
		}

		std::cout << "OK: EBOs Successfully checked\n";

	}

	void bind_shader_program(GLuint shader_program_full, GLuint shader_program_wireframe)
	{
		shaderProgramFull = shader_program_full;
		shaderProgramWireframe = shader_program_wireframe;
		glUseProgram(shaderProgramFull);
	}

	void scale(float f)
	{
		if (f <= 0)
		{
			std::cout << "Scale too small: " << f << " <= 0\n";
		}
		else
		{
			x_scl = f;
			y_scl = f;
			z_scl = f;
		}
	}

	void position(float x, float y, float z)
	{
		x_pos = x;
		y_pos = y;
		z_pos = z;
	}

	void rotation(float x, float y, float z)
	{
		x_rot = x;
		y_rot = y;
		z_rot = z;
	}

	void setup()
	{
		// Create a Vertex Buffer Object for each Data Type
		// Verticies
		glGenBuffers(1, &buffer_v);
		glBindBuffer(GL_ARRAY_BUFFER, buffer_v);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3_t), &vertices[0], GL_STATIC_DRAW);
		std::cout << "Verticie Buffer: ";
		ErrorCheck(); // Test

		// Texture 2D
		tex.SetupTexture("C:/Users/Sam/source/repos/OpenGL_C++/OpenGL_0.6/OpenGL_0.6/images/gay.bmp");

		if (bool_vt)
		{
			// UVs
			glGenBuffers(1, &buffer_vt);
			glBindBuffer(GL_ARRAY_BUFFER, buffer_vt);
			glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec3_t), &uvs[0], GL_STATIC_DRAW);
			std::cout << "UV Buffer: ";
			ErrorCheck(); // Test
		}

		if (bool_vn)
		{
			// Normals
			glGenBuffers(1, &buffer_vn);
			glBindBuffer(GL_ARRAY_BUFFER, buffer_vn);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3_t), &normals[0], GL_STATIC_DRAW);
			std::cout << "Normal Buffer: ";
			ErrorCheck(); // Test
		}

		// Create Vertex Array Object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		std::cout << "Vertex Array Object: ";
		ErrorCheck();

		/* Attributes */
		// Specify the layout of the vertex data
		// Binds verticie buffer as active buffer then
		// Binds position attribute to active buffer

		glBindBuffer(GL_ARRAY_BUFFER, buffer_v);
		attrib_v = glGetAttribLocation(shaderProgramFull, "v_position");
		glEnableVertexAttribArray(attrib_v);
		glVertexAttribPointer(attrib_v, 3, GL_FLOAT, GL_FALSE, 0, 0);
		std::cout << "Position Attribute: ";
		ErrorCheck();

		if (bool_vt)
		{
			glBindBuffer(GL_ARRAY_BUFFER, buffer_vt);
			attrib_vt = glGetAttribLocation(shaderProgramFull, "v_texture_coord");
			glEnableVertexAttribArray(attrib_vt);
			glVertexAttribPointer(attrib_vt, 3, GL_FLOAT, GL_FALSE, 0, 0);
			//glActiveTexture(GL_TEXTURE0);
			std::cout << "Texture Attribute: ";
			ErrorCheck();
		}

		glBindBuffer(GL_ARRAY_BUFFER, buffer_vn);
		attrib_vn = glGetAttribLocation(shaderProgramFull, "v_normal_coord");
		glEnableVertexAttribArray(attrib_vn);
		glVertexAttribPointer(attrib_vn, 3, GL_FLOAT, GL_FALSE, 0, 0);
		std::cout << "Normal Attribute: ";
		ErrorCheck();
		

		/* Element Buffer (Index Buffer) */
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		std::cout << ebo_v.size() * sizeof(unsigned int) << std::endl;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_v.size() * sizeof(unsigned int), &ebo_v[0], GL_STATIC_DRAW);
		std::cout << "Element Buffer Object: ";
		ErrorCheck();

		/* Uniforms */
		uniMatrix = glGetUniformLocation(shaderProgramFull, "matrix");
		ErrorCheck();

		uniPersp = glGetUniformLocation(shaderProgramFull, "persp");
		ErrorCheck();

		uniView = glGetUniformLocation(shaderProgramFull, "view");
		ErrorCheck();

		uniPos = glGetUniformLocation(shaderProgramFull, "obj_position");
		ErrorCheck();

		uniRot = glGetUniformLocation(shaderProgramFull, "obj_rotation");
		ErrorCheck();

		uniScl = glGetUniformLocation(shaderProgramFull, "obj_scale");
		ErrorCheck();

		uniLightCol = glGetUniformLocation(shaderProgramFull, "lightColour");
		ErrorCheck();

		uniLightPos = glGetUniformLocation(shaderProgramFull, "lightPos");
		ErrorCheck();

		uniCameraPos = glGetUniformLocation(shaderProgramFull, "cameraPos");
		ErrorCheck();

		if (bool_vt)
		{
			uniTex = glGetUniformLocation(shaderProgramFull, "tex");
		}

		/* Timer */
		start_time = clock();

		std::cout << "OK: Object Successfully setup\n";

		timer.StartTimer();
	}

	void setupWireframe()
	{
		// Wireframe Vertices
		glGenBuffers(1, &buffer_v_wireframe);
		glBindBuffer(GL_ARRAY_BUFFER, buffer_v_wireframe);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3_t), &vertices[0], GL_STATIC_DRAW);
		std::cout << "Verticie Buffer: ";
		ErrorCheck(); // Test

		/* Vertex Array Object -> Wireframe */
		glGenVertexArrays(1, &vao_wireframe);
		glBindVertexArray(vao_wireframe);
		std::cout << "Vertex Array Object: ";
		ErrorCheck();

		/* Attributes */
		glBindBuffer(GL_ARRAY_BUFFER, buffer_v_wireframe);
		attrib_v_wireframe = glGetAttribLocation(shaderProgramWireframe, "v_position_wireframe");
		ErrorCheck();
		glEnableVertexAttribArray(attrib_v_wireframe);
		ErrorCheck();
		glVertexAttribPointer(attrib_v_wireframe, 3, GL_FLOAT, GL_FALSE, 0, 0);
		std::cout << "Position Attribute: ";
		ErrorCheck();

		loadWireframeLines();

		/* Element Buffer (Index Buffer) */
		glGenBuffers(1, &ebo_wireframe);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_wireframe);
		std::cout << "EBO Wireframe Lines Size: " << ebo_wireframe_lines.size() * sizeof(unsigned int) << std::endl;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_wireframe_lines.size() * sizeof(unsigned int), &ebo_wireframe_lines[0], GL_STATIC_DRAW);
		std::cout << "Element Buffer Object: ";
		ErrorCheck();

		/* Uniforms Wireframe */
		uniMatrix_wireframe = glGetUniformLocation(shaderProgramWireframe, "matrix");
		ErrorCheck();
		uniPersp_wireframe = glGetUniformLocation(shaderProgramWireframe, "persp");
		ErrorCheck();
		uniView_wireframe = glGetUniformLocation(shaderProgramWireframe, "view");
		ErrorCheck();
		uniPos_wireframe = glGetUniformLocation(shaderProgramWireframe, "obj_position");
		ErrorCheck();
		uniRot_wireframe = glGetUniformLocation(shaderProgramWireframe, "obj_rotation");
		ErrorCheck();
		uniScl_wireframe = glGetUniformLocation(shaderProgramWireframe, "obj_scale");
		ErrorCheck();
	}

	// Rendering

	void render(Camera camera)
	{
		/* Time */
		float diff_time = (clock() - start_time);
		diff_time = diff_time / 100;

// Use program
// Set uniforms
// Bind vao
// Bind textures
// Dont re bind vbo?
// Settings e.g. culling
// Draw elements


		// Set Shader Program
		glUseProgram(shaderProgramFull);
		// Bind VAO
		glBindVertexArray(vao);

		/* Uniforms */
		glUniformMatrix4fv(uniMatrix, 1, GL_TRUE, &camera.modelTransformationMatrix.m[0][0]);
		glUniformMatrix4fv(uniPersp, 1, GL_TRUE, &camera.perspectiveProjectionMatrix.m[0][0]);
		glUniformMatrix4fv(uniView, 1, GL_TRUE, &camera.cameraViewMatrix.m[0][0]);

		glUniform3f(uniPos, x_pos, y_pos, z_pos);
		glUniform3f(uniRot, x_rot, y_rot, z_rot);
		glUniform3f(uniScl, x_scl, y_scl, z_scl);

		glUniform3f(uniLightCol, 1.0f, 1.0f, 1.0f);
		glUniform3f(uniLightPos, 0.0f, 0.0f, 0.0f);
		glUniform3f(uniCameraPos, camera.cameraFrom.x, camera.cameraFrom.y, camera.cameraFrom.z);

		/* Draw */
		glDrawElements(GL_TRIANGLES, ebo_v.size(), GL_UNSIGNED_INT, NULL);


		glUseProgram(shaderProgramWireframe);
		glBindVertexArray(vao_wireframe);

		glUniformMatrix4fv(uniMatrix_wireframe, 1, GL_TRUE, &camera.modelTransformationMatrix.m[0][0]);
		glUniformMatrix4fv(uniPersp_wireframe, 1, GL_TRUE, &camera.perspectiveProjectionMatrix.m[0][0]);
		glUniformMatrix4fv(uniView_wireframe, 1, GL_TRUE, &camera.cameraViewMatrix.m[0][0]);

		glUniform3f(uniPos_wireframe, x_pos, y_pos, z_pos);
		glUniform3f(uniRot_wireframe, x_rot, y_rot, z_rot);
		glUniform3f(uniScl_wireframe, x_scl, y_scl, z_scl);

		glLineWidth(1);
		glDrawElements(GL_LINES, ebo_wireframe_lines.size(), GL_UNSIGNED_INT, NULL);

		glPointSize(1);
		glDrawArrays(GL_POINTS, 0, vertices.size());
	}

	void delete_buffer_resources()
	{
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &ebo_wireframe);
		glDeleteVertexArrays(1, &vao);
		glDeleteVertexArrays(1, &vao_wireframe);
	}

};

/* Atttributes
// Vertex
glBindBuffer(GL_ARRAY_BUFFER, buffer_v);
glEnableVertexAttribArray(attrib_v);
glVertexAttribPointer(attrib_v, 3, GL_FLOAT, GL_FALSE, 0, 0);

// Texture
glBindBuffer(GL_ARRAY_BUFFER, buffer_vt);
glEnableVertexAttribArray(attrib_vt);
glVertexAttribPointer(attrib_vt, 3, GL_FLOAT, GL_FALSE, 0, 0);

// Normal
glBindBuffer(GL_ARRAY_BUFFER, buffer_vn);
glEnableVertexAttribArray(attrib_vn);
glVertexAttribPointer(attrib_vn, 3, GL_FLOAT, GL_FALSE, 0, 0);
*/

/*
	void calculateNormals()
	{
		vec3_t a;
		vec3_t b;
		vec3_t c;

		vec3_t normalA;
		vec3_t normalB;
		vec3_t normalC;

		vec3_t normal;

		ebo_vn.clear();

		for (int i = 0; i < ebo_v.size(); i += 3)
		{
			a = vertices[ebo_v[i]];
			b = vertices[ebo_v[i + 1]];
			c = vertices[ebo_v[i + 2]];

			bool clockwise = clockDir(a, b, c);

			normalA = v3_Cross(v3_Sub(c, a), v3_Sub(a, b));
			normalB = v3_Cross(v3_Sub(a, b), v3_Sub(b, c));
			normalC = v3_Cross(v3_Sub(b, c), v3_Sub(c, a));

			normal = v3_Add(normalA, normalB, normalC);
			normal = v3_Normalise(normal);

			if(clockwise)
			{
				normal = v3_MulS(normal, -1);
			}

			normals.push_back(normal);

			for (int n = 0; n < 3; n++)
			{
				ebo_vn.push_back(i / 3);
			}
		}
		std::cout << "OK: Normals Generated\n";
	}

	*/

/*void calculateSmoothNormals()
	{
		vec3_t a;
		vec3_t b;
		vec3_t c;

		vec3_t normal;

		normals.clear();
		ebo_vn.clear();

		std::vector<vec3_t> faceNormals;
		for (int i = 0; i < ebo_v.size(); i += 3)
		{
			a = vertices[ebo_v[i]];
			b = vertices[ebo_v[i + 1]];
			c = vertices[ebo_v[i + 2]];

			normal = v3_Cross(v3_Sub(a, b), v3_Sub(b, c));
			normal = v3_Normalise(normal);

			faceNormals.push_back(normal);
		}

		for (int i = 0; i < vertices.size(); i++)
		{
			normal = vec3(0, 0, 0);
			for (int f = 0; f < ebo_v.size(); f++)
			{
				if (i == ebo_v[f])
				{
					normal = v3_Add(normal, faceNormals[floor(f / 3)]);
				}
			}
			normal = v3_Normalise(normal);
			normals.push_back(normal);
		}

		ebo_vn = ebo_v;


		std::cout << "OK: Normals Calculated\n";
	}*/
