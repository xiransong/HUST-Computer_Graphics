#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <map>
#include <vector>

class Texture
{
public:
	static int loadTexture(const std::string& textureName, const std::string& path);
	
	Texture();
	Texture(std::vector<std::string> textureName);
	~Texture();

	void setTexture(std::vector<std::string> textureName);
	void bindTexture();

private:
	static std::map<std::string, unsigned int> texturePool;
	
	std::vector<unsigned int> textureID;
};

#endif