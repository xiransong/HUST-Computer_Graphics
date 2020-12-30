#if 1
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "MyIcosphere.h"

#include <iostream>
#include <assert.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader planetShader("vertex.vs", "planet.fs");
	Shader sunShader("vertex.vs", "sun.fs");

	// shader configuration (light and texture setting)
	planetShader.use();
	planetShader.setVec3("pointLight.position", 0.0f, 0.0f, 0.0f);
	planetShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
	planetShader.setFloat("pointLight.constant", 1.0f);
	planetShader.setFloat("pointLight.linear", 0.0014);
	planetShader.setFloat("pointLight.quadratic", 0.000007);
	planetShader.setInt("material.diffuse", 0);
	planetShader.setInt("material.specular", 1);
	planetShader.setFloat("material.shininess", 32.0f);
	sunShader.use();
	sunShader.setInt("texture1", 0);

	// load texture
	Texture::loadTexture("earth-diffuse", "pic/icoearth.jpg");
	Texture::loadTexture("earth-specular", "pic/icoearth.jpg");
	Texture::loadTexture("moon-diffuse", "pic/icomoon.jpg");
	Texture::loadTexture("moon-specular", "pic/icomoon.jpg");
	Texture::loadTexture("sun", "pic/icosun.jpg");

	// create mesh
	MyIcosphere earthPrimitive(0.5, 4, true);
	Texture earthTexture(std::vector<std::string>({ "earth-diffuse", "earth-specular" }));
	Mesh earthMesh(&earthPrimitive, &earthTexture);

	MyIcosphere moonPrimitive(0.3, 4, true);
	Texture moonTexture(std::vector<std::string>({ "moon-diffuse", "moon-specular" }));
	Mesh moonMesh(&moonPrimitive, &moonTexture);

	MyIcosphere sunPrimitive(0.7, 5, true);
	Texture  sunTexture(std::vector<std::string>({ "sun" }));
	Mesh sunMesh(&sunPrimitive, &sunTexture);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		
		// be sure to activate shader when setting uniforms/drawing objects
		planetShader.use();
		planetShader.setVec3("viewPos", camera.Position);
		planetShader.setMat4("projection", projection);
		planetShader.setMat4("view", view);
		sunShader.use();
		sunShader.setVec3("viewPos", camera.Position);
		sunShader.setMat4("projection", projection);
		sunShader.setMat4("view", view);

		// world transformation and render
		planetShader.use();

		float earthOrbitRadius = 6.0f;
		glm::mat4 modelEarth = glm::mat4(1.0f);  // draw earth
		modelEarth = glm::rotate(modelEarth, (float)glfwGetTime() * 0.3f, glm::vec3(0.0f, 1.0f, 0.0f));
		modelEarth = glm::translate(modelEarth, glm::vec3(-earthOrbitRadius, 0.0f, 0.0f));

		planetShader.setMat4("model", modelEarth);
		planetShader.setVec3("pointLight.diffuse", 0.95f, 0.95f, 0.95f);
		planetShader.setVec3("pointLight.specular", 0.9f, 0.9f, 0.9f);
		earthMesh.draw();	

		float moonOrbitRadius = 2.0f;
		glm::mat4  modelMoon = glm::mat4(1.0f);  // draw moon
		modelMoon = modelEarth * modelMoon;
		modelMoon = glm::rotate(modelMoon, (float)glfwGetTime(), glm::vec3(0.3f, 0.4f, 0.0f));
		modelMoon = glm::translate(modelMoon, glm::vec3(0.0f, 0.0f, moonOrbitRadius));
		planetShader.setMat4("model", modelMoon);
		planetShader.setVec3("pointLight.diffuse", 0.95f, 0.95f, 0.95f);
		planetShader.setVec3("pointLight.specular", 0.1f, 0.1f, 0.1f);
		moonMesh.draw();

		sunShader.use();

		glm::mat4 modelSun = glm::mat4(1.0f);  // draw sun
		sunShader.setMat4("model", modelSun);
		sunMesh.draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &lightCubeVAO);
	//glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

#endif