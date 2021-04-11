#version 330 core

#ifdef GL_ES
precision mediump float;
#endif

in vec2 v_tex_coord;
// in vec4           v_color;

uniform sampler2D texMap;
uniform sampler2D texSand;
uniform sampler2D texSpice;
uniform sampler2D texStone;
uniform sampler2D texRock;
// uniform sampler2D s_texture;

void main()
{

    vec4 sand  = texture2D(texSand, v_tex_coord);
    vec4 spice = texture2D(texSpice, v_tex_coord);
    vec4 stone = texture2D(texStone, v_tex_coord);
    vec4 rock  = texture2D(texRock, v_tex_coord);

    vec4  mapV  = texture2D(texMap, v_tex_coord);
    float sandV = clamp(1.0 - (mapV.r + mapV.g + mapV.b), 0.0, 1.0);
    

    gl_FragColor =
        (sand * sandV + spice * mapV.r + stone * mapV.g + rock * mapV.b) /
        (sandV + mapV.r + mapV.g + mapV.b);

}