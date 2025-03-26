#include "Shader.h"
#include <sstream>  // for reading shaders
#include<glm/gtc/type_ptr.hpp>

Shader::Shader()
{
}

Shader::Shader(const char* vertexPath, const char* fragmentPath,const char* fgeometryShader)
{


	std::string VertexShaderCode;
	std::string FragmentShaderCode;
	std::string GeometryShaderCode;

	try {

		//open file
		std::ifstream vertexShaderFile(vertexPath); //��������� ����
		std::stringstream vertexShaderStream;  // c����� ���� ��� �������

		std::ifstream fragmentShaderFile(fragmentPath); //��������� ����
		std::stringstream fragmentShaderStream; //������� ���� ��� �������

		//read file buffer contents into stream
		vertexShaderStream << vertexShaderFile.rdbuf(); // ����� � ����
		fragmentShaderStream << fragmentShaderFile.rdbuf(); //����� � ����

		vertexShaderFile.close(); //��������� ����
		fragmentShaderFile.close();
		VertexShaderCode = vertexShaderStream.str(); //c ����� �������� � ������ (������������)
		FragmentShaderCode = fragmentShaderStream.str();

		if (fgeometryShader) {
			std::ifstream GeometryShaderFile(GeometryShaderCode); 
			std::stringstream GeometryShaderStream;

			GeometryShaderStream << GeometryShaderFile.rdbuf();
			GeometryShaderCode = GeometryShaderCode.c_str();
			GeometryShaderFile.close();

		}


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
	
	unsigned int geometryShader;
	if (fgeometryShader) {
		const char* geometryShaderSource = GeometryShaderCode.c_str();
		geometryShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;

		}



	}


	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	if (fgeometryShader)
	glAttachShader(ID, geometryShader);

	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;

	}
	glUseProgram(ID);


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (fgeometryShader)
	glDeleteShader(geometryShader);
}

Shader& Shader::use()
{
	glUseProgram(ID);
	return *this;
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

void Shader::setMat4(const std::string& name, glm::mat4 matrix)
{
	int timeValueLocation = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(timeValueLocation,1,GL_FALSE, glm::value_ptr(matrix));
}
