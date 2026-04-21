#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;


	{

		float positions[] = {
			-0.5f, -0.5f,	// 0
			 0.5f, -0.5f,	// 1	
			 0.5f,  0.5f,	// 2
			-0.5f,  0.5f	// 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		unsigned int vao;


		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);


		IndexBuffer ib(indices, 6);


		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		shader.unBind();
		va.unBind();
		vb.unBind();
		ib.unBind();


		float r = 0.0f;
		float increment = 0.01f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);


			GLCall(shader.Bind());

			GLCall(shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f));	// does not take shaderID as argument.  

			va.Bind();
			ib.Bind();
			//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			//	
			//GLClearError();
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3*sizeof(unsigned int)));
			//ASSERT(GLLogCall());


			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;




			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}


	//glDeleteProgram(shader);
	// Add this scope to first trigger the annihilation operator, then execute glfwTerminate(). Otherwise GLGetError() show error.
	}

	glfwTerminate();
	return 0;
}