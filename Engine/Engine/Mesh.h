#pragma once

#include <memory>
#include <gl/glew.h>

class StaticMesh
{
public:


public:
	StaticMesh(GLuint VertexBuffer, GLuint VertexArray, GLsizei Count);
	~StaticMesh();

	void Draw();

private:
	GLuint VertexBuffer;
	GLuint VertexArray;
	GLsizei Count;
};