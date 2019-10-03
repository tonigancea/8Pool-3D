#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float clock;

out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;

void main()
{
	
	float offsetX = mod(clock, 5);

	vec3 new_poz = vec3(v_position.x + offsetX, v_position.y, v_position.z);

	frag_normal = v_normal;
	frag_color = v_color;
	tex_coord = v_texture_coord;
	gl_Position = Projection * View * Model * vec4(new_poz, 1.0);
}
