#include "ResourceManager.h"
#include <iostream>
#include <stb_image.h>



 std::map<std::string, Shader>ResourceManager::Shaders;
 std::map<std::string, Texture>ResourceManager::Textures;

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{

    Shader shader(vShaderFile, fShaderFile, gShaderFile);
    Shaders[name] = shader;
    return Shaders[name] = shader;
}

Shader& ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{

    return Textures[name] = loadTextureFromFIle(file, alpha);
}

Texture& ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::clear()
{
    Shaders.clear();
    Textures.clear();
}



Texture ResourceManager::loadTextureFromFIle(const char* File, bool alpha)
{
	Texture texture;
	if (alpha) {

		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;

	}

	//load and generate the texture
	int width, height, nrChannels;
	unsigned char* flags;
	flags = stbi_load(File, &width, &height, &nrChannels, 0);
	if (flags) {
		texture.Generate(flags, height, width);  // тут создалась текстура 

	}
	else
	{
		std::cout << "ERROR::TEXTURE:failed to load" << std::endl;
	}
	//and finaly free image data
	stbi_image_free(flags);

    return texture;
}
