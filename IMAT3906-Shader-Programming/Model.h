#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "Win32OpenGL.h"
#include <iostream>
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "TextureLoader.h"

class Model
{
public:
	Model();

	Model(char *path);

	void draw(GLuint &program);

	glm::mat4 m_modelMatrix;

private:
	TextureLoader m_textureLoader = TextureLoader();
	std::vector<Texture> m_texturesLoaded;

	void loadModel(std::string path);

	std::vector<Mesh> m_meshes;
	std::string m_directory;

	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};