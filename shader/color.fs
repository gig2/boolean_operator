//GLSL version
#version 330 core
//Input
in vec3 Color;

// Output
layout (location = 0) out vec4 out_Color;

void main()
{
    out_Color   =   vec4(Color.xyz,1.);

}

