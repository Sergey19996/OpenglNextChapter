#ifndef  SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>


class Shader
{
public:
	//thie program ID
	unsigned int ID;

	//constructor read and builds the shdaer
	Shader(const char* vertePath, const char* fragmentPath);
	//use/activate the shader
	void use();
	//utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloatArray(const std::string& name, float* firstvalue,int size) const;
	void setIntArray(const std::string& name, int* array, int size) const;



};


#endif // ! SHADER_H
