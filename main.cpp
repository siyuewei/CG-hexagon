#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <vector>
#include <chrono>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
RGBAColor baseColor = { 1.0f, 0.0f, 0.0f, 1.0f }; // 红色
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::vector<point> generateVertexData();
std::vector<RGBAColor> generateColors(const RGBAColor& baseColor, int numColors);
float* generateFColors(const RGBAColor& baseColor, int numColors);
RGBAColor interpolateColors(const RGBAColor& color1, const RGBAColor& color2, float t);
float deltaTime();
float* generateFPointSize();
float* generateFBrightness();

//快捷键Q退出各种特效
bool isDrawPointLine = false;	  //快捷键S
bool isColorChanging = false;	  //快捷键C
bool changeColorBright = false;	  //快捷键B
bool isMoving = false;	//快捷键M
bool isRotate = false;	//快捷键R
bool isPointMovingAtLine = false;	//快捷键L
RGBAColor targetColor; // 保存目标颜色
float durationInSeconds = 10000.0f; // 渐变的总时长
float elapsedTime = 0.0f; // 已经过去的时间
float nowTime = 0.0f;
float t = 0.0f;
float speed = 0.0001f;

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
	Shader movePointShader = Shader("move_point.vs", "move_point.fs");

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

	//三角形三个顶点一组存在VBO里面
	unsigned VAO[10], VBO[10];
	glGenBuffers(10, VBO);
	glGenVertexArrays(10, VAO);
	for (int i = 0; i < 10; ++i) {
		float* data = new float[3 * 4];
		point p1 = vertexData[placeIndices[i * 3] - 1];
		point p2 = vertexData[placeIndices[i * 3 + 1] - 1];
		point p3 = vertexData[placeIndices[i * 3 + 2] - 1];
		float colorPosition = colorIndices[i] - 1;
		// 第一个顶点的数据
		data[0] = p1.x;
		data[1] = p1.y;
		data[2] = 0.0f;
		data[3] = colorPosition;

		// 第二个顶点的数据
		data[4] = p2.x;
		data[5] = p2.y;
		data[6] = 0.0f;
		data[7] = colorPosition;

		// 第三个顶点的数据
		data[8] = p3.x;
		data[9] = p3.y;
		data[10] = 0.0f;
		data[11] = colorPosition;

		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(float), data, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		delete[]data;
	}

	//十个顶点存在VBO里面
	unsigned VAO_point, VBO_point;
	glGenBuffers(1, &VBO_point);
	glGenVertexArrays(1, &VAO_point);
	float point_data[] = {
		vertexData[0].x,vertexData[0].y,0.0f,0,
		vertexData[1].x,vertexData[1].y,0.0f,1,
		vertexData[2].x,vertexData[2].y,0.0f,2,
		vertexData[3].x,vertexData[3].y,0.0f,3,
		vertexData[4].x,vertexData[4].y,0.0f,4,
		vertexData[5].x,vertexData[5].y,0.0f,5,
		vertexData[6].x,vertexData[6].y,0.0f,6,
		vertexData[7].x,vertexData[7].y,0.0f,7,
		vertexData[8].x,vertexData[8].y,0.0f,8,
	};
	glBindVertexArray(VAO_point);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_point);
	glBufferData(GL_ARRAY_BUFFER, 4 * 10 * sizeof(float), point_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//用来画线上移动的点
	unsigned VAO_point_at_line, VBO_point_at_line;
	glGenBuffers(1, &VBO_point_at_line);
	glGenVertexArrays(1, &VAO_point_at_line);
	float point_at_line_data[] = {
		vertexData[0].x,vertexData[0].y,0.0f,vertexData[1].x,vertexData[1].y,0.0f, //1
		vertexData[0].x,vertexData[0].y,0.0f,vertexData[5].x,vertexData[5].y,0.0f, //2
		vertexData[0].x,vertexData[0].y,0.0f,vertexData[8].x,vertexData[8].y,0.0f, //3

		vertexData[1].x,vertexData[1].y,0.0f,vertexData[2].x,vertexData[2].y,0.0f, //4
		vertexData[1].x,vertexData[1].y,0.0f,vertexData[6].x,vertexData[6].y,0.0f, //5
		vertexData[1].x,vertexData[1].y,0.0f,vertexData[8].x,vertexData[8].y,0.0f, //6

		vertexData[2].x,vertexData[2].y,0.0f,vertexData[3].x,vertexData[3].y,0.0f, //7
		vertexData[2].x,vertexData[2].y,0.0f,vertexData[6].x,vertexData[6].y,0.0f, //8

		vertexData[3].x,vertexData[3].y,0.0f,vertexData[4].x,vertexData[4].y,0.0f, //9
		vertexData[3].x,vertexData[3].y,0.0f,vertexData[6].x,vertexData[6].y,0.0f, //10

		vertexData[4].x,vertexData[4].y,0.0f,vertexData[5].x,vertexData[5].y,0.0f, //11
		vertexData[4].x,vertexData[4].y,0.0f,vertexData[6].x,vertexData[6].y,0.0f, //12
		vertexData[4].x,vertexData[4].y,0.0f,vertexData[7].x,vertexData[7].y,0.0f, //13

		vertexData[5].x,vertexData[5].y,0.0f,vertexData[7].x,vertexData[7].y,0.0f, //14
		vertexData[5].x,vertexData[5].y,0.0f,vertexData[8].x,vertexData[8].y,0.0f, //15

		vertexData[6].x,vertexData[6].y,0.0f,vertexData[7].x,vertexData[7].y,0.0f, //16
		vertexData[6].x,vertexData[6].y,0.0f,vertexData[8].x,vertexData[8].y,0.0f, //17

		vertexData[7].x,vertexData[7].y,0.0f,vertexData[8].x,vertexData[8].y,0.0f, //18
	};
	glBindVertexArray(VAO_point_at_line);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_point_at_line);
	glBufferData(GL_ARRAY_BUFFER, 6 * 18 * sizeof(float), point_at_line_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (isColorChanging)
		{
			// 计算当前时间下的插值颜色
			float t = elapsedTime / durationInSeconds;
			baseColor = interpolateColors(baseColor, targetColor, t);

			// 更新时间
			elapsedTime += deltaTime(); // deltaTime()函数返回上一帧到当前帧的时间间隔

			// 如果渐变完成，设置标志为false
			if (elapsedTime >= durationInSeconds)
			{
				isColorChanging = false;
				elapsedTime = 0.0f;
			}
		}

		float* colors = generateFColors(baseColor, 6);
		triangleShader.use();
		triangleShader.setFloat4Array("colors", 6, colors);

		// create transformations
		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime() - nowTime, glm::vec3(0.0f, 0.0f, 1.0f));

		//六边形
		triangleShader.use();
		if (isMoving) {
			triangleShader.setFloat("xOffset", static_cast<float>(sin(glfwGetTime() / 2 + 0.5)));
			triangleShader.setFloat("yOffset", static_cast<float>(cos(glfwGetTime() / 2 + 0.5)));
		}
		else {
			triangleShader.setFloat("xOffset", 0);
			triangleShader.setFloat("yOffset", 0);
		}
		if (isRotate) {
			triangleShader.setMatrix4fv("transform", 1, transform);
		}
		else {
			triangleShader.setMatrix4fv("transform", 1, glm::mat4(1.0f));
		}
		for (int i = 0; i < 10; ++i) {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		if (isDrawPointLine) {
			//点
			pointShader.use();
			if (isMoving) {
				pointShader.setFloat("xOffset", static_cast<float>(sin(glfwGetTime() / 2 + 0.5)));
				pointShader.setFloat("yOffset", static_cast<float>(cos(glfwGetTime() / 2 + 0.5)));
			}
			else {
				pointShader.setFloat("xOffset", 0);
				pointShader.setFloat("yOffset", 0);
			}
			if (isRotate) {
				pointShader.setMatrix4fv("transform", 1, transform);
			}
			else {
				triangleShader.setMatrix4fv("transform", 1, glm::mat4(1.0f));
			}

			//生成随机点大小数组
			glEnable(GL_PROGRAM_POINT_SIZE);
			float *point_size_array = generateFPointSize();
			pointShader.setFloat1Array("pointSizeArray", 9, point_size_array);
			if (changeColorBright) {
				float* brightness_array = generateFBrightness();
				pointShader.setFloat1Array("brightnessArray", 9, brightness_array);
			}
			glBindVertexArray(VAO_point);
			glDrawArrays(GL_POINTS, 0, 9);
			// 禁用点精灵功能并切换到下一个着色器程序
			glDisable(GL_PROGRAM_POINT_SIZE);

			// 线
			lineShader.use();
			if (isMoving) {
				lineShader.setFloat("xOffset", static_cast<float>(sin(glfwGetTime() / 2 + 0.5)));
				lineShader.setFloat("yOffset", static_cast<float>(cos(glfwGetTime() / 2 + 0.5)));
			}
			else {
				lineShader.setFloat("xOffset", 0);
				lineShader.setFloat("yOffset", 0);
			}
			if (isRotate) {
				lineShader.setMatrix4fv("transform", 1, transform);
			}
			else {
				triangleShader.setMatrix4fv("transform", 1, glm::mat4(1.0f));
			}
			for (int i = 0; i < 10; ++i) {
				glBindVertexArray(VAO[i]);
				glDrawArrays(GL_LINE_LOOP, 0, 3);
			}
		}

		if (isPointMovingAtLine) {
			//线上移动
			movePointShader.use();
			if (isMoving) {
				movePointShader.setFloat("xOffset", static_cast<float>(sin(glfwGetTime() / 2 + 0.5)));
				movePointShader.setFloat("yOffset", static_cast<float>(cos(glfwGetTime() / 2 + 0.5)));
			}
			else {
				movePointShader.setFloat("xOffset", 0);
				movePointShader.setFloat("yOffset", 0);
			}
			if (isRotate) {
				movePointShader.setMatrix4fv("transform", 1, transform);
			}
			else {
				movePointShader.setMatrix4fv("transform", 1, glm::mat4(1.0f));
			}
			glEnable(GL_PROGRAM_POINT_SIZE);
			glBindVertexArray(VAO_point_at_line);
			if (t > 1.0f || t < 0.0f) {
				speed = -speed;
			}
			t += speed;
			movePointShader.setFloat("t", t);
			glDrawArrays(GL_POINTS, 0, 18);
			glDisable(GL_PROGRAM_POINT_SIZE);
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(10, VBO);
	glDeleteVertexArrays(10, VAO);

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
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		isDrawPointLine = true;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		isDrawPointLine = false;
		changeColorBright = false;
		isColorChanging = false;
		isMoving = false;
		isRotate = false;
		isPointMovingAtLine = false;
		nowTime = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		isColorChanging = true;
		targetColor = RGBAColor{ 0.0f, 0.0f, 1.0f, 1.0f };
		elapsedTime = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		if (isDrawPointLine) {
			changeColorBright = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		isMoving = true;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		isRotate = true;
		nowTime = glfwGetTime();
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		isPointMovingAtLine = true;
	}
}

std::vector<point> generateVertexData() {
	float sqrt3 = 1.732f;
	std::vector<point> finalVertex;

	unsigned int minEdge;
	float coeffi;
	if (SCR_HEIGHT > SCR_WIDTH) {
		minEdge = SCR_WIDTH;
		coeffi = (SCR_WIDTH * 1.0 / SCR_HEIGHT);
	}
	else {
		minEdge = SCR_HEIGHT;
		coeffi = (SCR_HEIGHT * 1.0 / SCR_WIDTH);
	}

	coeffi = 1;

	point p1, p2, p3, p4, p5, p6, p7, p8, p9;
	if (minEdge == SCR_WIDTH) {
		p1 = point(0.9f * sqrt3 / 2, 0.9f / 2 * coeffi);
		p2 = point(0.0f, 0.9f * coeffi);
		p3 = point(-0.9f * sqrt3 / 2, 0.9f / 2 * coeffi);
		p4 = point(-0.9f * sqrt3 / 2, -0.9f / 2 * coeffi);
		p5 = point(0.0f, -0.9f * coeffi);
		p6 = point(0.9f * sqrt3 / 2, -0.9f / 2 * coeffi);
	}
	else {
		p1 = point(0.9f * sqrt3 / 2 * coeffi, 0.9f / 2);
		p2 = point(0.0f, 0.9f);
		p3 = point(-0.9f * sqrt3 / 2 * coeffi, 0.9f / 2);
		p4 = point(-0.9f * sqrt3 / 2 * coeffi, -0.9f / 2);
		p5 = point(0.0f, -0.9f);
		p6 = point(0.9f * sqrt3 / 2 * coeffi, -0.9f / 2);
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

float* generateFColors(const RGBAColor& baseColor, int numColors)
{
	float* fcolors = new float[numColors * 4];

	for (int i = 0; i < numColors; ++i) {
		fcolors[4 * i] = baseColor.r * (1.0f - i * 0.1f); // 减小红色通道
		fcolors[4 * i + 1] = baseColor.g * (1.0f - i * 0.1f); // 减小绿色通道
		fcolors[4 * i + 2] = baseColor.b * (1.0f - i * 0.1f); // 减小蓝色通道
		fcolors[4 * i + 3] = baseColor.a;
	}
	return fcolors;
}

RGBAColor interpolateColors(const RGBAColor& color1, const RGBAColor& color2, float t)
{
	RGBAColor result;
	result.r = (1.0f - t) * color1.r + t * color2.r;
	result.g = (1.0f - t) * color1.g + t * color2.g;
	result.b = (1.0f - t) * color1.b + t * color2.b;
	result.a = (1.0f - t) * color1.a + t * color2.a;
	return result;
}

float deltaTime()
{
	static auto previousTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - previousTime;
	previousTime = currentTime;
	return deltaTime.count();
}

float* generateFPointSize()
{
	static float pointSizeArray[9] = { 0.0f };
	static float pointSizeMin[9] = { 0.0f };
	static float pointSizeMax[9] = { 0.0f };
	static float transitionStep[9] = { 0.0f }; // 用于控制每个点的变化速度
	static bool initialized = false;
	float margin = 10.0f;

	if (!initialized) {
		int min = 10, max = 20;
		std::random_device seed;
		std::ranlux48 engine(seed());
		std::uniform_int_distribution<> distrib(min, max);

		for (int i = 0; i < 9; i++) {
			pointSizeArray[i] = float(distrib(engine));
			pointSizeMin[i] = pointSizeArray[i] - margin;
			pointSizeMax[i] = pointSizeArray[i] + margin;
			transitionStep[i] = 0.01f; // 初始化每个点的变化速度
		}

		initialized = true;
	}
	else {
		for (int i = 0; i < 9; i++) {
			// 逐渐改变点大小
			pointSizeArray[i] += transitionStep[i];

			// 确保点大小在范围内，并反向变化
			if (pointSizeArray[i] > pointSizeMax[i] || pointSizeArray[i] < pointSizeMin[i]) {
				transitionStep[i] = -transitionStep[i];
			}
		}
	}

	return pointSizeArray;
}

float* generateFBrightness()
{
	static float brightnessArray[9] = { 0.0f };
	static float transitionStep[9] = { 0.0f }; // 用于控制每个点的变化速度
	static bool initialized = false;

	if (!initialized) {
		std::random_device seed;
		std::ranlux48 engine(seed());
		std::uniform_real_distribution<float> distrib(0.0f, 1.0f); // 使用 uniform_real_distribution 来生成 0 到 1 之间的随机数

		for (int i = 0; i < 9; i++) {
			brightnessArray[i] = distrib(engine);
			transitionStep[i] = 0.001f; // 初始化每个点的变化速度
		}

		initialized = true;
	}
	else {
		for (int i = 0; i < 9; i++) {
			// 逐渐改变亮度
			brightnessArray[i] += transitionStep[i];

			// 确保亮度在范围内，并反向变化
			if (brightnessArray[i] > 1.0f || brightnessArray[i] < 0.0f) {
				transitionStep[i] = -transitionStep[i];
			}
		}
	}

	return brightnessArray;
}

