#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Learning", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// VSync
	glfwSwapInterval(1);

	// Init glew and check version if ok
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error with glewInit()" << std::endl;
	}

	{
		float positions[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// vertex array object
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		shader.SetUniform4f("u_Color", 0.6f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/checker.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		float r = 0.0f;
		float increment = 0.05f;

		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		Renderer renderer;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);

			if (r > 1.0f)
				r = 0.0f;
			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}