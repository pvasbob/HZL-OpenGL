#pragma once

#include "VertexBuffer.h"

// The line below commented to solve the cylic include problem:  
// VertexArray.h inclde VertexBufferLayout.h
// which include Renderer.h
// which include VertexArray.h
// the way to solve this is to just declare whatever is used in this .h file,
// and include the corresponding .h file in the .cpp file.
//#include "VertexBufferLayout.h"

// just declare this class
class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void unBind() const;

private:
	unsigned int m_RendererID;
};
