#include "Mesh.h"

StaticMesh::StaticMesh(GLuint VertexBuffer, GLuint VertexArray, GLsizei Count, GLenum Mode)
{
	this->VertexBuffer = VertexBuffer;
	this->VertexArray = VertexArray;
	this->Count = Count;
	this->Mode = Mode;
}

StaticMesh::~StaticMesh()
{
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteVertexArrays(1, &VertexArray);
}

void StaticMesh::Draw()
{
	glBindVertexArray(VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	glDrawArrays(Mode, 0, Count);
}
