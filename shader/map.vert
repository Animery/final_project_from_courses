#version 330 core

#ifdef GL_ES
precision mediump float;
#endif
// precision mediump float;

uniform mat3 u_matrix;
in vec2      a_position;
in vec2      a_tex_coord;
// in vec3 a_color;

// out vec4   v_color;
out vec2 v_tex_coord;

void main()
{
    vec3 pos    = vec3(a_position, 1.0) * u_matrix;
    gl_Position = vec4(pos, 1.0);
    v_tex_coord = a_tex_coord;
    // v_tex_coord = gl_Vertex.xy;
}