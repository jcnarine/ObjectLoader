#include <Logging.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "ObjLoader.h"
#include "Shader.h"
#include "Camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define LOG_GL_NOTIFICATIONS


GLFWwindow* window;
Camera::sptr camera = nullptr;

void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	camera->ResizeWindow(width, height);
}

bool initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		LOG_ERROR("Failed to initialize GLFW");
		return false;
	}

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
	
	//Create a new GLFW window
	window = glfwCreateWindow(800, 800, "INFR1350U", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set our window resized callback
	glfwSetWindowSizeCallback(window, GlfwWindowResizedCallback);

	return true;
}

bool initGLAD() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		LOG_ERROR("Failed to initialize Glad");
		return false;
	}
	return true;
}

int main() {
	Logger::Init(); // We'll borrow the logger from the toolkit, but we need to initialize it

	//Initialize GLFW
	if (!initGLFW())
		return 1;

	//Initialize GLAD
	if (!initGLAD())
		return 1;

	// Let OpenGL know that we want debug output, and route it to our handler function
	glEnable(GL_DEBUG_OUTPUT);

	// GL states
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	camera = Camera::Create();
	camera->SetPosition(glm::vec3(0, 15, 20)); // Set initial position
	camera->SetUp(glm::vec3(0, 1, 0)); // Use a z-up coordinate system
	camera->LookAt(glm::vec3(0.0f)); // Look at center of the screen
	camera->SetFovDegrees(90.0f); // Set an initial FOV

	ObjLoader creeper("creeper.obj");
	VertexArrayObject::sptr vao = creeper.loadObj();

	Shader::sptr shader = Shader::Create();
	shader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	shader->LoadShaderPartFromFile("shaders/frag_shader.glsl", GL_FRAGMENT_SHADER);
	shader->Link();

	glm::mat4 transform = glm::mat4(1.0f);
	glm::mat3 rotate = glm::mat3(1.0f);

	double lastFrame = glfwGetTime();
	
	///// Game loop /////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Calculate the time since our last frame (dt)
		double thisFrame = glfwGetTime();
		float dt = static_cast<float>(thisFrame - lastFrame);
		
		rotate = glm::rotate(glm::mat4(1.0f), static_cast<float>(thisFrame), glm::vec3(0, 1, 0));
		transform = glm::translate(glm::mat4(1.0f), glm::vec3(-10,-15,0));
		transform = glm::rotate(transform, static_cast<float>(thisFrame), glm::vec3(0, 1, 0));

		glClearColor(0.08f, 0.17f, 0.31f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Bind();
		shader->SetUniform("LightPos", camera->GetPosition());
		shader->SetUniformMatrix("u_ModelViewProjection", camera->GetViewProjection() * transform);
		shader->SetUniformMatrix("u_Model", transform);
		shader->SetUniformMatrix("u_ModelRotation", rotate);

		vao->Render();		
		
		rotate = glm::rotate(glm::mat4(1.0f), static_cast<float>(thisFrame), glm::vec3(0, 0, -1));
		transform = glm::translate(glm::mat4(1.0f), glm::vec3(10, -15, 4));
		transform = glm::rotate(transform, static_cast<float>(thisFrame), glm::vec3(0, -1, 0));

		shader->SetUniform("LightPos", camera->GetPosition());
		shader->SetUniformMatrix("u_ModelViewProjection", camera->GetViewProjection() * transform);
		shader->SetUniformMatrix("u_Model", transform);
		shader->SetUniformMatrix("u_ModelRotation", rotate);

		vao->Render();
		
		glfwSwapBuffers(window);
		lastFrame = thisFrame;
	}

	// Clean up the toolkit logger so we don't leak memory
	Logger::Uninitialize();
	return 0;
}