#pragma once

/* Vertex Sourcea */
// Full lighting
const char* vs_full = R"glsl(
    #version 150 core
	
    in vec3 v_position;
	in vec3 v_texture_coord;
	in vec3 v_normal_coord;

	out vec3 f_pos;
	out vec3 f_texture_coord;
	out vec3 f_normal_coord;

	out vec3 f_colour;
	

	uniform mat4 matrix;
	uniform mat4 persp;
	uniform mat4 view;

	uniform vec3 obj_position;
	uniform vec3 obj_rotation;
	uniform vec3 obj_scale;

    void main()
    {
        f_colour = v_position;
		mat3 rot_x = mat3(1, 0, 0, 0, cos(obj_rotation.x), sin(obj_rotation.x), 0, -sin(obj_rotation.x), cos(obj_rotation.x));
		mat3 rot_y = mat3(cos(obj_rotation.y), 0, -sin(obj_rotation.y), 0, 1, 0, sin(obj_rotation.y), 0, cos(obj_rotation.y));
		mat3 rot_z = mat3(cos(obj_rotation.z), sin(obj_rotation.z), 0, -sin(obj_rotation.z), cos(obj_rotation.z), 0, 0, 0, 1);
		vec3 new_pos =  v_position * rot_x * rot_y * rot_z * obj_scale + obj_position;
        gl_Position = matrix * persp * view * vec4(new_pos, 1.0);
		f_pos = new_pos;
		f_texture_coord = v_texture_coord;
		f_normal_coord = v_normal_coord * rot_x * rot_y * rot_z;
    }
)glsl";

/* Fragment Sources */
// Full - v, vt, vn
const char* fs_full = R"glsl(
    #version 150 core
	
    in vec3 f_colour;
	in vec3 f_texture_coord;
	in vec3 f_normal_coord;

	in vec3 f_pos;

    out vec4 o_colour;

	uniform sampler2D tex;
	uniform mat4 matrix;

	uniform vec3 obj_rotation;

	uniform vec3 lightColour;
	uniform vec3 lightPos;

	uniform vec3 cameraPos;

    void main()
    {
		// Rotation
		mat3 rot_x = mat3(1, 0, 0, 0, cos(obj_rotation.x), sin(obj_rotation.x), 0, -sin(obj_rotation.x), cos(obj_rotation.x));
		mat3 rot_y = mat3(cos(obj_rotation.y), 0, -sin(obj_rotation.y), 0, 1, 0, sin(obj_rotation.y), 0, cos(obj_rotation.y));
		mat3 rot_z = mat3(cos(obj_rotation.z), sin(obj_rotation.z), 0, -sin(obj_rotation.z), cos(obj_rotation.z), 0, 0, 0, 1);

		// Ambient
		float ambientStrength = 0.05;
		vec3 ambient = ambientStrength * lightColour;

		// Diffuse
		vec3 normal = normalize(f_normal_coord);

		// Calculate Positions
		vec3 fragPos = f_pos;
		vec3 lightDirection = lightPos - fragPos;
		
		// Calculate angle of incidence
		float brightness = dot(normal, lightDirection) / (length(lightDirection) * length(normal));
		brightness = clamp(brightness, 0, 1);

		vec4 surfaceColour = texture(tex, f_texture_coord.xy);
		o_colour =  vec4(brightness * lightColour * surfaceColour.rgb, surfaceColour.a);
		//o_colour = surfaceColour;
    }
)glsl";


// For wireframe mode
const char* vs_wireframe = R"glsl(
    #version 150 core
	
    in vec3 v_position_wireframe;

	uniform mat4 matrix;
	uniform mat4 persp;
	uniform mat4 view;

	uniform vec3 obj_position;
	uniform vec3 obj_rotation;
	uniform vec3 obj_scale;

    void main()
    {
		mat3 rot_x = mat3(1, 0, 0, 0, cos(obj_rotation.x), sin(obj_rotation.x), 0, -sin(obj_rotation.x), cos(obj_rotation.x));
		mat3 rot_y = mat3(cos(obj_rotation.y), 0, -sin(obj_rotation.y), 0, 1, 0, sin(obj_rotation.y), 0, cos(obj_rotation.y));
		mat3 rot_z = mat3(cos(obj_rotation.z), sin(obj_rotation.z), 0, -sin(obj_rotation.z), cos(obj_rotation.z), 0, 0, 0, 1);
		vec3 new_pos =  v_position_wireframe * rot_x * rot_y * rot_z * obj_scale + obj_position;
        gl_Position = matrix * persp * view * vec4(new_pos, 1.0);
    }
)glsl";


const char* fs_wireframe = R"glsl(
    #version 150 core

    out vec4 o_colour;

    void main()
    {
		o_colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
)glsl";

// For the GUI
const char* vs_gui = R"glsl(
    #version 150 core

    in vec3 v_position_GUI;

    void main()
    {
	gl_Position = vec4(v_position_GUI, 1.0);
    }
)glsl";


const char* fs_gui = R"glsl(
    #version 150 core

    out vec4 o_colour;

	uniform vec3 colour;

    void main()
    {
		o_colour = vec4(colour, 1.0);
    }
)glsl";


/* Vertex 
// Base - v
const char* vs_base = R"glsl(
    #version 150 core
	
    in vec3 v_position;

	out vec3 f_colour;

	uniform mat4 matrix;
	uniform mat4 persp;
	uniform mat4 view;

	uniform vec3 obj_position;
	uniform vec3 obj_rotation;
	uniform vec3 obj_scale;

    void main()
    {
        f_colour = v_position;
		mat3 rot_x = mat3(1, 0, 0, 0, cos(obj_rotation.x), sin(obj_rotation.x), 0, -sin(obj_rotation.x), cos(obj_rotation.x));
		mat3 rot_y = mat3(cos(obj_rotation.y), 0, -sin(obj_rotation.y), 0, 1, 0, sin(obj_rotation.y), 0, cos(obj_rotation.y));
		mat3 rot_z = mat3(cos(obj_rotation.z), sin(obj_rotation.z), 0, -sin(obj_rotation.z), cos(obj_rotation.z), 0, 0, 0, 1);
		vec3 new_pos =  v_position * rot_x * rot_y * rot_z * obj_scale + obj_position;
        gl_Position = matrix * persp * view * vec4(new_pos, 1.0);
    }
)glsl";


// Texture - v, vt
const char* vs_text = R"glsl(
    #version 150 core
	
    in vec3 v_position;
	in vec3 v_texture_coord;

	out vec3 f_colour;
	out vec3 f_texture_coord;

	uniform mat4 matrix;
	uniform mat4 persp;
	uniform mat4 view;

	uniform vec3 obj_position;
	uniform vec3 obj_rotation;
	uniform vec3 obj_scale;

    void main()
    {
        f_colour = vec3(1.0, 1.0, 1.0);
		mat3 rot_x = mat3(1, 0, 0, 0, cos(obj_rotation.x), sin(obj_rotation.x), 0, -sin(obj_rotation.x), cos(obj_rotation.x));
		mat3 rot_y = mat3(cos(obj_rotation.y), 0, -sin(obj_rotation.y), 0, 1, 0, sin(obj_rotation.y), 0, cos(obj_rotation.y));
		mat3 rot_z = mat3(cos(obj_rotation.z), sin(obj_rotation.z), 0, -sin(obj_rotation.z), cos(obj_rotation.z), 0, 0, 0, 1);
		vec3 new_pos =  v_position * rot_x * rot_y * rot_z * obj_scale + obj_position;
        gl_Position = matrix * persp * view * vec4(new_pos, 1.0);
		f_texture_coord = v_texture_coord;
    }
)glsl";

// Normal - v, vn
const char* vs_norm = R"glsl(
    #version 150 core
	
    in vec3 v_position;
	in vec3 v_normal_coord;

	out vec3 f_colour;
	out vec3 f_normal_coord;

	uniform mat4 matrix;
	uniform mat4 persp;
	uniform mat4 view;

	uniform vec3 obj_position;
	uniform vec3 obj_rotation;
	uniform vec3 obj_scale;

    void main()
    {
        f_colour = v_position;
		mat3 rot_x = mat3(1, 0, 0, 0, cos(obj_rotation.x), sin(obj_rotation.x), 0, -sin(obj_rotation.x), cos(obj_rotation.x));
		mat3 rot_y = mat3(cos(obj_rotation.y), 0, -sin(obj_rotation.y), 0, 1, 0, sin(obj_rotation.y), 0, cos(obj_rotation.y));
		mat3 rot_z = mat3(cos(obj_rotation.z), sin(obj_rotation.z), 0, -sin(obj_rotation.z), cos(obj_rotation.z), 0, 0, 0, 1);
		vec3 new_pos =  v_position * rot_x * rot_y * rot_z * obj_scale + obj_position;
        gl_Position = matrix * persp * view * vec4(new_pos, 1.0);
		f_normal_coord = v_normal_coord;
    }
)glsl";
*/

/* Fragment
// Base - v
const char* fs_base = R"glsl(
	#version 150 core

	in vec3 f_colour;

	out vec4 o_colour;

	void main()
	{
	  o_colour = vec4(f_colour, 1.0);
	}
)glsl";

// Texture - v, vt
const char* fs_text = R"glsl(
	#version 150 core

	in vec3 f_colour;
	in vec3 f_texture_coord;

	out vec4 o_colour;

	uniform sampler2D tex;

	uniform vec3 lightColour;

	void main()
	{
		float ambientStrength = 0.1;
		o_colour = texture(tex, f_texture_coord.xy);
	}
)glsl";

// Normal - v, vn
const char* fs_norm = R"glsl(
	#version 150 core

	in vec3 f_colour;
	in vec3 f_normal_coord;

	out vec4 o_colour;

	void main()
	{
	  o_colour = vec4(f_normal_coord, 1.0);
	}
)glsl";
*/