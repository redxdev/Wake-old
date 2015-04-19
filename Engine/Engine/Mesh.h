#pragma once

#include <gl/glew.h>

class StaticMesh
{
public:
	StaticMesh(GLuint VertexBuffer, GLuint VertexArray, GLsizei Count, GLenum Mode);
	~StaticMesh();

	void Draw();

private:
	GLuint VertexBuffer;
	GLuint VertexArray;
	GLsizei Count;
	GLenum Mode;
};