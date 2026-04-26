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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

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

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		// moving cameras 100 unit to the right, which is equal to moving object to left 100 units.
		glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0,0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
		// the multiply order is pvm becasue of column major in opengl
		glm::mat4 mvp   = proj * view * model;

		//position matrix in lines 
		//glm::vec4 vp1(100.0f, 100.0f, 0.0f, 1.0f);
		//glm::vec4 vp2(200.0f, 100.0f, 0.0f, 1.0f);
		//glm::vec4 vp3(200.0f, 200.0f, 0.0f, 1.0f);
		//glm::vec4 vp4(100.0f, 200.0f, 0.0f, 1.0f);

		// show the conversion result line by line from the vertex position to the -1.0 ~ 1.0 
		// not gonna be red in  later.		#
		//glm::vec4 result1 = proj * vp1;
		//glm::vec4 result2 = proj * vp2;
		//glm::vec4 result3 = proj * vp3;
		//glm::vec4 result4 = proj * vp4;


		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp);


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
		
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();


		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

 


		float r = 0.0f;
		float increment = 0.01f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();


			ImGui_ImplGlfwGL3_NewFrame();


			GLCall(shader.Bind());

			GLCall(shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f));	// does not take shaderID as argument.  

			renderer.Draw(va, ib, shader);

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			{
				static float f = 0.0f;
				static int counter = 0;
				ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}




			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}


	//glDeleteProgram(shader);
	// Add this scope to first trigger the annihilation operator, then execute glfwTerminate(). Otherwise GLGetError() show error.
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}