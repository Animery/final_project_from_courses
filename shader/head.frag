#version 330 core
#ifdef GL_ES
precision mediump float;
#endif

in vec2           v_tex_coord;
in vec4           v_color;
uniform sampler2D s_texture;

void main()
{
    gl_FragColor = texture2D(s_texture, v_tex_coord) * v_color;
}