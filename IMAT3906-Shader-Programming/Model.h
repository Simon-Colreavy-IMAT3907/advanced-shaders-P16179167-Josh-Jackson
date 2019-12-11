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

	Model(const char *path, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot, std::string drawType);

	void draw(GLuint &program);

	glm::mat4 m_modelMatrix;

private:
	std::string m_drawType;
	TextureLoader m_textureLoader = TextureLoader();
	std::vector<Texture> m_texturesLoaded;

	void loadModel(std::string path, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot);

	std::vector<Mesh> m_meshes;
	std::string m_directory;

	void processNode(aiNode *node, const aiScene *scene, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};