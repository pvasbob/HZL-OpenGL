#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	//while (glGetError() != GL_NO_ERROR);
	while (glGetError()) 
	{
		std::cout << "Clear Errors" << std::endl;
	};
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while(GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function << 
			" " << file << line <<  std::endl;
		return false;
	}
	return true;
}


void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const  {
	
	shader.Bind();
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3*sizeof(unsigned int)));

};

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
