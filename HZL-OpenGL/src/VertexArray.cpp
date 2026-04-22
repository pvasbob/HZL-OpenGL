#include "VertexArray.h"

// include VertexbufferLayout.h because it is commented to avoid cyclical header inclusino in VertexArray.h
#include "VertexBufferLayout.h"
// Important note: you can not just include like below to use a certain memeber function in a class:
// unsigned int VertexBufferLayout::GetStride() const
// BECAUSE:
// YU are thinking like a Linker, which is great, but the Compiler is the one who stops you first.
// The compiler needs the full definition of the class in order to:
// 1. verify access rights, like public, private, protected
// 2. determine memeory layout to make sure the object is correctly used in C++ standards.
// 3. The "This" pointer: The funciton is actually calle like GetStride(&layout). The compiler needs to full 
//    definition of the clas in order to handle the 'this' pointer correctly.
// 4~. For a normal(global) function, we can do exactly that. When i define a function outside a class, it is
//    a free function( or global function) with global scope.


#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}


VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}


void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	// bind the va
	Bind();
	// bind the vb so tht all the work recoreded by the binded va is done on this vb.
	vb.Bind();
	const std::vector<VertexBufferLayoutElement>& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		// glVertexAttribPointer is not a general-purpose function. It is hard-wired to look only at the GL_ARRAY_BUFFER slot.
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
	}

}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unBind() const 
{
	GLCall(glBindVertexArray(0));
}




