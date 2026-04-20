#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



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
