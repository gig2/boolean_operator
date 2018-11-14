// GLSL version
#version 330 core
// vertex input
layout (location = 0)   in  vec3    in_Vertex;
layout (location = 1)   in  vec3    in_Color;

// uniform
uniform mat4    MVP;

out vec4 Color;

void    main()
{
    // Final position
    gl_Position =   MVP * vec4(in_Vertex,1.0);
    // output
    Color = vec4(in_Color,1.0);
}

