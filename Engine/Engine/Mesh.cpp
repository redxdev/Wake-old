#include "Mesh.h"

StaticMesh::StaticMesh(GLuint VertexBuffer, GLuint VertexArray, GLsizei Count)
{
	this->VertexBuffer = VertexBuffer;
	this->VertexArray = VertexArray;
	this->Count = Count;
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

	glDrawArrays(GL_TRIANGLES, 0, Count);
}
