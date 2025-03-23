#include "Shader.h"
#include <sstream>  // for reading shaders

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{


	std::string VertexShaderCode;
	std::string FragmentShaderCode;

	try {

		//open file
		std::ifstream vertexShaderFile(vertexPath); //открываем файл
		std::stringstream vertexShaderStream;  // cоздаём мост для стринга

		std::ifstream fragmentShaderFile(fragmentPath); //Открываем файл
		std::stringstream fragmentShaderStream; //создаем мост для стринга

		//read file buffer contents into stream
		vertexShaderStream << vertexShaderFile.rdbuf(); // пишем в мост
		fragmentShaderStream << fragmentShaderFile.rdbuf(); //пишем в мост

		vertexShaderFile.close(); //закрываем файл
		fragmentShaderFile.close();
		VertexShaderCode = vertexShaderStream.str(); //c моста передаем в стринг (конвертируем)
		FragmentShaderCode = fragmentShaderStream.str();


	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader file" << std::endl;

	}
	
	const char* vertexShaderSource = VertexShaderCode.c_str();
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

	}
	/* */

	const char* fragmentShaderSource = FragmentShaderCode.c_str();
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

	}


	/* */
	
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;

	}
	glUseProgram(ID);


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	
	int timeValueLocation = glGetUniformLocation(ID, name.c_str());
	glUniform1i(timeValueLocation, value);
}

void Shader::setInt(const std::string& name, int value) const
{
	
	int timeValueLocation = glGetUniformLocation(ID, name.c_str());
	glUniform1i(timeValueLocation, value);
}

void Shader::setFloat(const std::string& name, float value) const
{

	int timeValueLocation = glGetUniformLocation(ID, name.c_str());
	glUniform1f(timeValueLocation, value);
}

void Shader::setFloatArray(const std::string& name, float* firstvalue,int size) const
{
	int timeValueLocation = glGetUniformLocation(ID, name.c_str());
	glUniform1fv(timeValueLocation,size, firstvalue);

}

void Shader::setIntArray(const std::string& name, int* array, int size) const
{
	int timeValueLocation = glGetUniformLocation(ID, name.c_str());
	glUniform1iv(timeValueLocation, size, array);
}
