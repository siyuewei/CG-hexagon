#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aColorPosition;

uniform vec4 colors[6];
uniform float xOffset;
uniform float yOffset;
uniform mat4 transform = mat4(1.0); // 初始值为单位矩阵

out vec4 ourColor;

void main()
{
	gl_Position = transform * vec4(aPos.x+xOffset , aPos.y+yOffset , aPos.z, 1.0);
	ourColor = colors[int(aColorPosition)];
}