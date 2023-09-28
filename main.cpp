#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <vector>
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

struct point
{
	float x;
	float y;
	point(float x, float y) {
		this->x = x;
		this->y = y;
	}
	point() {

	}
};
struct RGBAColor {
	float r, g, b, a;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::vector<point> generateVertexData();
std::vector<RGBAColor> generateColors(const RGBAColor& baseColor, int numColors);

int main() {
	//1.initial
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // _APPLE_

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGl : textrue", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initial GLAD" << std::endl;
		return -1;
	}


	//2.Shader
	Shader triangleShader = Shader("triangle.vs", "triangle.fs");
	Shader pointShader = Shader("point.vs", "point.fs");
	Shader lineShader = Shader("line.vs", "line.fs");


	//3.data
	std::vector<point> vertexData = generateVertexData();
	unsigned int placeIndices[] = {
		2,3,7, //tri 1
		2,7,9, //tri 2
		1,2,9, //tri 3
		3,4,7, //tri 4
		7,8,9, //tri 5
		6,8,9, //tri 6
		1,6,9, //tri 7
		4,5,7, //tri 8
		5,7,8, //tri 9
		5,6,8  //tri 10
	};

	unsigned int colorIndices[] = {
		1,3,5,2,5,3,6,4,6,2
	};
	RGBAColor baseColor = { 1.0f, 0.0f, 0.0f, 1.0f }; // 红色
	std::vector<RGBAColor> colors = generateColors(baseColor, 6);

	unsigned VAO[10], VBO[10];
	glGenBuffers(10, VBO);
	glGenVertexArrays(10, VAO);
	for (int i = 0; i < 10; ++i) {
		float* data = new float[3 * 6];
		point p1 = vertexData[placeIndices[i * 3] - 1];
		point p2 = vertexData[placeIndices[i * 3 + 1] - 1];
		point p3 = vertexData[placeIndices[i * 3 + 2] - 1];
		RGBAColor color = colors[colorIndices[i] - 1];
		// 第一个顶点的数据
		data[0] = p1.x;
		data[1] = p1.y;
		data[2] = 0.0f;
		data[3] = color.r;
		data[4] = color.g;
		data[5] = color.b;

		// 第二个顶点的数据
		data[6] = p2.x;
		data[7] = p2.y;
		data[8] = 0.0f;
		data[9] = color.r;
		data[10] = color.g;
		data[11] = color.b;

		// 第三个顶点的数据
		data[12] = p3.x;
		data[13] = p3.y;
		data[14] = 0.0f;
		data[15] = color.r;  
		data[16] = color.g; 
		data[17] = color.b; 

		//for (int j = 0; j <= 11; ++j) {
		//	std::cout << data[j] << " ";
		//}
		//std::cout << std::endl;
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, 3*6*sizeof(float), data, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		delete []data;
	}

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//六边形
		triangleShader.use();
		for (int i = 0; i < 10; ++i) {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		//点
		pointShader.use();
		glEnable(GL_PROGRAM_POINT_SIZE);
		for (int i = 0; i < 10; ++i) {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_POINTS, 0, 3);
		}
		// 禁用点精灵功能并切换到下一个着色器程序
		glDisable(GL_PROGRAM_POINT_SIZE);

		// 线
		lineShader.use();
		for (int i = 0; i < 10; ++i) {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_LINE_LOOP, 0, 3);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//glDeleteBuffers(10, VBO);
	//glDeleteVertexArrays(1, &VAO);

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

std::vector<point> generateVertexData() {
	float oneOverSqrt3 = 0.57735f;
	

	std::vector<point> finalVertex;

	unsigned int minEdge;
	float coeffi;
	if (SCR_HEIGHT > SCR_WIDTH) {
		minEdge = SCR_WIDTH;
		coeffi = (SCR_WIDTH*1.0 / SCR_HEIGHT);
	}
	else {
		minEdge = SCR_HEIGHT;
		coeffi = (SCR_HEIGHT*1.0 / SCR_WIDTH);
	}

	point p1, p2, p3, p4, p5, p6, p7, p8, p9;
	if (minEdge == SCR_WIDTH) {
		p1 = point(0.9f, 0.9f * oneOverSqrt3 * coeffi);
		p2 = point(0.0f, 0.9f * coeffi);
		p3 = point(-0.9f, 0.9f * oneOverSqrt3 * coeffi);
		p4 = point(-0.9f, -0.9f * oneOverSqrt3 * coeffi);
		p5 = point(0.0f, -0.9f * coeffi);
		p6 = point(0.9f, -0.9f * oneOverSqrt3 * coeffi);
	}
	else {
		p1 = point(0.9f / 2 * oneOverSqrt3 * coeffi, 0.9f / 2);
		p2 = point(0.0f, 0.9f);
		p3 = point(-0.9f / 2 * oneOverSqrt3 * coeffi, 0.9f / 2);
		p4 = point(-0.9f / 2 * oneOverSqrt3 * coeffi, -0.9f / 2);
		p5 = point(0.0f, -0.9f);
		p6 = point(0.9f / 2 * oneOverSqrt3 * coeffi, -0.9f / 2);
	}
	p7.x = p3.x * 0.6f;
	p7.y = 0.0f;
	p8.x = p1.x * 0.4f;
	p8.y = p1.y * -1.1f;
	p9.x = p1.x * 0.6f;
	p9.y = p1.y * 0.5f;

	finalVertex.push_back(p1);
	finalVertex.push_back(p2);
	finalVertex.push_back(p3);
	finalVertex.push_back(p4);
	finalVertex.push_back(p5);
	finalVertex.push_back(p6);
	finalVertex.push_back(p7);
	finalVertex.push_back(p8);
	finalVertex.push_back(p9);

	return finalVertex;
}

std::vector<RGBAColor>  generateColors(const RGBAColor& baseColor, int numColors) {

	//float* fcolors = new float[numColors*4];

	//for (int i = 0; i < numColors; ++i) {
	//	fcolors[4 * i] = baseColor.r * (1.0f - i * 0.1f); // 减小红色通道
	//	fcolors[4 * i + 1] = baseColor.g * (1.0f - i * 0.1f); // 减小绿色通道
	//	fcolors[4 * i + 2] = baseColor.b * (1.0f - i * 0.1f); // 减小蓝色通道
	//	fcolors[4 * i + 3] = baseColor.a;
	//}

	//return fcolors;

	std::vector<RGBAColor> colors;
	colors.push_back(baseColor);

	for (int i = 1; i < numColors; ++i) {
		RGBAColor newColor;
		newColor.r = baseColor.r * (1.0f - i * 0.15f); // 减小红色通道
		newColor.g = baseColor.g * (1.0f - i * 0.15f); // 减小绿色通道
		newColor.b = baseColor.b * (1.0f - i * 0.15f); // 减小蓝色通道
		newColor.a = baseColor.a;

		colors.push_back(newColor);
	}

	return colors;
}