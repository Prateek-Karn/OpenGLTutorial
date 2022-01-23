#include "VBO.h"


VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	//param (type of buffer, size of data (bytes), data, and then mode)
	// GL_STREAM - Vertices modifed once and used few times
	// GL_STATIC - Vertices will be modified once and used many times
	// GL_DYNAMIC - Vertices will be modified multiple times and used many times
	// DRAW - Vertices will be modified to drawing on screen
	// READ - Vertices will be read from the buffer
	// COPY - Vertices will be copied from the buffer
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Update(GLfloat* vertices, GLsizeiptr size)
{
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
