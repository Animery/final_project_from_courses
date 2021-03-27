#version 330 core
precision mediump float;

uniform mat3   u_matrix;
in vec2 a_position;
in vec2 a_tex_coord;
in vec3 a_color;

out vec4   v_color;
out vec2   v_tex_coord;

void           main()
{
    v_tex_coord = a_tex_coord;
    v_color     = vec4(a_color, 1.0);
    vec3 pos    = vec3(a_position, 1.0) * u_matrix;
    gl_Position = vec4(pos, 1.0);
}