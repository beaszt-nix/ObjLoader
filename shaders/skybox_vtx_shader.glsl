#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = vec3(-1 *aPos.x, -1 * aPos.y, aPos.z);
    vec4 pos = projection * view * vec4(aPos, 0.0);
    gl_Position = pos.xyww;
} 