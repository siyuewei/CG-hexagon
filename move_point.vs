#version 460 core
layout (location = 0) in vec3 aPos1;
layout (location = 1) in vec3 aPos2;

out vec3 ourColor;
uniform float t = 1.0f;
uniform float xOffset;
uniform float yOffset;
uniform mat4 transform = mat4(1.0); // 初始值为单位矩阵

void main(){
	gl_PointSize = 10.0f;
    vec3 interpolatedPosition = aPos1 + t * (aPos2 - aPos1);
    gl_Position = transform * vec4(interpolatedPosition.x + xOffset,interpolatedPosition.y + yOffset,interpolatedPosition.z, 1.0);
    ourColor = vec3(1.0f,1.0f,1.0f);
}