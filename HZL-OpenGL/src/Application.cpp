#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	//window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
			100.0f, 100.0f, 0.0f, 0.0f,	// 0
			200.0f, 100.0f, 1.0f, 0.0f,	// 1	
			200.0f, 200.0f, 1.0f, 1.0f,	// 2
			100.0f, 200.0f, 0.0f, 1.0f	// 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};


		GLCall(glEnable(GL_BLEND))
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	
		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);



		IndexBuffer ib(indices, 6);

		//glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
		glm::vec4 vp1(100.0f, 100.0f, 0.0f, 1.0f);
		glm::vec4 vp2(200.0f, 100.0f, 0.0f, 1.0f);
		glm::vec4 vp3(200.0f, 200.0f, 0.0f, 1.0f);
		glm::vec4 vp4(100.0f, 200.0f, 0.0f, 1.0f);

		// show the conversion result line by line from the vertex position to the -1.0 ~ 1.0 
		// not gonna be red in  later.		#
		glm::vec4 result1 = proj * vp1;
		glm::vec4 result2 = proj * vp2;
		glm::vec4 result3 = proj * vp3;
		glm::vec4 result4 = proj * vp4;


		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", proj);


		Texture texture("res/textures/ChernoLogo.png");
		//Texture texture("res/textures/youyou.jpg");
		texture.Bind(0);
		// the '0' here must match the '0' right above.
		shader.SetUniform1i("u_Texture", 0);

		shader.unBind();
		va.unBind();
		vb.unBind();
		ib.unBind();

		Renderer renderer;


		float r = 0.0f;
		float increment = 0.01f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			GLCall(shader.Bind());

			GLCall(shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f));	// does not take shaderID as argument.  

			renderer.Draw(va, ib, shader);

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