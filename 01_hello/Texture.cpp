#include "Texture.h"

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <iostream>

std::map<std::string, unsigned int> Texture::texturePool;

int Texture::loadTexture(const std::string& textureName, const std::string& path)
{
	if (Texture::texturePool.find(textureName) != Texture::texturePool.end()) {
		std::cout << "textureName \"" << textureName<< "\" already exists!" << std::endl;
		return -1;
	}
	
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data == NULL) {
		std::cout << "texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
		return -1;
	}

	GLenum format;
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // <- important !
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

	Texture::texturePool.insert(std::pair<std::string, unsigned int>(textureName, textureID));
	
	return 0;
}

Texture::Texture()
{
}

Texture::Texture(std::vector<std::string> textureName)
{
	setTexture(textureName);
}

Texture::~Texture()
{
}

void Texture::setTexture(std::vector<std::string> textureName)
{
	this->textureID.clear();
	for (const auto& name : textureName) {
		std::map<std::string, unsigned int>::iterator it = Texture::texturePool.find(name);
		if (it == Texture::texturePool.end()) {
			std::cout << "can't find texture name \"" << name << "\" in Texture::texturePool" << std::endl;
			this->textureID.push_back(0);
		}
		else {
			this->textureID.push_back(it->second);
		}
	}
}

void Texture::bindTexture()
{
	for (std::size_t i = 0; i < this->textureID.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textureID[i]);
	}
	glActiveTexture(GL_TEXTURE0);
}
