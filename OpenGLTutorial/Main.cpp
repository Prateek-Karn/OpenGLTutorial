#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"shaderClass.h"
#include<thread>
#include<chrono>
#include<vector>
#include<numeric>
#define WOBBLE 0.5

float_t period = 0;
void wiggle(GLfloat* vertices, GLsizeiptr size) {
	
	double pi = 2 * acos(0.0);
	using namespace std::chrono_literals;
	vertices[0] = (WOBBLE * sin( pi * period) + 1) * -0.5f ;
	vertices[1] = (WOBBLE * cos( pi * period) + 1) * -0.5f * float(sqrt(3)) / 3;
	vertices[3] = (WOBBLE * sin( pi * period + (2 * pi / 3)) + 1) * 0.5f ;
	vertices[4] = (WOBBLE * cos( pi * period + (2 * pi / 3)) + 1) * -0.5f * float(sqrt(3)) / 3 ;
	vertices[6] = (WOBBLE * sin( pi * period + (4 * pi / 3)) * 0.5f) ;
	vertices[7] = (WOBBLE * cos( pi * period + (4 * pi / 3)) + 1) * 1.0f * float(sqrt(3)) / 3 ;
	vertices[9] = (WOBBLE / 3 * cos(-1 * pi * period + (0 * pi / 3)) + 1) * -0.5f / 2 ;
	vertices[10]= (WOBBLE / 3 * cos(-1 * pi * period + (0 * pi / 3)) + 1) * 0.5f * float(sqrt(3)) / 6 ;
	vertices[12]= (WOBBLE / 3 * cos(-1 * pi * period + (2 * pi / 3)) + 1) * 0.5f / 2 ;
	vertices[13]= (WOBBLE / 3 * cos(-1 * pi * period + (2 * pi / 3)) + 1) * 0.5f * float(sqrt(3)) / 6 ;
	vertices[15]= (WOBBLE / 3 * cos(-1 * pi * period + (4 * pi / 3)) * 0.5f)  ;
	vertices[16]= (WOBBLE / 3 * cos(-1 * pi * period + (4 * pi / 3)) + 1) * -0.5f * float(sqrt(3)) / 3;

	//std::this_thread::sleep_for(20ms);
}
int main() {
	//initialise glfw
	glfwInit();
	//GLFW doesnt know which version we are using so we need to tell it that
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//opengl profile is like package of functions, core and compatibility
	//core has all modern ones and compatibility includes legacy ones
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//create a window param(width, height, Title, Fullscreen?, not important)
	GLFWwindow* window = glfwCreateWindow(800, 800, "Hello world", NULL, NULL);
	//error check if window is created or not
	if (window == NULL) {
		std::cerr << "Glfw window failed";
		glfwTerminate();
		return -1;
	}
	//we need to make the window the current context (i.e we want to use it)
	glfwMakeContextCurrent(window);

	//load in the glad to tell it to do open gl
	gladLoadGL();

	//viewport of window means the area of window we want to be rendering in
	glViewport(0, 0, 800, 800);

	
	//we are going to do 2d, coordinate system is normalised, leftmost is -1, righmost is +1 in x, similarly with y +1 is top -1 is bottom. 
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //left vertex
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // right vertex
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // top vertex
		-0.5f / 2, 0.5f * float(sqrt(3)) /6, 0.0f, //inner left
		0.5f / 2, 0.5f * float(sqrt(3)) /6, 0.0f,// inner right
		0.0f, -0.5f * float(sqrt(3)) / 3 , 0.0f // inner down
	};

	GLuint indices[] =
	{
		0,3,5, // lower left triangle
		3,2,4, // lower right traiangle
		5,4,1 // upper triangle
	};

	Shader shaderProgram("default.vert", "default.frag");
	//sending data from cpu to gpu is kinda slow, so we make buffers to send batches of information between them called buffers
	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	VAO1.LinkVBO(VBO1, 0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();



	//modify the clear function to have a better background colour
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);// navy blue, 4th param is alpha (0 transparent 1 opaque) 
	//now we write to the back buffer what we want to do, i.e clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	//swap front and back buffer to see it displayed
	glfwSwapBuffers(window);
	
	auto currentTime = std::chrono::system_clock::now();
	auto lastTime = currentTime;

	// lets make a loop to keep the window up until we tell it to close
	while (!glfwWindowShouldClose(window)) {
		float_t dt = std::chrono::duration_cast<std::chrono::duration<float_t>>(currentTime - lastTime).count();
		lastTime = currentTime;

		//modify the clear function to have a better background colour
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);// navy blue, 4th param is alpha (0 transparent 1 opaque) 
		//now we write to the back buffer what we want to do, i.e clear the screen
		glClear(GL_COLOR_BUFFER_BIT);
		//run the shader program
		shaderProgram.Activate();
		//bind the vao to use it
		VAO1.Bind();
		std::thread t1(wiggle, vertices, sizeof(vertices));
		t1.join();
		VBO1.Bind();
		VBO1.Update(vertices, sizeof(vertices));
		VBO1.Unbind();
		//specify primitive we want to draw
		//glDrawArrays(GL_TRIANGLES, 0, 3); // for good ol
		//for using index buffer to draw 3 triangles param (primitive, no of indices to draw, data type of indices, index of indices)
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		//swap front and back buffer to see it displayed
		glfwSwapBuffers(window);
		currentTime = std::chrono::system_clock::now();
		//if we click on the window and we are resizing, interacting with it, 
		// if it doesnt do anything, windows will think its not responding so this function
		glfwPollEvents();
		period += dt;
		std::cout << "Frame time: " << dt << "\n";
		
	}
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	
	//for glfwCreateWindow()
	glfwDestroyWindow(window);
	//for glfwInit()
	glfwTerminate();
	std::cin.get();
}