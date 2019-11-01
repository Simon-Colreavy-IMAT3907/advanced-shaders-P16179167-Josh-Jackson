#pragma once

#include "Win32OpenGL.h"

class TextureLoader 
{
	public:
		TextureLoader();
		~TextureLoader();

		static int LoadBMP(const string location, GLuint &textureID, bool withMipMaps);
};