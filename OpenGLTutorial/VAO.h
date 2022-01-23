#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad\glad.h>
#include"VBO.h"

class VAO {
public:
	GLuint ID;
	VAO();

	void LinkVBO(VBO VBO, GLuint layout);
	//Binding means a certain object becomes current and whenver we we use a function that modifes an object, it modifies all the binded objects to it along with it
	void Bind();
	void Unbind();
	void Delete();
};
#endif // !VAO_CLASS_H
