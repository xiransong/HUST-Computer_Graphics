#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);  //�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������

	//GLAD����������OpenGL�ĺ���ָ���
	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  //��GLAD������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);  //����OpenGL��Ⱦ���ڵĳߴ��С

	//ע������ص�����������GLFW����ϣ��ÿ�����ڵ�����С��ʱ��������������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//��Ⱦѭ��(Render Loop)��������������GLFW�˳�ǰһֱ��������
	while (!glfwWindowShouldClose(window))
	{
		//����
		processInput(window);  //����û��Ƿ����˷��ؼ�(Esc)

		//��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //����ʹ��һ���Զ������ɫ�����Ļ
		glClear(GL_COLOR_BUFFER_BIT);
		//glClearColor������һ��״̬���ú�������glClear��������һ��״̬ʹ�õĺ���

		//��鲢�����¼�����������
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();  //����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)  //����û��Ƿ����˷��ؼ�(Esc)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
