#pragma once

#include <memory>
#include <gl/glew.h>

class Mesh
{
public:


public:
	Mesh(GLuint VertexBuffer, GLuint VertexArray);

private:
	GLuint VertexBuffer;
	GLuint VertexArray;
};