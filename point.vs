#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main(){
	gl_PointSize = 10.0f;
	gl_Position = vec4(aPos,1.0);
	ourColor = vec3(1.0f,1.0f,1.0f);
}