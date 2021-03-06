#include "GLResources.hpp"
#include <glad/glad.h>
#include <iostream>
#include <vector>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <stb/stb_image.h>
#include "obfuscator.hpp"

uint32_t GLResources::CreateTexture2D(const std::string& path, bool gamma)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, nChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
	if (data == nullptr || width == 0 || height == 0 || nChannels == 0)
	{
		std::cerr << OBFUSCATE("Cannot load texture from ") << path << std::endl;
		return 0;
	}

	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	GLenum format, internalFormat;
	switch (nChannels)
	{
	case 1:
		format = GL_RED;
		internalFormat = GL_RED;
		break;
	case 3:
		format = GL_RGB;
		internalFormat = gamma ? GL_SRGB : GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGB ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGB ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(false);

	const auto dirIdx = path.find_last_of(OBFUSCATE("/"));
	const std::string filename = path.substr(dirIdx + 1);

	std::clog << OBFUSCATE("Load texture ") << filename << OBFUSCATE(" finished") << std::endl;

	return texture;
}

uint32_t GLResources::CreateTexture2D(const std::vector<unsigned char>& data, int width, int height, int channel, bool gamma)
{
	stbi_set_flip_vertically_on_load(true);

	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	GLenum format, internalFormat;
	switch (channel)
	{
	case 1:
		format = GL_RED;
		internalFormat = GL_RED;
		break;
	case 3:
		format = GL_RGB;
		internalFormat = gamma ? GL_SRGB : GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, &data[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGB ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGB ? GL_REPEAT : GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_set_flip_vertically_on_load(false);

	return texture;
}

uint32_t GLResources::LoadPresetImage(const std::string& path, int numExtension)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, nChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
	if (data == nullptr || width == 0 || height == 0 || nChannels == 0)
	{
		std::cerr << OBFUSCATE("Cannot load texture from ") << path << std::endl;
		return 0;
	} 

	std::vector<unsigned char> extendedData;
	const unsigned int size = width * height;
	extendedData.reserve(size * numExtension);

	// r1 r2 r3 r4 r5 ... 데이터를
	// r1 g1 b1 r2 g2 b2 r3 g3 b3 .... 이렇게 바꾸는 코드인데
	// 심각하게 느리다, 어떻게 개선해야하까
	for (int i = 0; i < size; ++i)
		extendedData.insert(extendedData.end(), numExtension, data[i]);

	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	nChannels = numExtension;
	GLenum format, internalFormat;
	switch (nChannels)
	{
	case 1:
		format = GL_RED;
		internalFormat = GL_RED;
		break;
	case 3:
		format = GL_RGB;
		internalFormat = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		internalFormat = GL_RGBA;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, &extendedData[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGB ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGB ? GL_REPEAT : GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	extendedData.clear();
	stbi_set_flip_vertically_on_load(false);

	const auto dirIdx = path.find_last_of(OBFUSCATE("/"));
	const std::string filename = path.substr(dirIdx + 1);

	std::clog << OBFUSCATE("Load texture ") << filename << OBFUSCATE(" finished") << std::endl;

	return texture;
}