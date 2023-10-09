#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aPlace;

out vec3 ourColor;

uniform float pointSizeArray[9];
uniform float brightnessArray[9] = {
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f
};


void main(){
	gl_PointSize = pointSizeArray[int(aPlace)];
	gl_Position = vec4(aPos,1.0);
	float brightness = brightnessArray[int(aPlace)];
    ourColor = vec3(brightness,brightness,brightness);
}