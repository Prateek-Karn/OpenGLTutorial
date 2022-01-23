#include "VAO.h"

VAO::VAO()
{
	// make a VAO which is a reference to which VBO we are using
	glGenVertexArrays(1, &ID); // make sure to generate before VBO
}

void VAO::LinkVBO(VBO VBO, GLuint layout)
{
	//its a way to communicate between vertex shader from outside
	//param (posn of vertex attrib, how many values for a vertex, data type, if normalised then false otherwise if you need to normalise then put true, stride = amount of data between a vertex, where does the vertex beign in the array)
	VBO.Bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
