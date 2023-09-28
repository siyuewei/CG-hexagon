#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main(){
	gl_PointSize = 100.0f;
	gl_Position = vec4(aPos*0.9f,1.0);
	ourColor = aColor;
}