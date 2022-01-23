#include"shaderClass.h"
// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

//write the constructor
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//lets make reference to store our shader in (because opengl only works on objects)
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//tell where the shader source code is
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	//compile it because gpu cant understand source code
	glCompileShader(vertexShader);

	//do same for fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//now we need to wrap up the shaders into a shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	//link it together
	glLinkProgram(ID);

	//delete the shaders objects we created as we dont need it anymore after attaching it
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void Shader::Activate() {
	//activate the shader program
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}