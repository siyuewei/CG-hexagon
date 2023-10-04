#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aColorPosition;

uniform vec4 colors[6];
out vec4 ourColor;

void main()
{
	gl_Position = vec4(aPos.x , aPos.y , aPos.z, 1.0);
	ourColor = colors[int(aColorPosition)];
}