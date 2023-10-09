#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aSizePlace;

out vec3 ourColor;
uniform float xOffset;
uniform float yOffset;
uniform mat4 transform = mat4(1.0); // 初始值为单位矩阵

void main(){
	gl_Position = transform * vec4(aPos.x+xOffset , aPos.y+yOffset , aPos.z, 1.0);
	ourColor = vec3(1.0f,1.0f,1.0f);
}