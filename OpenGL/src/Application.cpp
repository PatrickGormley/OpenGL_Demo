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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

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
	window = glfwCreateWindow(960, 540, "Learning", NULL, NULL);
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
		/*float positions[] =
		{
			100.0f, 100.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 1.0f, 0.0f,
			200.0f, 200.0f, 1.0f, 1.0f,
			100.0f, 200.0f, 0.0f, 1.0f


			-0.5f, -0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, 1.0f, 0.0f,
			0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};*/
		float positions[] =
		{
			-50.0f, -50.0f, 0.0f, 0.0f,
			50.0f, -50.0f, 1.0f, 0.0f,
			50.0f, 50.0f, 1.0f, 1.0f,
			-50.0f, 50.0f, 0.0f, 1.0f
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

		//glm::mat4 projMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
		glm::mat4 projMatrix = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
		//glm::mat4 mpvMatrix = projMatrix * viewMatrix * modelMatrix;


		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		shader.SetUniform4f("u_Color", 0.6f, 0.3f, 0.8f, 1.0f);
		//shader.SetUniformMat4f("u_MVP", mpvMatrix);

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
		
		// Setup ImGui binding
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		// ImGui
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// Sending items to ImGui
		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();
			ImGui_ImplGlfwGL3_NewFrame();

			// Test obj 1
			{
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 mpvMatrix = projMatrix * viewMatrix * modelMatrix;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mpvMatrix);

				renderer.Draw(va, ib, shader);
			}

			// Test obj 2
			{
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mpvMatrix = projMatrix * viewMatrix * modelMatrix;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mpvMatrix);

				renderer.Draw(va, ib, shader);
			}

			if (r > 1.0f)
				r = 0.0f;
			r += increment;


			{      
				ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
				ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}